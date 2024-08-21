#include "cmdregister.h"

#include "app_system.h"
#include "cmdmanager.h"
#include "command/cmd_base.h"
#include "command/cmd_changecontext.h"
#include "command/cmd_configrobot.h"
#include "command/cmd_drawdemo.h"
#include "command/cmd_exportfile.h"
#include "command/cmd_importfile.h"
#include "command/cmd_move.h"
#include "command/cmd_newcontext.h"
#include "command/cmd_remove.h"
#include "command/cmd_removecontext.h"
#include "command/cmd_robotfor.h"
#include "command/cmd_robotmove.h"
#include "command/cmd_ruler.h"
#include "command/cmd_setting.h"
#include "command/cmd_showinfo.h"
#include "command/cmd_viewerpan.h"
#include "command/cmd_viewerrot.h"
#include "command/cmd_viewerzoom.h"
#include "command/cmddefine.h"

namespace rfgui
{
// return: cmd group name, cmd enum
void CmdRegister::getUserCmds(std::vector<std::pair<std::string, cmd::ECmd>> &cmds)
{
    /* File */
    cmds.emplace_back("File", cmd::ECmd::NewContext);
    // cmds.emplace_back("File", cmd::ECmd::RemoveContext);
    // cmds.emplace_back("File", cmd::ECmd::ChangeContext);
    cmds.emplace_back("File", cmd::ECmd::ImportFile);
    cmds.emplace_back("File", cmd::ECmd::ExportFile);
    cmds.emplace_back("File", cmd::ECmd::Setting);

    /* DB */
    cmds.emplace_back("Data", cmd::ECmd::ConfigRobot);
    cmds.emplace_back("Data", cmd::ECmd::DrawDemo);
    cmds.emplace_back("Data", cmd::ECmd::ShowInfo);
    cmds.emplace_back("Data", cmd::ECmd::Remove);
    cmds.emplace_back("Data", cmd::ECmd::Undo);
    cmds.emplace_back("Data", cmd::ECmd::Ruler);
    cmds.emplace_back("Data", cmd::ECmd::Move);

    /* Robot */
    cmds.emplace_back("Viewer", cmd::ECmd::RobotMove);
    cmds.emplace_back("Viewer", cmd::ECmd::RobotForward);

    /* Viewer, not add to toolbar */
    // cmds.emplace_back("Viewer", cmd::ECmd::PanViewer);
    // cmds.emplace_back("Viewer", cmd::ECmd::RotViewer);
    // cmds.emplace_back("Viewer", cmd::ECmd::ZoomViewer);
}

std::unique_ptr<CmdBase> CmdRegister::createCommand(const std::string &cmd_name)
{
    auto cmd_enum = cmd::CmdUtil::getCmdEnum(cmd_name);
    std::unique_ptr<CmdBase> cmd;
    switch (cmd_enum)
    {
    case cmd::ECmd::ConfigRobot: cmd = std::make_unique<CmdConfigRobot>(); break;
    case cmd::ECmd::ChangeContext: cmd = std::make_unique<CmdChangeContext>(); break;
    case cmd::ECmd::DrawDemo: cmd = std::make_unique<CmdAddDemoVariant>(); break;
    case cmd::ECmd::ExportFile: cmd = std::make_unique<CmdExportFile>(); break;
    case cmd::ECmd::ImportFile: cmd = std::make_unique<CmdImportFile>(); break;
    case cmd::ECmd::Move: cmd = std::make_unique<CmdMove>(); break;
    case cmd::ECmd::NewContext: cmd = std::make_unique<CmdNewContext>(); break;
    case cmd::ECmd::PanViewer: cmd = std::make_unique<CmdPanViewer>(); break;
    case cmd::ECmd::RemoveContext: cmd = std::make_unique<CmdRemoveContext>(); break;
    case cmd::ECmd::RobotForward: cmd = std::make_unique<CmdRobotForward>(); break;
    case cmd::ECmd::RobotMove: cmd = std::make_unique<CmdRobotMove>(); break;
    case cmd::ECmd::Ruler: cmd = std::make_unique<CmdRuler>(); break;
    case cmd::ECmd::Remove: cmd = std::make_unique<CmdRemove>(); break;
    case cmd::ECmd::RotViewer: cmd = std::make_unique<CmdRotViewer>(); break;
    case cmd::ECmd::Setting: cmd = std::make_unique<CmdSetting>(); break;
    case cmd::ECmd::ShowInfo: cmd = std::make_unique<CmdShowInfo>(); break;
    case cmd::ECmd::Undo: cmd = nullptr; break;
    case cmd::ECmd::ZoomViewer: cmd = std::make_unique<CmdZoomViewer>(); break;
    case cmd::ECmd::None: break;
    default:
    {
        assert(false);
        cmd = nullptr;
    }
    }
    if (cmd)
    {
        cmd->setCmdName(cmd_name);
    }
    return cmd;
};

} // namespace rfgui
