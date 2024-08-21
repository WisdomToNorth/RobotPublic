#include "cmd_viewerzoom.h"

#include "rfgraphics/rfviewevent.h"

#include "app_system.h"

namespace rfgui
{
CmdZoomViewer::CmdZoomViewer()
    : CmdBase(CmdType::Nestable)
{
}
void CmdZoomViewer::execute()
{
}

void CmdZoomViewer::processVEvent(const rfapp::ViewerEvent &info)
{
    GApp::instance().getCurrentViewer()->zoomAtPoint(info.mouse_pos_x, info.mouse_pos_y,
                                                     info.wheel_delta_x, info.wheel_delta_y);
    GApp::instance().getCurrentViewer()->updateViewer(ViewerCore::KUpdate::Update);
    this->done();
}
} // namespace rfgui
