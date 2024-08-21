#pragma once
#ifndef COMMAND_IMPORTFILE_H
#define COMMAND_IMPORTFILE_H

#include <string>

#include "cmd_base.h"

namespace rfgui
{
class CmdImportFile : public CmdBase
{
    enum class LoadingState
    {
        Loading = 1,
        LoadedButEmpty = -1,
        Loaded = 0
    };

public:
    CmdImportFile() : CmdBase(CmdType::Interactive){};
    ~CmdImportFile() = default;

private:
    void execute() override;
    void openTheFile(const std::string &filename);
};
} // namespace rfgui
#endif // !_COMMAND_IMPORTFILE_H_
