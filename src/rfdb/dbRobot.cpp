#include "dbRobot.h"

#include <vector>

namespace rfdb
{
std::string dbRobot::getJointName(unsigned int i)
{
    return "ARM_J" + std::to_string(i);
}

void dbRobot::convertDegToRad(const dbRobotStateDeg &robot_state_deg,
                              dbRobotStateRad &robot_state_rad)
{
    robot_state_rad.clear();
    for (auto &deg : robot_state_deg)
    {
        robot_state_rad.push_back(deg * M_PI / 180.0);
    }
}

void dbRobot::convertRadToDeg(const dbRobotStateRad &robot_state_rad,
                              dbRobotStateDeg &robot_state_deg)
{
    robot_state_deg.clear();
    for (const auto &rad : robot_state_rad)
    {
        robot_state_deg.push_back(rad * 180.0 / M_PI);
    }
}

dbRobot::dbRobot()
{
    property_ = new dbProperty();
}

dbRobot::~dbRobot()
{
    delete property_;
}

bool dbRobot::setJoints(const std::vector<JointParams> &arm_params)
{
    this->joint_params_ = arm_params;
    return true;
}

bool dbRobot::setRobotStateDeg(const dbRobotStateDeg &robot_state_deg)
{
    this->robot_state_deg_ = robot_state_deg;
    return true;
}

bool dbRobot::setRobotStateRad(const dbRobotStateRad &robot_state_rad)
{
    dbRobotStateDeg robot_state_deg;
    convertRadToDeg(robot_state_rad, robot_state_deg);
    return setRobotStateDeg(robot_state_deg);
}

bool dbRobot::setModel(const dbVariant &part)
{
    this->robot_part_ = part.toPart();
    return true;
}

const std::vector<dbRobot::JointParams> &dbRobot::getJoints() const
{
    return joint_params_;
}
void dbRobot::getRobotStateDeg(dbRobot::dbRobotStateDeg &res) const
{
    res = robot_state_deg_;
}

void dbRobot::getRobotStateRad(dbRobot::dbRobotStateRad &res) const
{
    convertDegToRad(robot_state_deg_, res);
}

dbPart *dbRobot::getModel() const
{
    return robot_part_;
}

void dbRobot::getShapes(std::unordered_map<std::string, Handle(AIS_ColoredShape)> &shps) const
{
    shps.clear();
    for (auto &[id, shp] : robot_shapes_)
    {
        shps.emplace(id, shp);
    }
}

dbProperty *dbRobot::getProperty() const
{
    return property_;
}
} // namespace rfdb
