#pragma once
#ifndef COMMAND_REMOVECONTEXT_H
#define COMMAND_REMOVECONTEXT_H

#include "cmd_base.h"

namespace rfgui
{
class CmdRemoveContext : public CmdBase
{
public:
    CmdRemoveContext();
    ~CmdRemoveContext() override = default;

private:
    void execute() override;
};
} // namespace rfgui
#endif
