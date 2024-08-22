
#pragma once
#ifndef CMD_DEFINE_H
#define CMD_DEFINE_H

#include <optional>
#include <string>
#include <unordered_map>

namespace rfgui
{

enum class CmdType : char
{
    Unknown = 0,
    Interactive, // forbid in command mode
    Appmode,     // return a app mode
    Immediate,   // execute immediately
    TimeSession, // execute in a time session
    Nestable     // execute in a nested command
};

enum class ECmd : unsigned char
{
    None,
    ConfigRobot,
    DrawDemo,
    ExportFile,
    ImportFile,
    Move,
    NewContext,
    PanViewer,
    RemoveContext,
    RobotForward,
    RobotMove,
    Ruler,
    Remove,
    RotViewer,
    ChangeContext,
    Setting,
    ShowInfo,
    Undo,
    ZoomViewer
};

struct Parameter
{
    std::string string_value = "";
    double double_value = 0.0;
    int int_value = 0;
};

struct CmdUtil
{
    static std::string getCmdName(ECmd cmd);
    static std::optional<ECmd> getCmdEnum(const std::string &cmd_name);
    static std::string getCmdShortCut(ECmd cmd);
    static bool isShortcutUsed(const std::string &shortcut);

private:
    // confrim the shortcut which already defined is no duplicate
    static bool validateShortcut();

private:
    static std::unordered_map<ECmd, std::string> cmd_shortcut_map_;
};

} // namespace rfgui
#endif
