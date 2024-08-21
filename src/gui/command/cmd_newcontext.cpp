#include "cmd_newcontext.h"

#include "app_system.h"
#include "ui/viewerwidget.h" //TODO: remove this include

namespace rfgui
{
CmdNewContext::CmdNewContext()
    : CmdBase(CmdType::Immediate)
{
}

CmdNewContext::~CmdNewContext()
{
}

void CmdNewContext::execute()
{
    GApp::instance().getViewerWidget()->addNewContext();
    this->done();
}
} // namespace rfgui
