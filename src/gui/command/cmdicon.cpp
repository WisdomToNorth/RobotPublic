#include "cmdicon.h"

#include <unordered_set>

#include "rfbase/datautils.h"

#include "app_system.h"

namespace rfgui
{
std::string CmdIcon::getIconFileName(ECmd cmd_name)
{
    switch (cmd_name)
    {
    case ECmd::ConfigRobot: return "config.png";
    case ECmd::DrawDemo: return "draw.png";
    case ECmd::ExportFile: return "export.png";
    case ECmd::ImportFile: return "import.png";
    case ECmd::Move: return "location.png";
    case ECmd::NewContext: return "context.png";
    case ECmd::PanViewer: return "pan.png";
    case ECmd::RobotForward: return "tool.png";
    case ECmd::RobotMove: return "path.png";
    case ECmd::RemoveContext: return "";
    case ECmd::Ruler: return "ruler.png";
    case ECmd::Remove: return "delete.png";
    case ECmd::RotViewer: return "rotate.png";
    case ECmd::ChangeContext: return "";
    case ECmd::Setting: return "setting.png";
    case ECmd::ShowInfo: return "info.png";
    case ECmd::Undo: return "undo.png";
    case ECmd::ZoomViewer: return "zoom.png";
    case ECmd::None: return "";
    }
    return "";
}

std::string CmdIcon::getIconPath(const std::string &theme_file_name, ECmd cmd_name)
{
    std::unordered_set<std::string> dark_set = {"darkmode.qss", "darkmode.qss", "steam.qss",
                                                "steam.qss",    "xsdark.qss",   "xsdark.qss"};
    bool use_dark = dark_set.find(theme_file_name) != dark_set.end();
    std::string icon_path = use_dark ? ":/REMainWindow/icons/dark/" : ":/REMainWindow/icons/light/";
    return icon_path + getIconFileName(cmd_name);
}

std::string CmdIcon::getIconPath(ECmd cmd_name)
{
    auto setting = GApp::instance().getSetting();
    auto theme_name = setting ? setting->getThemeFileName() : "";
    return getIconPath(theme_name, cmd_name);
}
}; // namespace rfgui
