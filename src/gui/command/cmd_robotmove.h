#pragma once
#ifndef COMMAND_ROBOTMOVE_H
#define COMMAND_ROBOTMOVE_H

#include <functional>

#include <QObject>

#include "cmd_base.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class robotMoveUI;
} // namespace Ui
QT_END_NAMESPACE

namespace rfgui
{
class CmdRobotMove : public CmdBase, public QObject
{
private:
    enum class RulerState
    {
        Init,
        FirstPick,
        SecondPick,
    };

public:
    CmdRobotMove();
    ~CmdRobotMove() override = default;

    void processVEvent(const rfapp::ViewerEvent &info) override;
    void onDone() override;

private:
    void execute() override;
    void on_pb_ok_pressed();
    void on_pb_cancel_pressed();

private:
    Ui::robotMoveUI *ui_ = nullptr;
    QWidget *robotmove_widget_ = nullptr;
};
} // namespace rfgui
#endif
