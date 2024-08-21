#pragma once
#ifndef COMMAND_RULER_H
#define COMMAND_RULER_H

#include <functional>
#include <string_view>
#include <unordered_map>

#include <QObject>

#include <AIS_ColoredShape.hxx>

#include "cmd_base.h"
#include "ui/form/rangebar.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class InfowidgetUi;
} // namespace Ui
QT_END_NAMESPACE

class QString;

namespace rfgui
{
class CmdRuler : public CmdBase, public QObject
{
private:
    enum class RulerState
    {
        Init,
        FirstPick,
        SecondPick,
    };

public:
    CmdRuler();
    ~CmdRuler() override;

    void processVEvent(const rfapp::ViewerEvent &info) override;
    void onDone() override;
    void onCancel() override;

private:
    void execute() override;
    void on_pb_save_pressed();
    void on_pb_done_pressed();
    std::string processCaculate(Handle(SelectMgr_EntityOwner) detect_owner);
    void clearObj();
    void setInfo(const std::string &info);
    void setState(RulerState state);

    void enableSelect();
    void disableSelect();

    static TopAbs_ShapeEnum getType(Handle(SelectMgr_EntityOwner) detect_owner);
    static bool toPoint(Handle(SelectMgr_EntityOwner) detect_owner, gp_Pnt &point);

private:
    Ui::InfowidgetUi *ui_ = nullptr;
    QWidget *info_widget_ = nullptr;
    RangeGroup *dist_hover_dialog_ = nullptr;

    Handle(SelectMgr_EntityOwner) first_pick_owner_ = nullptr;
    Handle(AIS_ColoredShape) aux_show_obj_ = nullptr;

    RulerState state_ = RulerState::Init;
    rfapp::ViewerEvent last_event_; // TODO: remove this

private:
    constexpr static std::string_view kDist = "Dist";
    constexpr static std::string_view kDistX = "Dist_x";
    constexpr static std::string_view kDistY = "Dist_y";
};
} // namespace rfgui
#endif
