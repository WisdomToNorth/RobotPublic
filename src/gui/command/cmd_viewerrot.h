#pragma once
#ifndef COMMAND_ROT_VIEWER_H
#define COMMAND_ROT_VIEWER_H

#include "cmd_base.h"

namespace rfgui
{
class CmdRotViewer : public CmdBase
{
public:
    CmdRotViewer() : CmdBase(CmdType::Nestable){};
    ~CmdRotViewer() = default;
    void processVEvent(const rfapp::ViewerEvent &info) override;

private:
    void execute() override;
    int mouse_x_record_;
    int mouse_y_record_;
};
} // namespace rfgui
#endif
