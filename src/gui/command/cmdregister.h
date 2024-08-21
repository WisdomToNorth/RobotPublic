
#pragma once
#ifndef CMD_REGISTER_H
#define CMD_REGISTER_H

#include <memory>
#include <string>
#include <unordered_map>

#include "command/cmd_base.h"

namespace rfgui
{

class CmdRegister
{
public:
    static std::unique_ptr<CmdBase> createCommand(const std::string &cmd_name);

    // cmd group name, cmd enum
    static void getUserCmds(std::vector<std::pair<std::string, cmd::ECmd>> &cmds);
};

} // namespace rfgui
#endif
