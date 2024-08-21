#pragma once
#ifndef DBROBOT_H
#define DBROBOT_H

#include <string>
#include <unordered_map>

#include <AIS_ColoredShape.hxx>
#include <gp_Pnt.hxx>

#include "dbPart.h"
#include "dbProperty.h"
#include "dbRobotDefine.h"
#include "dbVariant.h"

namespace rfdb
{
class dbRobot
{
public:
    enum class JType
    {
        RotX,
        RotY,
        RotZ,
        TransX,
        TransY,
        TransZ,
        Fix,
        None
    };

    struct JointParams
    {
        std::vector<std::string> models_name_list_;
        gp_Pnt arm_rot_pnt_;        // point on viewmodel
        gp_Pnt dh_info_;            // DH parameter
        double min_v_degree_ = 0.0; // Degree system
        double max_v_degree_ = 0.0; // Degree system
        JType type_ = JType::None;
    };

    using dbRobotStateDeg = std::vector<double>;
    using dbRobotStateRad = std::vector<double>;
    static std::string getJointName(unsigned int i);
    static void convertDegToRad(const dbRobotStateDeg &robot_state_deg,
                                dbRobotStateRad &robot_state_rad);
    static void convertRadToDeg(const dbRobotStateRad &robot_state_rad,
                                dbRobotStateDeg &robot_state_deg);

public:
    dbRobot();
    ~dbRobot();

    bool setJoints(const std::vector<JointParams> &arm_params);
    bool setRobotStateDeg(const dbRobotStateDeg &robot_state_deg);
    bool setRobotStateRad(const dbRobotStateRad &robot_state_rad);
    bool setModel(const dbVariant &part);

    const std::vector<JointParams> &getJoints() const;
    void getRobotStateDeg(dbRobot::dbRobotStateDeg &res) const;
    void getRobotStateRad(dbRobot::dbRobotStateRad &res) const;
    dbPart *getModel() const;
    void getShapes(std::unordered_map<std::string, Handle(AIS_ColoredShape)> &shps) const;
    dbProperty *getProperty() const;

private:
    dbProperty *property_;
    std::vector<JointParams> joint_params_;
    dbRobotStateDeg robot_state_deg_;
    dbPart *robot_part_ = nullptr;
    std::unordered_map<std::string, Handle(AIS_ColoredShape)> robot_shapes_;
};
} // namespace rfdb
#endif
