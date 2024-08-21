
#pragma once
#ifndef CMD_DEFINE_H
#define CMD_DEFINE_H

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

namespace cmd
{
static const std::string kConfigRobot = "ConfigRobot";
static const std::string kDrawDemo = "DrawDemo";
static const std::string kExportFile = "ExportFile";
static const std::string kImportFile = "ImportFile";
static const std::string kMove = "Move";
static const std::string kNewContext = "NewContext";
static const std::string kPanViewer = "PanViewer";
static const std::string kRemoveContext = "RemoveContext";
static const std::string kRobotForward = "RobotForward";
static const std::string kRobotMove = "RobotMove";
static const std::string kRuler = "Ruler";
static const std::string kRemove = "Remove";
static const std::string kRotViewer = "RotViewer";
static const std::string kChangeContext = "ChangeContext";
static const std::string kSetting = "Setting";
static const std::string kShowInfo = "ShowInfo";
static const std::string kUndo = "Undo";
static const std::string kZoomViewer = "ZoomViewer";

enum class ECmd
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
    static ECmd getCmdEnum(const std::string &cmd_name);
    static std::string getCmdShortCut(ECmd cmd);
    static bool isShortcutUsed(const std::string &shortcut);

private:
    // confrim the shortcut which already defined is no duplicate
    static bool validateShortcut();

private:
    static std::unordered_map<ECmd, std::string> cmd_shortcut_map_;
};

} // namespace cmd
} // namespace rfgui
#endif
