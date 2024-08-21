#pragma once
#ifndef ROBOTHANDLE_H
#define ROBOTHANDLE_H

#include "rfRobotDefine.h"

namespace rfrobot
{
class RobotUtils
{
public:
    static bool isEquals(const RSDeg &robot_state1, const RSDeg &robot_state2, double eps = 1e-6);

    static bool isEquals(const Posture &loc_euler1, const Posture &loc_euler2, double eps = 1e-6);

    static gp_Dir jointTypeToDir(JType type);

    // pnts.txt
    // 2
    // [78, 2, 0]
    // [78, 78, 0]
    static bool loadPnts(const std::string &filename, std::vector<gp_Pnt> &pnts);

    static JType strToJointType(const std::string &str);

    // Euler Converntion: Rxyz
    static Quaternion eulerToQuaternion(const gp_Pnt &euler,
                                        kConverntion conv = kConverntion::Rxyz);
    static gp_Pnt quaternionToEuler(const Quaternion &quat, kConverntion conv = kConverntion::Rxyz);

    static Quaternion slerpQuaternion(const Quaternion &q1, const Quaternion &q2, double t);

    // max_angle_gap is degree
    static void slerpRobotPosture(const Posture &loc1, const Posture &loc2, double max_gap,
                                  double max_angle_gap, std::vector<Posture> &res);

    static void slerpVector(const std::vector<double> &v1, const std::vector<double> &v2,
                            size_t cnt, std::vector<std::vector<double>> &res);
    static double vectorDistance(const std::vector<double> &start_state,
                                 const std::vector<double> &goals_state);

    static gp_Trsf postureToTrsf(Posture loc_euler);

    static std::string postureToStr(Posture loc_euler);

    static std::string robotstateToStr(RSDeg state, size_t precision);

    static RSDeg jointAngleToRobotState(const std::vector<double> &degrees);
    static RSDeg jointAngleArcToRobotState(const std::vector<double> &radians);

}; // namespace RobotTools
}; // namespace rfrobot
#endif
