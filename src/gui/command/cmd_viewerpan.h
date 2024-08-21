#pragma once
#ifndef COMMAND_PAN_VIEWER_H
#define COMMAND_PAN_VIEWER_H

#include "cmd_base.h"
namespace rfgui
{
class CmdPanViewer : public CmdBase
{
public:
    CmdPanViewer();
    ~CmdPanViewer() override = default;
    void processVEvent(const rfapp::ViewerEvent &info) override;

private:
    void execute() override;
    int mouse_x_record_;
    int mouse_y_record_;
};
} // namespace rfgui
#endif
