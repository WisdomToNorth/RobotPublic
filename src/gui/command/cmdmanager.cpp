#include "cmdmanager.h"

#include "rflog/rfLog.h"

#include "app_system.h"
#include "command/cmd_base.h"
#include "command/cmdregister.h"

namespace rfgui
{

void CmdMngr::getCmdRMenu()
{
    if (doing_cmds_.empty())
    {
        return;
    }
    doing_cmds_.top()->createRMenu();
}

void CmdMngr::processRMenuAction(const std::string &action_name)
{
    if (doing_cmds_.empty())
    {
        return;
    }
    doing_cmds_.top()->processRMAction(action_name);
}

void CmdMngr::addToCmdList(std::unique_ptr<CmdBase> cmd)
{
    doing_cmds_.push(std::move(cmd));
    fmt::print("Add command {} to list, cur cmd stk size: {}\n", doing_cmds_.top()->getCmdName(),
               doing_cmds_.size());
    if (doing_cmds_.size() > 2)
    {
        assert(false);
    }
}

bool CmdMngr::executeLastCmd()
{
    if (doing_cmds_.empty())
    {
        return false;
    }
    auto &top_cmd = doing_cmds_.top();
    auto cmd_type = top_cmd->getType();
    switch (cmd_type)
    {
    case CmdType::Appmode:
    case CmdType::Immediate:
    case CmdType::Interactive:
    case CmdType::Nestable:
    {
        top_cmd->execute();
        break;
    }
    case CmdType::TimeSession:
    {
        std::thread(
            [this, &top_cmd]() mutable
            {
                // top_cmd is not able to be called after executed because it is moved
                top_cmd->execute();
                rflog::debug("TimeSession command {} done\n", done_cmds_.back()->getCmdName());
            })
            .detach();
        break;
    }
    case CmdType::Unknown: break;
    }
    return true;
}

void CmdMngr::process(const rfapp::ViewerEvent &info)
{
    if (doing_cmds_.empty())
    {
        return;
    }
    doing_cmds_.top()->processVEvent(info);
}

void CmdMngr::doneCmd()
{
    if (doing_cmds_.empty())
    {
        return;
    }
    auto &top_cmd = doing_cmds_.top();
    fmt::print("Command {} done, cur cmd stk size: {}\n", top_cmd->getCmdName(),
               doing_cmds_.size() - 1);
    top_cmd->onDone();
    done_cmds_.push_back(std::move(top_cmd));
    doing_cmds_.pop();
}

void CmdMngr::undo()
{
    if (!done_cmds_.empty())
    {
        done_cmds_.back()->undo();
        done_cmds_.pop_back();
    }
}

bool CmdMngr::isCmdListEmpty() const
{
    return doing_cmds_.empty();
}

std::string CmdMngr::getCurCmdName() const
{
    if (doing_cmds_.empty())
    {
        return "Empty Command";
    }
    return doing_cmds_.top()->getCmdName();
}
} // namespace rfgui
