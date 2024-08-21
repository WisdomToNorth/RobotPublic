#pragma once
#ifndef CMD_ICON_H
#define CMD_ICON_H

#include <string>

#include "command/cmddefine.h"

namespace rfgui
{
class CmdIcon
{
public:
    static std::string getIconPath(cmd::ECmd cmd_name);
    static std::string getIconPath(const std::string &cmd_name);

private:
    static std::string getIconPath(const std::string &theme_file_name, cmd::ECmd cmd_name);
    static std::string getIconFileName(cmd::ECmd cmd_name);
};
} // namespace rfgui
#endif
