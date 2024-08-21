#include "cmd_viewerpan.h"

#include "rfgraphics/rfviewevent.h"

#include "app_system.h"
#include "viewer_utils.h"

namespace rfgui
{
CmdPanViewer::CmdPanViewer()
    : CmdBase(CmdType::Nestable)
{
}

void CmdPanViewer::execute()
{
}

void CmdPanViewer::processVEvent(const rfapp::ViewerEvent &info)
{
    if (info.type_flags == rfapp::VType::MButtonDown)
    {
        mouse_x_record_ = info.mouse_pos_x;
        mouse_y_record_ = info.mouse_pos_y;
        auto cadview_ = GApp::instance().getCurrentViewer();
        if (cadview_)
        {
            cadview_->setUserCursor(ViewerCore::CursorType::pan);
        }
    }
    else if (info.type_flags == rfapp::VType::MouseMove)
    {
        int mouse_x = info.mouse_pos_x;
        int mouse_y = info.mouse_pos_y;
        int delta_x = mouse_x - mouse_x_record_;
        int delta_y = mouse_y_record_ - mouse_y;
        mouse_x_record_ = mouse_x;
        mouse_y_record_ = mouse_y;

        auto cadview_ = GApp::instance().getCurrentViewer();
        if (cadview_)
        {
            cadview_->pan(delta_x, delta_y);
            cadview_->updateViewer(ViewerCore::KUpdate::Redraw);
        }
    }
    else if (info.type_flags == rfapp::VType::MButtonUp)
    {
        auto cadview_ = GApp::instance().getCurrentViewer();
        if (cadview_)
        {
            cadview_->setUserCursor(ViewerCore::CursorType::def);
        }
        this->done();
    }
}
} // namespace rfgui
