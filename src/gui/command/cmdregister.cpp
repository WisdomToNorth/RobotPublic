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
void CmdRegister::getUserCmds(std::vector<std::pair<std::string, ECmd>> &cmds)
{
    /* File */
    cmds.emplace_back("File", ECmd::NewContext);
    cmds.emplace_back("File", ECmd::RemoveContext);
    cmds.emplace_back("File", ECmd::ChangeContext);
    cmds.emplace_back("File", ECmd::ImportFile);
    cmds.emplace_back("File", ECmd::ExportFile);
    cmds.emplace_back("File", ECmd::Setting);

    /* DB */
    cmds.emplace_back("Data", ECmd::ConfigRobot);
    cmds.emplace_back("Data", ECmd::DrawDemo);
    cmds.emplace_back("Data", ECmd::ShowInfo);
    cmds.emplace_back("Data", ECmd::Remove);
    cmds.emplace_back("Data", ECmd::Undo);
    cmds.emplace_back("Data", ECmd::Ruler);
    cmds.emplace_back("Data", ECmd::Move);

    /* Robot */
    cmds.emplace_back("Viewer", ECmd::RobotMove);
    cmds.emplace_back("Viewer", ECmd::RobotForward);

    /* Viewer, not add to toolbar */
    // cmds.emplace_back("Viewer", ECmd::PanViewer);
    // cmds.emplace_back("Viewer", ECmd::RotViewer);
    // cmds.emplace_back("Viewer", ECmd::ZoomViewer);
}

std::unique_ptr<CmdBase> CmdRegister::createCommand(ECmd cmd_enum)
{
    std::unique_ptr<CmdBase> cmd;
    switch (cmd_enum)
    {
    case ECmd::ConfigRobot: cmd = std::make_unique<CmdConfigRobot>(); break;
    case ECmd::ChangeContext: cmd = std::make_unique<CmdChangeContext>(); break;
    case ECmd::DrawDemo: cmd = std::make_unique<CmdAddDemoVariant>(); break;
    case ECmd::ExportFile: cmd = std::make_unique<CmdExportFile>(); break;
    case ECmd::ImportFile: cmd = std::make_unique<CmdImportFile>(); break;
    case ECmd::Move: cmd = std::make_unique<CmdMove>(); break;
    case ECmd::NewContext: cmd = std::make_unique<CmdNewContext>(); break;
    case ECmd::PanViewer: cmd = std::make_unique<CmdPanViewer>(); break;
    case ECmd::RemoveContext: cmd = std::make_unique<CmdRemoveContext>(); break;
    case ECmd::RobotForward: cmd = std::make_unique<CmdRobotForward>(); break;
    case ECmd::RobotMove: cmd = std::make_unique<CmdRobotMove>(); break;
    case ECmd::Ruler: cmd = std::make_unique<CmdRuler>(); break;
    case ECmd::Remove: cmd = std::make_unique<CmdRemove>(); break;
    case ECmd::RotViewer: cmd = std::make_unique<CmdRotViewer>(); break;
    case ECmd::Setting: cmd = std::make_unique<CmdSetting>(); break;
    case ECmd::ShowInfo: cmd = std::make_unique<CmdShowInfo>(); break;
    case ECmd::Undo: cmd = nullptr; break;
    case ECmd::ZoomViewer: cmd = std::make_unique<CmdZoomViewer>(); break;
    case ECmd::None: break;
    default:
    {
        assert(false);
        cmd = nullptr;
    }
    }
    if (cmd)
    {
        cmd->setCmdName(CmdUtil::getCmdName(cmd_enum));
    }
    return cmd;
};

} // namespace rfgui
