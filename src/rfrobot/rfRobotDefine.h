
#pragma once
#ifndef ROBOT_DATA_W_H
#define ROBOT_DATA_W_H

#include <functional>

#include "rfdb/dbRobot.h"
#include "rfdb/dbRobotDefine.h"

namespace rfrobot
{
using JType = rfdb::dbRobot::JType;

using kConverntion = rfdb::Converntion;
using Posture = rfdb::Posture;
using Quaternion = rfdb::Quaternion;

using JointParams = rfdb::dbRobot::JointParams;

using RSRad = rfdb::dbRobot::dbRobotStateRad;
using RSDeg = rfdb::dbRobot::dbRobotStateDeg;

using GetJNameFunc = std::function<std::string(unsigned int)>;
inline GetJNameFunc getJName = rfdb::dbRobot::getJointName;

}; // namespace rfrobot
#endif
