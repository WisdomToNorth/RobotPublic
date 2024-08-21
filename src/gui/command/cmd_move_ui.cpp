#include "cmd_move_ui.h"

#include <QDoubleSpinBox>
#include <QObject>

#include "cmd_move.h"
#include "ui_cmd_move.h"
namespace rfgui
{

CmdMoveUi::CmdMoveUi(CmdMove *cmd_ptr)
    : QObject()
    , cmd_move_(cmd_ptr)
{
    ui_ = new Ui::Motion_control();
    move_widget_ = new QWidget();
    ui_->setupUi(move_widget_);

    connect(ui_->pb_ok, &QPushButton::clicked, this, &CmdMoveUi::on_pb_ok_pressed);
    connect(ui_->pb_cancel, &QPushButton::clicked, this, &CmdMoveUi::on_pb_cancel_pressed);
    connect(ui_->btn_moveXPos, &QPushButton::clicked, this, &CmdMoveUi::on_btn_moveXPos_pressed);
    connect(ui_->btn_moveXNeg, &QPushButton::clicked, this, &CmdMoveUi::on_btn_moveXNeg_pressed);
    connect(ui_->btn_moveYPos, &QPushButton::clicked, this, &CmdMoveUi::on_btn_moveYPos_pressed);
    connect(ui_->btn_moveYNeg, &QPushButton::clicked, this, &CmdMoveUi::on_btn_moveYNeg_pressed);
    connect(ui_->btn_moveZPos, &QPushButton::clicked, this, &CmdMoveUi::on_btn_moveZPos_pressed);
    connect(ui_->btn_moveZNeg, &QPushButton::clicked, this, &CmdMoveUi::on_btn_moveZNeg_pressed);
    connect(ui_->pb_rotX, &QPushButton::clicked, this, &CmdMoveUi::on_pb_rotX_pressed);
    connect(ui_->pb_rotY, &QPushButton::clicked, this, &CmdMoveUi::on_pb_rotY_pressed);
    connect(ui_->pb_rotZ, &QPushButton::clicked, this, &CmdMoveUi::on_pb_rotZ_pressed);
    connect(ui_->pb_movetoOri, &QPushButton::clicked, this, &CmdMoveUi::on_pb_movetoOri_pressed);
    connect(ui_->pb_alignment, &QPushButton::clicked, this, &CmdMoveUi::on_pb_alignment_pressed);
    connect(ui_->pb_reverse, &QPushButton::clicked, this, &CmdMoveUi::on_pb_reverse_pressed);
    connect(ui_->pb_setas_benchmark, &QPushButton::clicked, this,
            &CmdMoveUi::on_pb_setas_benchmark_pressed);
    connect(ui_->sb_movestp, qOverload<double>(&QDoubleSpinBox::valueChanged), this,
            &CmdMoveUi::on_sb_movestp_valueChanged);
    connect(ui_->sb_rotstp, qOverload<double>(&QDoubleSpinBox::valueChanged), this,
            &CmdMoveUi::on_sb_rotstp_valueChanged);
}

CmdMoveUi::~CmdMoveUi()
{
    delete ui_;
    delete move_widget_;
}

void CmdMoveUi::on_pb_ok_pressed()
{
    cmd_move_->cmdOk();
}

void CmdMoveUi::on_pb_cancel_pressed()
{
    cmd_move_->cmdCancel();
}

void CmdMoveUi::on_btn_moveXPos_pressed()
{
    cmd_move_->moveXPos();
}

void CmdMoveUi::on_btn_moveXNeg_pressed()
{
    cmd_move_->moveXNeg();
}

void CmdMoveUi::on_btn_moveYPos_pressed()
{
    cmd_move_->moveYPos();
}

void CmdMoveUi::on_btn_moveYNeg_pressed()
{
    cmd_move_->moveYNeg();
}

void CmdMoveUi::on_btn_moveZPos_pressed()
{
    cmd_move_->moveZPos();
}

void CmdMoveUi::on_btn_moveZNeg_pressed()
{
    cmd_move_->moveZNeg();
}

void CmdMoveUi::on_pb_rotX_pressed()
{
    cmd_move_->rotX();
}

void CmdMoveUi::on_pb_rotY_pressed()
{
    cmd_move_->rotY();
};

void CmdMoveUi::on_pb_rotZ_pressed()
{
    cmd_move_->rotZ();
};

void CmdMoveUi::on_pb_movetoOri_pressed()
{
    cmd_move_->moveToOri();
}

void CmdMoveUi::on_pb_alignment_pressed()
{
    cmd_move_->alignment();
}

void CmdMoveUi::on_pb_resetMove_pressed()
{
    cmd_move_->resetMove();
}

void CmdMoveUi::on_pb_reverse_pressed()
{
    cmd_move_->reverse();
}

void CmdMoveUi::on_pb_setas_benchmark_pressed()
{
    cmd_move_->setAsBenchmark();
};

void CmdMoveUi::on_pb_reset_benchmark_pressed()
{
    cmd_move_->resetBenchmark();
}

void CmdMoveUi::on_cb_reverse_toggled(bool checked)
{
    cmd_move_->setReverseDir(checked);
}

void CmdMoveUi::on_sb_movestp_valueChanged(double arg)
{
    cmd_move_->setMoveStep(arg);
}

void CmdMoveUi::on_sb_rotstp_valueChanged(double arg)
{
    cmd_move_->setRotateStep(arg);
}

} // namespace rfgui
