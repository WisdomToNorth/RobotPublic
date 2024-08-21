#include "rfRobotUtils.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include <Eigen/Eigen>
#include <gp_Ax1.hxx>
#include <gp_Quaternion.hxx>
#include <ompl/base/spaces/SE3StateSpace.h>

#include "rfbase/datautils.h"
#include "rfgeo3d/geodataio.h"
#include "rfgeo3d/geoutils.h"

namespace rfrobot
{
using namespace rfgeo;

bool RobotUtils::isEquals(const RSDeg &robot_state1, const RSDeg &robot_state2, double eps)
{
    if (robot_state1.size() != robot_state2.size())
    {
        return false;
    }
    for (size_t i = 0; i < robot_state1.size(); i++)
    {
        if (fabs(robot_state1[i] - robot_state2[i]) > eps)
        {
            return false;
        }
    }
    return true;
}

bool RobotUtils::isEquals(const Posture &loc_euler1, const Posture &loc_euler2, double eps)
{
    if (!loc_euler1.location_.IsEqual(loc_euler2.location_, eps))
        return false;
    if (!loc_euler1.euler_.IsEqual(loc_euler2.euler_, eps))
        return false;
    return true;
}

gp_Dir RobotUtils::jointTypeToDir(JType type)
{
    if (type == JType::RotZ)
    {
        return gp_Dir(0, 0, 1);
    }
    else if (type == JType::RotY)
    {
        return gp_Dir(0, 1, 0);
    }
    else if (type == JType::RotX)
    {
        return gp_Dir(1, 0, 0);
    }
    else
    {
        std::cerr << "ERROR: joint type not supported\n";
        return gp_Dir();
    }
}

JType RobotUtils::strToJointType(const std::string &str)
{
    static const std::unordered_map<std::string, JType> str2type = {{"RotX", JType::RotX},
                                                                    {"RotY", JType::RotY},
                                                                    {"RotZ", JType::RotZ},
                                                                    {"TransX", JType::TransX},
                                                                    {"TransY", JType::TransY},
                                                                    {"TransZ", JType::TransZ}

    };

    auto it = str2type.find(str);
    return it != str2type.end() ? it->second : JType::None;
}

// pnts.txt
// 2
// [78, 2, 0]
// [78, 78, 0]

bool RobotUtils::loadPnts(const std::string &filename, std::vector<gp_Pnt> &pnts)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "ERROR: " << filename << " import failed" << '\n';
        return false;
    }

    // First line, number of points
    size_t pnt_cnt;
    file >> pnt_cnt;

    std::string line;
    std::getline(file, line); // To consume the rest of the first line

    while (std::getline(file, line))
    {
        pnts.emplace_back(GeoDataIO::strToPt(line));
    }

    file.close();
    if (pnts.size() != pnt_cnt)
    {
        std::cout << "WARNING: " << filename
                  << ": num of loaded point is inconsistent with record, please check.\n";
        return false;
    }
    return true;
}

Quaternion RobotUtils::eulerToQuaternion(const gp_Pnt &euler, kConverntion conv)
{
    double roll = euler.X() * M_PI / 180.0;
    double pitch = euler.Y() * M_PI / 180.0;
    double yaw = euler.Z() * M_PI / 180.0;

    Eigen::Quaterniond quat;

    switch (conv)
    {
    case kConverntion::Rzyx:
        quat = Eigen::AngleAxisd(yaw, Eigen::Vector3d::UnitZ()) *
               Eigen::AngleAxisd(pitch, Eigen::Vector3d::UnitY()) *
               Eigen::AngleAxisd(roll, Eigen::Vector3d::UnitX());
        break;
    case kConverntion::Rxyz:
        quat = Eigen::AngleAxisd(roll, Eigen::Vector3d::UnitX()) *
               Eigen::AngleAxisd(pitch, Eigen::Vector3d::UnitY()) *
               Eigen::AngleAxisd(yaw, Eigen::Vector3d::UnitZ());
        break;
    case kConverntion::Rzxz:
        quat = Eigen::AngleAxisd(yaw, Eigen::Vector3d::UnitZ()) *
               Eigen::AngleAxisd(roll, Eigen::Vector3d::UnitX()) *
               Eigen::AngleAxisd(yaw, Eigen::Vector3d::UnitZ());
        break;
    }

    Quaternion result;
    result.w_ = quat.w();
    result.x_ = quat.x();
    result.y_ = quat.y();
    result.z_ = quat.z();

    return result;
}

