#pragma once
#ifndef CMD_BASE_H
#define CMD_BASE_H

#include "rfgraphics/rfviewevent.h"

#include "cmddefine.h"

namespace rfgui
{
class CmdBase
{
public:
    CmdBase(CmdType type);
    virtual ~CmdBase();
    CmdBase(const CmdBase &) = delete;
    CmdBase &operator=(const CmdBase &) = delete;

    virtual void done();
    virtual void undo();
    virtual void onDone();
    virtual void onCancel();
    virtual void createRMenu();
    virtual void processRMAction(const std::string & /*action name*/);
    virtual void processVEvent(const rfapp::ViewerEvent & /*info*/);

    CmdType getType() const;
    void setCmdName(const std::string &cmd_name);
    std::string getCmdName() const;
    const cmd::Parameter &getParameter() const;
    void setParameter(const cmd::Parameter &parameter);

protected:
    cmd::Parameter parameter_;

private:
    friend class CmdMngr;

    virtual void execute() = 0;
    CmdType type_ = CmdType::Unknown;
    std::string cmd_name_;
};

} // namespace rfgui
#endif // !_COMMAND_BASE_H_
