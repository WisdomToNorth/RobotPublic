#pragma once
#ifndef COMMAND_MOVE_UI_H
#define COMMAND_MOVE_UI_H

#include <QPoint>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
class Motion_control;
}
QT_END_NAMESPACE

namespace rfgui
{
class CmdMove;
class CmdMoveUi : public QObject
{
public:
    CmdMoveUi(CmdMove *cmd_ptr);
    ~CmdMoveUi();

    QWidget *getWidget() const
    {
        return move_widget_;
    }

private:
    void on_pb_ok_pressed();
    void on_pb_cancel_pressed();

    void on_btn_moveXPos_pressed();
    void on_btn_moveXNeg_pressed();
    void on_btn_moveYPos_pressed();
    void on_btn_moveYNeg_pressed();
    void on_btn_moveZPos_pressed();
    void on_btn_moveZNeg_pressed();
    void on_pb_rotX_pressed();
    void on_pb_rotY_pressed();
    void on_pb_rotZ_pressed();

    void on_pb_movetoOri_pressed();
    void on_pb_alignment_pressed();
    void on_pb_reverse_pressed();
    void on_pb_resetMove_pressed();
    void on_pb_setas_benchmark_pressed();
    void on_pb_reset_benchmark_pressed();
    void on_cb_reverse_toggled(bool checked);
    void on_sb_movestp_valueChanged(double arg);
    void on_sb_rotstp_valueChanged(double arg);

private:
    Ui::Motion_control *ui_;

    QWidget *move_widget_ = nullptr;
    CmdMove *cmd_move_ = nullptr;
};
} // namespace rfgui
#endif