gp_Pnt RobotUtils::quaternionToEuler(const Quaternion &quat, kConverntion conv)
{
    Eigen::Quaterniond eigenQuat(quat.w_, quat.x_, quat.y_, quat.z_);
    Eigen::Vector3d euler;

    switch (conv)
    {
    case kConverntion::Rzyx: euler = eigenQuat.toRotationMatrix().eulerAngles(2, 1, 0); break;
    case kConverntion::Rxyz: euler = eigenQuat.toRotationMatrix().eulerAngles(0, 1, 2); break;
    case kConverntion::Rzxz:
    {
        // Eigen doesn't support Rzxz directly; need to handle this manually
        euler = eigenQuat.toRotationMatrix().eulerAngles(2, 0, 2);
        break;
    }
    }

    return gp_Pnt(euler[0] * 180.0 / M_PI, euler[1] * 180.0 / M_PI, euler[2] * 180.0 / M_PI);
}

void RobotUtils::slerpRobotPosture(const Posture &loc1, const Posture &loc2, double max_gap,
                                   double max_angle_gap, std::vector<Posture> &output)
{
    gp_Pnt pt1 = loc1.location_;
    gp_Pnt pt2 = loc2.location_;
    gp_Pnt delta_loc = GeoUtils::minusPt(pt2, pt1);

    double temp;
    temp = std::max(std::abs(delta_loc.Y() / max_gap), std::abs(delta_loc.X() / max_gap));
    temp = std::max(temp, std::abs(delta_loc.Z() / max_gap));

    int n = static_cast<int>(std::ceil(temp));

    Quaternion loc1_quat = eulerToQuaternion(loc1.euler_);
    Quaternion loc2_quat = eulerToQuaternion(loc2.euler_);

    Eigen::Quaterniond q1(loc1_quat.w_, loc1_quat.x_, loc1_quat.y_, loc1_quat.z_);
    Eigen::Quaterniond q2(loc2_quat.w_, loc2_quat.x_, loc2_quat.y_, loc2_quat.z_);
    double angle = q1.angularDistance(q2) * 180.0 / M_PI;

    int m = static_cast<int>(std::ceil(angle / max_angle_gap));

    int num_steps = std::max(n, m);

    output.clear();
    double x_s = pt1.X();
    double y_s = pt1.Y();
    double z_s = pt1.Z();
    double dx = delta_loc.X() / num_steps;
    double dy = delta_loc.Y() / num_steps;
    double dz = delta_loc.Z() / num_steps;

    for (int i = 0; i <= num_steps; ++i)
    {
        Quaternion qres = slerpQuaternion(loc1_quat, loc2_quat, static_cast<double>(i) / num_steps);
        gp_Pnt euler_i = quaternionToEuler(qres);
        output.emplace_back(Posture(gp_Pnt(x_s + dx * i, y_s + dy * i, z_s + dz * i), euler_i));
    }
}

Quaternion RobotUtils::slerpQuaternion(const Quaternion &q1, const Quaternion &q2, double t)
{
    Eigen::Quaterniond eigen_q1(q1.w_, q1.x_, q1.y_, q1.z_);
    Eigen::Quaterniond eigen_q2(q2.w_, q2.x_, q2.y_, q2.z_);

    Eigen::Quaterniond eigen_result = eigen_q1.slerp(t, eigen_q2);

    Quaternion result;
    result.w_ = eigen_result.w();
    result.x_ = eigen_result.x();
    result.y_ = eigen_result.y();
    result.z_ = eigen_result.z();

    return result;
}

