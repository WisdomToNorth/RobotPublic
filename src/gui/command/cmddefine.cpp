#include "cmddefine.h"

#include <cassert>
#include <iostream>

#include "rfbase/meta_enum.h"

namespace rfgui
{

std::string CmdUtil::getCmdName(ECmd _enum)
{
    return rfbase::MetaEnum::nameStr(_enum).c_str();
}

std::optional<ECmd> CmdUtil::getCmdEnum(const std::string &_cmd_name)
{
    std::string_view cmd_name = _cmd_name;
    return magic_enum::enum_cast<ECmd>(cmd_name);
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
} // namespace rfgui
