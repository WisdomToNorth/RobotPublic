#include "cmdicon.h"

#include <unordered_set>

#include "rfbase/datautils.h"

#include "app_system.h"

namespace rfgui
{
std::string CmdIcon::getIconFileName(cmd::ECmd cmd_name)
{
    switch (cmd_name)
    {
    case cmd::ECmd::ConfigRobot: return "config.png";
    case cmd::ECmd::DrawDemo: return "draw.png";
    case cmd::ECmd::ExportFile: return "export.png";
    case cmd::ECmd::ImportFile: return "import.png";
    case cmd::ECmd::Move: return "location.png";
    case cmd::ECmd::NewContext: return "context.png";
    case cmd::ECmd::PanViewer: return "pan.png";
    case cmd::ECmd::RobotForward: return "tool.png";
    case cmd::ECmd::RobotMove: return "path.png";
    case cmd::ECmd::RemoveContext: return "";
    case cmd::ECmd::Ruler: return "ruler.png";
    case cmd::ECmd::Remove: return "delete.png";
    case cmd::ECmd::RotViewer: return "rotate.png";
    case cmd::ECmd::ChangeContext: return "";
    case cmd::ECmd::Setting: return "setting.png";
    case cmd::ECmd::ShowInfo: return "info.png";
    case cmd::ECmd::Undo: return "undo.png";
    case cmd::ECmd::ZoomViewer: return "zoom.png";
    case cmd::ECmd::None: return "";
    }
    return "";
}

std::string CmdIcon::getIconPath(const std::string &theme_file_name, cmd::ECmd cmd_name)
{
    std::unordered_set<std::string> dark_set = {"darkmode.qss", "darkmode.qss", "steam.qss",
                                                "steam.qss",    "xsdark.qss",   "xsdark.qss"};
    bool use_dark = dark_set.find(theme_file_name) != dark_set.end();
    std::string icon_path = use_dark ? ":/REMainWindow/icons/dark/" : ":/REMainWindow/icons/light/";
    return icon_path + getIconFileName(cmd_name);
}

std::string CmdIcon::getIconPath(const std::string &cmd_name)
{
    return getIconPath(cmd::CmdUtil::getCmdEnum(cmd_name));
}

std::string CmdIcon::getIconPath(cmd::ECmd cmd_name)
{
    auto setting = GApp::instance().getSetting();
    auto theme_name = setting ? setting->getThemeFileName() : "";
    return getIconPath(theme_name, cmd_name);
}
}; // namespace rfgui
