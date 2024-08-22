
#pragma once
#ifndef CMD_MANAGER_H
#define CMD_MANAGER_H

#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include "rfgraphics/rfviewevent.h"
#include "rflog/rfLog.h"

#include "command/cmd_base.h"

namespace rfgui
{

class CmdMngr
{
public:
    static CmdMngr &getInstance()
    {
        static CmdMngr *instance = new CmdMngr();
        return *instance;
    }

    void getCmdRMenu();

    void processRMenuAction(const std::string &action_name);
    void addToCmdList(std::unique_ptr<CmdBase> cmd);
    bool executeLastCmd();

    void process(const rfapp::ViewerEvent &info);

    void doneCmd();
    void undo();

    std::string getCurCmdName() const;
    bool isCmdListEmpty() const;

private:
    CmdMngr() = default;
    ~CmdMngr() = default;
    std::vector<std::unique_ptr<CmdBase>> done_cmds_;
    std::stack<std::unique_ptr<CmdBase>> doing_cmds_;
};

} // namespace rfgui
#endif
