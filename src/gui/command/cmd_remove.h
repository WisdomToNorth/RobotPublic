#pragma once
#ifndef COMMAND_REMOVE_H
#define COMMAND_REMOVE_H

#include "cmd_base.h"
#include "viewnode.h"

namespace rfgui
{
class CmdRemove : public CmdBase
{
public:
    CmdRemove() : CmdBase(CmdType::Immediate){};
    ~CmdRemove() = default;

private:
    void execute() override;

    bool removeNode(ViewNode *node);
};
} // namespace rfgui
#endif
