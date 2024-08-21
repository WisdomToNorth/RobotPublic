#pragma once
#ifndef COMMAND_NEWCONTEXT_H
#define COMMAND_NEWCONTEXT_H

#include "cmd_base.h"

namespace rfgui
{
class CmdNewContext : public CmdBase
{
public:
    CmdNewContext();
    ~CmdNewContext();

private:
    void execute() override;
};
} // namespace rfgui
#endif
