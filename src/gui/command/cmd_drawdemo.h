#pragma once
#ifndef COMMAND_DRAWDEMO_H
#define COMMAND_DRAWDEMO_H

#include "cmd_base.h"
namespace rfgui
{
class CmdAddDemoVariant : public CmdBase
{
public:
    enum class DemoType
    {
        None = 0,
        Bottle,
        CubePlusQuarterBall,
        Ball,
        DumbBell,
        Abassica
    };

public:
    CmdAddDemoVariant() : CmdBase(CmdType::Immediate){};
    ~CmdAddDemoVariant() override{};
    void generate(DemoType type);

private:
    void execute() override;
};
} // namespace rfgui
#endif
