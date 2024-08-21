#include "cmd_base.h"

#include "app_system.h"

namespace rfgui
{
CmdBase::CmdBase(CmdType type) : type_(type)
{
}
CmdBase::~CmdBase()
{
}

void CmdBase::undo()
{
}
void CmdBase::onDone()
{
}
void CmdBase::onCancel()
{
}
void CmdBase::createRMenu()
{
}
void CmdBase::processRMAction(const std::string & /*action name*/)
{
}
void CmdBase::processVEvent(const rfapp::ViewerEvent & /*info*/)
{
}
void CmdBase::done()
{
    GApp::instance().setCmdDone();
}

CmdType CmdBase::getType() const
{
    return type_;
};

void CmdBase::setCmdName(const std::string &cmd_name)
{
    cmd_name_ = cmd_name;
}

std::string CmdBase::getCmdName() const
{
    return cmd_name_;
}

const cmd::Parameter &CmdBase::getParameter() const
{
    return parameter_;
}

void CmdBase::setParameter(const cmd::Parameter &parameter)
{
    parameter_ = parameter;
}

} // namespace rfgui
