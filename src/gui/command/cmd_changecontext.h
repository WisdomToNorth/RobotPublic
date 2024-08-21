#pragma once
#ifndef COMMAND_SETCONTEXT_H
#define COMMAND_SETCONTEXT_H

#include "cmd_base.h"

namespace rfgui
{
class CmdChangeContext : public CmdBase
{
public:
    CmdChangeContext();
    ~CmdChangeContext() override;

private:
    void execute() override;
};
} // namespace rfgui
#endif
