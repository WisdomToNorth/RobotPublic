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
    static std::string getIconPath(ECmd cmd_name);

private:
    static std::string getIconPath(const std::string &theme_file_name, ECmd cmd_name);
    static std::string getIconFileName(ECmd cmd_name);
};
} // namespace rfgui
#endif
