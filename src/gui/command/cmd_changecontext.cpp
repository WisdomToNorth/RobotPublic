#include "cmd_changecontext.h"

#include "app_system.h"
#include "cmddefine.h"
#include "ui/viewerwidget.h" //TODO: remove this include

namespace rfgui
{
CmdChangeContext::CmdChangeContext()
    : CmdBase(CmdType::Immediate)
{
}

CmdChangeContext::~CmdChangeContext()
{
}

void CmdChangeContext::execute()
{
    GApp::instance().getViewerWidget()->setCurrentContext(parameter_.int_value);
    this->done();
}
} // namespace rfgui
