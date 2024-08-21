#include "cmd_viewerrot.h"

#include "rfgraphics/rfviewevent.h"
#include "rflog/rfLog.h"

#include "app_system.h"
#include "cmdmanager.h"
#include "viewer_utils.h"

namespace rfgui
{
void CmdRotViewer::execute()
{
}

void CmdRotViewer::processVEvent(const rfapp::ViewerEvent &info)
{
    if (info.type_flags == rfapp::VType::LRButtonDown)
    {
        if (ViewerCore *cadview = GApp::instance().getCurrentViewer(); cadview)
        {
            mouse_x_record_ = info.mouse_pos_x;
            mouse_y_record_ = info.mouse_pos_y;
            cadview->setUserCursor(ViewerCore::CursorType::rot);
            cadview->startRotation(mouse_x_record_, mouse_y_record_);
        }
    }
    else if (info.type_flags == rfapp::VType::MouseMove)
    {
        if (ViewerCore *cadview = GApp::instance().getCurrentViewer(); cadview)
        {
            cadview->rotAtPoint(info.mouse_pos_x, info.mouse_pos_y);
            cadview->updateViewer(ViewerCore::KUpdate::Redraw);
        }
    }
    else if (info.type_flags == rfapp::VType::LButtonUp ||
             info.type_flags == rfapp::VType::RButtonUp)
    {
        if (ViewerCore *cadview = GApp::instance().getCurrentViewer(); cadview)
        {
            cadview->setUserCursor(ViewerCore::CursorType::def);
        }
        this->done();
    }
}
} // namespace rfgui
