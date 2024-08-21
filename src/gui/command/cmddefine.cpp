#include "cmddefine.h"

#include <cassert>
#include <iostream>

namespace rfgui
{
namespace cmd
{
std::string CmdUtil::getCmdName(ECmd _)
{
    switch (_)
    {
    case ECmd::None: return "";
    case ECmd::ConfigRobot: return kConfigRobot;
    case ECmd::ChangeContext: return kChangeContext;
    case ECmd::DrawDemo: return kDrawDemo;
    case ECmd::ExportFile: return kExportFile;
    case ECmd::ImportFile: return kImportFile;
    case ECmd::Move: return kMove;
    case ECmd::NewContext: return kNewContext;
    case ECmd::PanViewer: return kPanViewer;
    case ECmd::RemoveContext: return kRemoveContext;
    case ECmd::RobotForward: return kRobotForward;
    case ECmd::RobotMove: return kRobotMove;
    case ECmd::Ruler: return kRuler;
    case ECmd::Remove: return kRemove;
    case ECmd::RotViewer: return kRotViewer;
    case ECmd::Setting: return kSetting;
    case ECmd::ShowInfo: return kShowInfo;
    case ECmd::Undo: return kUndo;
    case ECmd::ZoomViewer: return kZoomViewer;
    }
    return "";
}

ECmd CmdUtil::getCmdEnum(const std::string &_cmd_name)
{
    if (_cmd_name == kConfigRobot)
        return ECmd::ConfigRobot;
    if (_cmd_name == kChangeContext)
        return ECmd::ChangeContext;
    if (_cmd_name == kDrawDemo)
        return ECmd::DrawDemo;
    if (_cmd_name == kExportFile)
        return ECmd::ExportFile;
    if (_cmd_name == kImportFile)
        return ECmd::ImportFile;
    if (_cmd_name == kMove)
        return ECmd::Move;
    if (_cmd_name == kNewContext)
        return ECmd::NewContext;
    if (_cmd_name == kPanViewer)
        return ECmd::PanViewer;
    if (_cmd_name == kRemoveContext)
        return ECmd::RemoveContext;
    if (_cmd_name == kRobotForward)
        return ECmd::RobotForward;
    if (_cmd_name == kRobotMove)
        return ECmd::RobotMove;
    if (_cmd_name == kRuler)
        return ECmd::Ruler;
    if (_cmd_name == kRemove)
        return ECmd::Remove;
    if (_cmd_name == kRotViewer)
        return ECmd::RotViewer;
    if (_cmd_name == kSetting)
        return ECmd::Setting;
    if (_cmd_name == kShowInfo)
        return ECmd::ShowInfo;
    if (_cmd_name == kUndo)
        return ECmd::Undo;
    if (_cmd_name == kZoomViewer)
        return ECmd::ZoomViewer;

    return ECmd::None;
}

std::unordered_map<ECmd, std::string> CmdUtil::cmd_shortcut_map_{
    {ECmd::ConfigRobot, "Ctrl+L"},
    {ECmd::ChangeContext, "Ctrl+K"},
    {ECmd::DrawDemo, "Ctrl+D"},
    {ECmd::ExportFile, "Ctrl+E"},
    {ECmd::ImportFile, "Ctrl+I"},
    {ECmd::Move, "Ctrl+M"},
    {ECmd::NewContext, "Ctrl+N"},
    {ECmd::PanViewer, "Ctrl+P"},
    {ECmd::RemoveContext, "Ctrl+Shift+K"},
    {ECmd::RobotForward, "Ctrl+F"},
    {ECmd::RobotMove, "Ctrl+Shift+M"},
    {ECmd::Ruler, "Ctrl+R"},
    {ECmd::Remove, "Ctrl+Delete"},
    {ECmd::RotViewer, "Ctrl+Shift+R"},
    {ECmd::Setting, "Ctrl+Shift+S"},
    {ECmd::ShowInfo, "Alt+I"},
    {ECmd::Undo, "Ctrl+Z"},
    {ECmd::ZoomViewer, "Ctrl+="},
    {ECmd::None, ""}};

bool CmdUtil::validateShortcut()
{
    std::unordered_map<std::string, int> shortcut_map;
    for (const auto &[_, sc] : cmd_shortcut_map_)
    {
        if (sc.empty())
        {
            continue;
        }
        if (shortcut_map.find(sc) != shortcut_map.end())
        {
            std::cout << "shortcut: " << sc << " is duplicate" << '\n';
            return false;
        }
        shortcut_map[sc] = 1;
    }
    return true;
}

std::string CmdUtil::getCmdShortCut(ECmd cmd)
{
    const std::unordered_map<ECmd, std::string> cmd_shortcut_map = cmd_shortcut_map_;
    assert(validateShortcut());
    auto it = cmd_shortcut_map.find(cmd);
    if (it != cmd_shortcut_map.end())
    {
        return it->second;
    }
    return "";
}

bool CmdUtil::isShortcutUsed(const std::string &shortcut)
{
    for (const auto &[_, sc] : cmd_shortcut_map_)
    {
        if (sc == shortcut)
        {
            return true;
        }
    }
    return false;
}
} // namespace cmd
} // namespace rfgui