// Slerp each number in vector, etc.
// input  [1, 2, 3] and [5, 7, 10], and cnt = 5
// output [[1, 2, 3], [2, 3.25, 4.75], [3, 4.5, 6.5], [4, 5.75, 8.25], [5, 7, 10]]
void RobotUtils::slerpVector(const std::vector<double> &v1, const std::vector<double> &v2,
                             size_t cnt, std::vector<std::vector<double>> &res)
{
    // Ensure input vectors are the same size
    if (v1.size() != v2.size())
    {
        throw std::invalid_argument("Input vectors must have the same size");
    }

    // Clear the result vector
    res.clear();

    // Resize the result vector to hold the interpolated vectors
    res.resize(cnt);

    // Calculate the step size for interpolation
    double step = 1.0 / static_cast<double>(cnt - 1);

    // Iterate over each element in the vectors
    for (size_t i = 0; i < cnt; ++i)
    {
        // Calculate the interpolation factor
        double t = static_cast<double>(i) * step;

        // Create a vector to hold the interpolated values
        std::vector<double> interpolated(v1.size());

        // Perform the linear interpolation for each element
        for (size_t j = 0; j < v1.size(); ++j)
        {
            interpolated[j] = (1 - t) * v1[j] + t * v2[j];
        }

        // Add the interpolated vector to the result
        res[i] = interpolated;
    }
}

gp_Trsf RobotUtils::postureToTrsf(Posture loc_euler)
{
    gp_Trsf trsfx, trsfy, trsfz, trsf;
    trsfx.SetRotation(gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(1, 0, 0)), loc_euler.euler_.X());
    trsfy.SetRotation(gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(0, 1, 0)), loc_euler.euler_.Y());
    trsfz.SetRotation(gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1)), loc_euler.euler_.Z());
    trsf.SetTranslation(gp_Pnt(0, 0, 0), loc_euler.location_);
    return trsf * trsfx * trsfy * trsfz;
}

std::string RobotUtils::postureToStr(Posture loc_euler)
{
    std::string str = GeoDataIO::occTypeToStr(loc_euler.location_) + "," +
                      GeoDataIO::occTypeToStr(loc_euler.euler_);
    return str;
}

std::string RobotUtils::robotstateToStr(RSDeg state, size_t precision)
{
    std::ostringstream stream;
    stream.precision(precision);
    size_t size = state.size();
    stream << std::fixed << '[';
    for (size_t i = 0; i < size; i++)
    {
        stream << state[i];
        if (i != size - 1)
        {
            stream << ", ";
        }
    }
    stream << ']';
    return stream.str();
}

RSDeg RobotUtils::jointAngleToRobotState(const std::vector<double> &angles)
{
    RSDeg res;
    res.resize(angles.size());

    for (size_t i = 0; i < angles.size(); i++)
    {
        res[i] = angles[i] / 180. * M_PI;
    }

    return res;
}

RSDeg RobotUtils::jointAngleArcToRobotState(const std::vector<double> &angles)
{
    RSDeg res;
    res.assign(angles.begin(), angles.end());
    return res;
}

double RobotUtils::vectorDistance(const std::vector<double> &start_state,
                                  const std::vector<double> &goal_state)
{
    // Ensure the input vectors are the same size
    if (start_state.size() != goal_state.size())
    {
        throw std::invalid_argument("Input vectors must have the same size");
    }

    double dist = 0;
    // Compute the sum of squared differences
    for (size_t i = 0; i < start_state.size(); ++i)
    {
        dist += std::pow(goal_state[i] - start_state[i], 2);
    }

    // Return the Euclidean distance
    return std::sqrt(dist);
}
} // namespace rfrobot
