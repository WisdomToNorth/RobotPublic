#pragma once
#ifndef COMMAND_SHOW_INFO_H
#define COMMAND_SHOW_INFO_H

#include <string>
#include <vector>

#include <QPlainTextEdit>

#include <SelectMgr_EntityOwner.hxx>

#include "rfgraphics/rfviewevent.h"

#include "cmd_base.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class InfowidgetUi;
} // namespace Ui
QT_END_NAMESPACE

namespace rfgui
{
class CmdShowInfo : public CmdBase, public QObject
{
public:
    CmdShowInfo();
    ~CmdShowInfo() override;

    void processVEvent(const rfapp::ViewerEvent &info) override;

private:
    void execute() override;
    void on_pb_save_pressed();
    void on_pb_done_pressed();

    void setInfo(const std::string &info);
    void handleModelInfo(Handle(SelectMgr_EntityOwner) detect_owner_);
    void handlePtInfo(Handle(SelectMgr_EntityOwner) detect_owner_);
    void handleEdgeInfo(Handle(SelectMgr_EntityOwner) detect_owner_);
    void handleFaceInfo(Handle(SelectMgr_EntityOwner) detect_owner_);
    void handleSolidInfo(Handle(SelectMgr_EntityOwner) detect_owner_);

private:
    Ui::InfowidgetUi *ui_ = nullptr;
    QWidget *info_widget_ = nullptr;
};
} // namespace rfgui
#endif
