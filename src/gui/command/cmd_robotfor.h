#pragma once
#ifndef COMMAND_ROBOTFOR_H
#define COMMAND_ROBOTFOR_H

#include <functional>

#include <QObject>

#include "cmd_base.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class robotForwardUI;
} // namespace Ui
QT_END_NAMESPACE

class QString;

namespace rfgui
{
class CmdRobotForward : public CmdBase, public QObject
{
private:
    enum class RulerState
    {
        Init,
        FirstPick,
        SecondPick,
    };

public:
    CmdRobotForward();
    ~CmdRobotForward() override = default;

    void processVEvent(const rfapp::ViewerEvent &info) override;
    void onDone() override;

private:
    void execute() override;
    void on_pb_ok_pressed();
    void on_pb_cancel_pressed();

private:
    Ui::robotForwardUI *ui_ = nullptr;
    QWidget *robotfor_widget_ = nullptr;
};
} // namespace rfgui
#endif
