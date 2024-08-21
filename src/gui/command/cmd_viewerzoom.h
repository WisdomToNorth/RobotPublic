#pragma once
#ifndef COMMAND_ZOOM_VIERER_H
#define COMMAND_ZOOM_VIERER_H

#include "cmd_base.h"
namespace rfgui
{
class CmdZoomViewer : public CmdBase
{
public:
    CmdZoomViewer();
    ~CmdZoomViewer() override = default;
    void processVEvent(const rfapp::ViewerEvent &info) override;

private:
    void execute() override;
};
} // namespace rfgui
#endif
