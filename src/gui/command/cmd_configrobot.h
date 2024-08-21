#pragma once
#ifndef COMMAND_CONFIGROBOT_H
#define COMMAND_CONFIGROBOT_H

#include "cmd_base.h"
namespace rfgui
{
class CmdConfigRobot : public CmdBase
{
public:
    CmdConfigRobot() : CmdBase(CmdType::TimeSession){};
    ~CmdConfigRobot() = default;

private:
    void execute() override;
};
} // namespace rfgui
#endif
