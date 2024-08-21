#pragma once
#ifndef COMMAND_EXPORTFILE_H
#define COMMAND_EXPORTFILE_H

#include "cmd_base.h"
namespace rfgui
{
class CmdExportFile : public CmdBase
{
public:
    CmdExportFile() : CmdBase(CmdType::TimeSession){};
    ~CmdExportFile()override = default;

private:
    void execute() override;
};
} // namespace rfgui
#endif
