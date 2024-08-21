#include "cmd_removecontext.h"

#include <QMessageBox>

#include "app_system.h"
#include "ui/viewerwidget.h" //TODO: remove this include

namespace rfgui
{
CmdRemoveContext::CmdRemoveContext()
    : CmdBase(CmdType::Immediate)
{
}

void CmdRemoveContext::execute()
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        GApp::instance().getMainWindow(), "Confirmation",
        "Are you sure you want to close the current viewer and all related variants?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        GApp::instance().getViewerWidget()->removeContext();
    }

    this->done();
}
} // namespace rfgui
