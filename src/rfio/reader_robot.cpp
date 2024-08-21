#include "reader_robot.h"

#include <iostream>

#include "rfbase/datautils.h"
#include "rfbase/rfjson.h"
#include "rfgeo3d/geodataio.h"

#include "reader_base.h"
#include "reader_brep.h"

namespace rfio
{

RobotReader::RobotReader()
{
}

RobotReader::~RobotReader()
{
}

bool RobotReader::readFile(const std::string &filepath, rfapp::TaskProgress *)
{
    file_path_ = filepath;
    using namespace rfbase;
    RfJson contextJson;
    if (!JsonUtils::loadJson(filepath, contextJson))
    {
        return false;
    }
    const RfJsonArray &robot_para_path = JsonUtils::getJsonArray(contextJson, "ARM");
    // TODO: only load the first robot
    const RfJson &front_robot = robot_para_path[0];
    std::string robot_para_path_str = JsonUtils::getString(front_robot, "parampath");
    std::string robot_model_path_str = JsonUtils::getString(front_robot, "path");

    const std::string &current_folder = rfbase::DataUtils::getFilePath(filepath);

    source_path_ = source_path_set_ ? source_path_ : current_folder + "../resources/";
    robot_para_path_str = current_folder + robot_para_path_str;
    robot_model_path_str = source_path_ + robot_model_path_str;

    std::cout << "robot_para_path_str: " << robot_para_path_str << '\n';
    std::cout << "robot_model_path_str: " << robot_model_path_str << '\n';

    return loadParams(robot_para_path_str, arm_params_) &&
           loadModel(robot_model_path_str, robot_model_);
}

rfdb::dbVariant RobotReader::transfer(rfapp::TaskProgress *, bool /* concurrent */)
{
    rfdb::dbRobot *robot = new rfdb::dbRobot();
    robot->setJoints(arm_params_);
    robot->setRobotStateDeg({0, 0, 0, 0, 0, 0});
    robot->setModel(robot_model_);
    return rfdb::dbVariant(robot);
}

// check item exist: "Joints""Trsf""name""path""base_orientation"
bool checkRobotJsonValid(const rfbase::RfJson &armJson)
{
    if (!armJson.contains("Joints") || !armJson.contains("Trsf") || !armJson.contains("Name") ||
        !armJson.contains("base_orientation"))
    {
        return false;
    }
    return true;
}

std::string RobotReader::getRobotParamPath(const std::string &filename, const std::string &robot_id)
{
    rfbase::RfJson contextJson;
    if (!rfbase::JsonUtils::loadJson(filename, contextJson))
    {
        return "";
    }

    const std::string &res = contextJson[robot_id.c_str()]["parampath"].get<std::string>();
    return res;
}

std::string RobotReader::getRobotModelPath(const std::string &filename, const std::string &robot_id)
{
    rfbase::RfJson contextJson;
    if (!rfbase::JsonUtils::loadJson(filename, contextJson))
    {
        return "";
    }

    return contextJson[robot_id.c_str()]["path"].get<std::string>();
}

bool RobotReader::loadParams(const std::string &filename,
                             std::vector<rfdb::dbRobot::JointParams> &arm_params)
{
    using namespace rfbase;
    RfJson arm_para_json;
    if (!JsonUtils::loadJson(filename, arm_para_json))
    {
        return false;
    }

    if (!checkRobotJsonValid(arm_para_json))
    {
        return false;
    }
    int arm_num = JsonUtils::getInt(arm_para_json, "JointCount");

    const RfJson &models_obj = arm_para_json["Joints"];

    // NOTE: if joint count if 6, joint part count is 7, the first one is base
    for (int i = 1; i <= arm_num; i++)
    {
        const auto &name = rfdb::dbRobot::getJointName(i);
        if (!models_obj.contains(name))
        {
            continue;
        }
        const RfJson &arm_info = models_obj[name];

        rfdb::dbRobot::JointParams joint_params;

        std::vector<std::string> models_name_list;
        JsonUtils::getArrayValue(arm_info, "id", models_name_list);
        for (const auto &model_name : models_name_list)
        {
            joint_params.models_name_list_.push_back(model_name);
        }
        joint_params.arm_rot_pnt_ =
            rfgeo::GeoDataIO::jsonArrayToPt(JsonUtils::getJsonArray(arm_info, "arm_rot_pnt"));
        joint_params.dh_info_ =
            rfgeo::GeoDataIO::jsonArrayToPt(JsonUtils::getJsonArray(arm_info, "L"));
        joint_params.min_v_degree_ = arm_info["Neg"].get<double>();
        joint_params.max_v_degree_ = arm_info["Pos"].get<double>();
        joint_params.type_ = strToJointType(arm_info["type"].get<std::string>());
        arm_params.emplace_back(joint_params);
    }
    return true;
}

bool RobotReader::loadModel(const std::string &filename, rfdb::dbVariant &part)
{
    BrepReader reader;
    if (!reader.readFile(filename, nullptr))
    {
        return false;
    }

    part = reader.transfer(nullptr);

    return true;
}

rfdb::dbRobot::JType RobotReader::strToJointType(const std::string &str)
{
    if (str == "RotX")
        return rfdb::dbRobot::JType::RotX;
    if (str == "RotY")
        return rfdb::dbRobot::JType::RotY;
    if (str == "RotZ")
        return rfdb::dbRobot::JType::RotZ;
    if (str == "TransX")
        return rfdb::dbRobot::JType::TransX;
    if (str == "TransY")
        return rfdb::dbRobot::JType::TransY;
    if (str == "TransZ")
        return rfdb::dbRobot::JType::TransZ;
    if (str == "Fix")
        return rfdb::dbRobot::JType::Fix;
    else
        return rfdb::dbRobot::JType::None;
}
} // namespace rfio
