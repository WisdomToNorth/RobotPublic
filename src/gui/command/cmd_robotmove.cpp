#include "cmd_robotmove.h"

#include <unordered_set>

#include "app_flag.h"
#include "app_system.h"
#include "ui_cmd_robotmove.h"
namespace rfgui
{

CmdRobotMove::CmdRobotMove() : CmdBase(CmdType::Interactive)
{
    ui_ = new Ui::robotMoveUI();
    robotmove_widget_ = new QWidget();
    ui_->setupUi(robotmove_widget_);

    connect(ui_->pb_save, &QPushButton::clicked, this, &CmdRobotMove::on_pb_cancel_pressed);
    connect(ui_->pb_done, &QPushButton::clicked, this, &CmdRobotMove::on_pb_ok_pressed);
}

void CmdRobotMove::processVEvent(const rfapp::ViewerEvent & /*info*/)
{
}

void CmdRobotMove::onDone()
{
}

void CmdRobotMove::execute()
{
    GApp::instance().addCmdDock(robotmove_widget_, "RobotMove"); // TODO: refactory
    std::unordered_set<selectflag::Select> select_set{selectflag::kShape};
    GApp::instance().setTempSelectMode(select_set, select_set);
}

void CmdRobotMove::on_pb_ok_pressed()
{
    rflog::debug("cmd move ok");
    this->done();
}

void CmdRobotMove::on_pb_cancel_pressed()
{
    // TODO: roll back
    rflog::debug("cmd move cancel");
    this->done();
}

// void gui_controls::cartx_plus()
// {
//     if (ui->cb_mode_fb->isChecked())
//     {
//         KDL::Frame temp = cart;
//         cart.p.x(cart.p.x() + cart_stepsize->Pin);
//         int ok = kinematic().Ik();
//         if (!ok)
//         {
//             cart.p.x(cart.p.x() - cart_stepsize->Pin); // Ik error, restore value
//             ui->toolButton_status->setStyleSheet(red);
//         }
//         else
//         {
//             cart.p.x(cart.p.x() - cart_stepsize->Pin); // Ik ok, substrackt test value.

//             // Create stream.
//             int ok = jog().jog_cart_euler(cart_stepsize->Pin, 0, 0, 0, 0, 0);
//             if (ok)
//             {
//                 okmove = true;
//                 ui->toolButton_status->setStyleSheet(green);
//             }
//             else
//             {
//                 cart = temp;
//                 ui->toolButton_status->setStyleSheet(red);
//             }
//         }
//     }
//     else
//     {
//         cart.p.x(cart.p.x() + cart_stepsize->Pin);
//         int ok = kinematic().Ik();
//         if (!ok)
//         {
//             cart.p.x(cart.p.x() - cart_stepsize->Pin); // restore failure
//             ui->toolButton_status->setStyleSheet(red);
//         }
//         else
//         {
//             ui->toolButton_status->setStyleSheet(green);
//         }
//     }
// }

// void gui_controls::cartx_min()
// {
//     if (ui->cb_mode_fb->isChecked())
//     {
//         KDL::Frame temp = cart;
//         cart.p.x(cart.p.x() - cart_stepsize->Pin);
//         int ok = kinematic().Ik();
//         if (!ok)
//         {
//             cart.p.x(cart.p.x() + cart_stepsize->Pin); // Ik error, restore value
//             ui->toolButton_status->setStyleSheet(red);
//         }
//         else
//         {
//             cart.p.x(cart.p.x() + cart_stepsize->Pin); // Ik ok, substrackt test value.

//             // Create stream.
//             int ok = jog().jog_cart_euler(cart_stepsize->Pin * -1, 0, 0, 0, 0, 0);
//             if (ok)
//             {
//                 okmove = true;
//                 ui->toolButton_status->setStyleSheet(green);
//             }
//             else
//             {
//                 cart = temp;
//                 ui->toolButton_status->setStyleSheet(red);
//             }
//         }
//     }
//     else
//     {
//         cart.p.x(cart.p.x() + cart_stepsize->Pin * -1);
//         int ok = kinematic().Ik();
//         if (!ok)
//         {
//             cart.p.x(cart.p.x() - cart_stepsize->Pin * -1); // restore failure
//             ui->toolButton_status->setStyleSheet(red);
//         }
//         else
//         {
//             ui->toolButton_status->setStyleSheet(green);
//         }
//     }
// }

// void gui_controls::carty_plus()
// {
//     if (ui->cb_mode_fb->isChecked())
//     {
//         KDL::Frame temp = cart;
//         cart.p.y(cart.p.y() + cart_stepsize->Pin);
//         int ok = kinematic().Ik();
//         if (!ok)
//         {
//             cart.p.y(cart.p.y() - cart_stepsize->Pin); // Ik error, restore value
//             ui->toolButton_status->setStyleSheet(red);
//         }
//         else
//         {
//             cart.p.y(cart.p.y() - cart_stepsize->Pin); // Ik ok, substrackt test value.

//             // Create stream.
//             int ok = jog().jog_cart_euler(0, cart_stepsize->Pin, 0, 0, 0, 0);
//             if (ok)
//             {
//                 okmove = true;
//                 ui->toolButton_status->setStyleSheet(green);
//             }
//             else
//             {
//                 cart = temp;
//                 ui->toolButton_status->setStyleSheet(red);
//             }
//         }
//     }
//     else
//     {
//         cart.p.y(cart.p.y() + cart_stepsize->Pin);
//         int ok = kinematic().Ik();
//         if (!ok)
//         {
//             cart.p.y(cart.p.y() - cart_stepsize->Pin); // restore failure
//             ui->toolButton_status->setStyleSheet(red);
//         }
//         else
//         {
//             ui->toolButton_status->setStyleSheet(green);
//         }
//     }
// }

// void gui_controls::carty_min()
// {
//     if (ui->cb_mode_fb->isChecked())
//     {
//         KDL::Frame temp = cart;
//         cart.p.y(cart.p.y() - cart_stepsize->Pin);
//         int ok = kinematic().Ik();
//         if (!ok)
//         {
//             cart.p.y(cart.p.y() + cart_stepsize->Pin); // Ik error, restore value
//             ui->toolButton_status->setStyleSheet(red);
//         }
//         else
//         {
//             cart.p.y(cart.p.y() + cart_stepsize->Pin); // Ik ok, substrackt test value.

//             // Create stream.
//             int ok = jog().jog_cart_euler(0, cart_stepsize->Pin * -1, 0, 0, 0, 0);
//             if (ok)
//             {
//                 okmove = true;
//                 ui->toolButton_status->setStyleSheet(green);
//             }
//             else
//             {
//                 cart = temp;
//                 ui->toolButton_status->setStyleSheet(red);
//             }
//         }
//     }
//     else
//     {
//         cart.p.y(cart.p.y() + cart_stepsize->Pin * -1);
//         int ok = kinematic().Ik();
//         if (!ok)
//         {
//             cart.p.y(cart.p.y() - cart_stepsize->Pin * -1); // restore failure
//             ui->toolButton_status->setStyleSheet(red);
//         }
//         else
//         {
//             ui->toolButton_status->setStyleSheet(green);
//         }
//     }
// }

// void gui_controls::cartz_plus()
// {
//     if (ui->cb_mode_fb->isChecked())
//     {
//         KDL::Frame temp = cart;
//         cart.p.z(cart.p.z() + cart_stepsize->Pin);
//         int ok = kinematic().Ik();
//         if (!ok)
//         {
//             cart.p.z(cart.p.z() - cart_stepsize->Pin); // Ik error, restore value
//             ui->toolButton_status->setStyleSheet(red);
//         }
//         else
//         {
//             cart.p.z(cart.p.z() - cart_stepsize->Pin); // Ik ok, substrackt test value.
//             // Create stream.
//             int ok = jog().jog_cart_euler(0, 0, cart_stepsize->Pin, 0, 0, 0);
//             if (ok)
//             {
//                 okmove = true;
//                 ui->toolButton_status->setStyleSheet(green);
//             }
//             else
//             {
//                 cart = temp;
//                 ui->toolButton_status->setStyleSheet(red);
//             }
//         }
//     }
//     else
//     {
//         cart.p.z(cart.p.z() + cart_stepsize->Pin);
//         int ok = kinematic().Ik();
//         if (!ok)
//         {
//             cart.p.z(cart.p.z() - cart_stepsize->Pin); // restore failure
//             ui->toolButton_status->setStyleSheet(red);
//         }
//         else
//         {
//             ui->toolButton_status->setStyleSheet(green);
//         }
//     }
// }

// void gui_controls::cartz_min()
// {
//     if (ui->cb_mode_fb->isChecked())
//     {
//         KDL::Frame temp = cart;
//         cart.p.z(cart.p.z() - cart_stepsize->Pin);
//         int ok = kinematic().Ik();
//         if (!ok)
//         {
//             cart.p.z(cart.p.z() + cart_stepsize->Pin); // Ik error, restore value
//             ui->toolButton_status->setStyleSheet(red);
//         }
//         else
//         {
//             cart.p.z(cart.p.z() + cart_stepsize->Pin); // Ik ok, substrackt test value.

//             // Create stream.
//             int ok = jog().jog_cart_euler(0, 0, cart_stepsize->Pin * -1, 0, 0, 0);
//             if (ok)
//             {
//                 okmove = true;
//                 ui->toolButton_status->setStyleSheet(green);
//             }
//             else
//             {
//                 cart = temp;
//                 ui->toolButton_status->setStyleSheet(red);
//             }
//         }
//     }
//     else
//     {
//         cart.p.z(cart.p.z() + cart_stepsize->Pin * -1);
//         int ok = kinematic().Ik();
//         if (!ok)
//         {
//             cart.p.z(cart.p.z() - cart_stepsize->Pin * -1); // restore failure
//             ui->toolButton_status->setStyleSheet(red);
//         }
//         else
//         {
//             ui->toolButton_status->setStyleSheet(green);
//         }
//     }
// }

// void gui_controls::eulerx_plus()
// {
//     if (ui->cb_mode_fb->isChecked())
//     {
//         cart.M.DoRotX(euler_stepsize->Pin * toRadians);
//         int ok = kinematic().Ik();
//         if (!ok)
//         {
//             cart.M.DoRotX(euler_stepsize->Pin * toRadians * -1); // Restore value
//             ui->toolButton_status->setStyleSheet(red);
//         }
//         else
//         { // Test is ok.
//             cart.M.DoRotX(euler_stepsize->Pin * toRadians
//                           * -1); // Substract value to original value

//             int ok = jog().jog_cart_euler(0, 0, 0, euler_stepsize->Pin, 0, 0);
//             if (ok)
//             {
//                 okmove = true;
//                 ui->toolButton_status->setStyleSheet(green);
//             }
//             else
//             {
//                 ui->toolButton_status->setStyleSheet(red);
//             }
//         }
//     }
//     else
//     {
//         cart.M.DoRotX(euler_stepsize->Pin * toRadians);
//         int ok = kinematic().Ik();
//         if (!ok)
//         {
//             cart.M.DoRotX(euler_stepsize->Pin * toRadians * -1); // Restore value
//             ui->toolButton_status->setStyleSheet(red);
//         }
//         else
//         {
//             ui->toolButton_status->setStyleSheet(green);
//         }
//     }
// }

// void gui_controls::eulerx_min()
// {
//     if (ui->cb_mode_fb->isChecked())
//     {
//         cart.M.DoRotX(euler_stepsize->Pin * toRadians * -1);
//         int ok = kinematic().Ik();
//         if (!ok)
//         {
//             cart.M.DoRotX(euler_stepsize->Pin * toRadians);
//             ui->toolButton_status->setStyleSheet(red);
//         }
//         else
//         {
//             cart.M.DoRotX(euler_stepsize->Pin * toRadians);

//             int ok = jog().jog_cart_euler(0, 0, 0, euler_stepsize->Pin * -1, 0, 0);
//             if (ok)
//             {
//                 okmove = true;
//                 ui->toolButton_status->setStyleSheet(green);
//             }
//             else
//             {
//                 ui->toolButton_status->setStyleSheet(red);
//             }
//         }
//     }
//     else
//     {
//         cart.M.DoRotX(euler_stepsize->Pin * toRadians * -1);
//         int ok = kinematic().Ik();
//         if (!ok)
//         {
//             cart.M.DoRotX(euler_stepsize->Pin * toRadians); // Restore value
//             ui->toolButton_status->setStyleSheet(red);
//         }
//         else
//         {
//             ui->toolButton_status->setStyleSheet(green);
//         }
//     }
// }

// void gui_controls::eulery_plus()
// {
//     if (ui->cb_mode_fb->isChecked())
//     {
//         cart.M.DoRotY(euler_stepsize->Pin * toRadians);
//         int ok = kinematic().Ik();
//         if (!ok)
//         {
//             cart.M.DoRotY(euler_stepsize->Pin * toRadians * -1); // Restore value
//             ui->toolButton_status->setStyleSheet(red);
//         }
//         else
//         { // Test is ok.
//             cart.M.DoRotY(euler_stepsize->Pin * toRadians
//                           * -1); // Substract value to original value

//             int ok = jog().jog_cart_euler(0, 0, 0, 0, euler_stepsize->Pin, 0);
//             if (ok)
//             {
//                 okmove = true;
//                 ui->toolButton_status->setStyleSheet(green);
//             }
//             else
//             {
//                 ui->toolButton_status->setStyleSheet(red);
//             }
//         }
//     }
//     else
//     {
//         cart.M.DoRotY(euler_stepsize->Pin * toRadians);
//         int ok = kinematic().Ik();
//         if (!ok)
//         {
//             cart.M.DoRotY(euler_stepsize->Pin * toRadians * -1); // Restore value
//             ui->toolButton_status->setStyleSheet(red);
//         }
//         else
//         {
//             ui->toolButton_status->setStyleSheet(green);
//         }
//     }
// }

// void gui_controls::eulery_min()
// {
//     if (ui->cb_mode_fb->isChecked())
//     {
//         cart.M.DoRotY(euler_stepsize->Pin * toRadians * -1);
//         int ok = kinematic().Ik();
//         if (!ok)
//         {
//             cart.M.DoRotY(euler_stepsize->Pin * toRadians);
//             ui->toolButton_status->setStyleSheet(red);
//         }
//         else
//         {
//             cart.M.DoRotY(euler_stepsize->Pin * toRadians);

//             int ok = jog().jog_cart_euler(0, 0, 0, 0, euler_stepsize->Pin * -1, 0);
//             if (ok)
//             {
//                 okmove = true;
//                 ui->toolButton_status->setStyleSheet(green);
//             }
//             else
//             {
//                 ui->toolButton_status->setStyleSheet(red);
//             }
//         }
//     }
//     else
//     {
//         cart.M.DoRotY(euler_stepsize->Pin * toRadians * -1);
//         int ok = kinematic().Ik();
//         if (!ok)
//         {
//             cart.M.DoRotY(euler_stepsize->Pin * toRadians); // Restore value
//             ui->toolButton_status->setStyleSheet(red);
//         }
//         else
//         {
//             ui->toolButton_status->setStyleSheet(green);
//         }
//     }
// }

// void gui_controls::eulerz_plus()
// {
//     if (ui->cb_mode_fb->isChecked())
//     {
//         cart.M.DoRotZ(euler_stepsize->Pin * toRadians);
//         int ok = kinematic().Ik();
//         if (!ok)
//         {
//             cart.M.DoRotZ(euler_stepsize->Pin * toRadians * -1);
//             ui->toolButton_status->setStyleSheet(red);
//         }
//         else
//         {
//             cart.M.DoRotZ(euler_stepsize->Pin * toRadians * -1);

//             int ok = jog().jog_cart_euler(0, 0, 0, 0, 0, euler_stepsize->Pin);
//             if (ok)
//             {
//                 okmove = true;
//                 ui->toolButton_status->setStyleSheet(green);
//             }
//             else
//             {
//                 ui->toolButton_status->setStyleSheet(red);
//             }
//         }
//     }
//     else
//     {
//         cart.M.DoRotZ(euler_stepsize->Pin * toRadians);
//         int ok = kinematic().Ik();
//         if (!ok)
//         {
//             cart.M.DoRotZ(euler_stepsize->Pin * toRadians * -1); // Restore value
//             ui->toolButton_status->setStyleSheet(red);
//         }
//         else
//         {
//             ui->toolButton_status->setStyleSheet(green);
//         }
//     }
// }

// void gui_controls::eulerz_min()
// {
//     if (ui->cb_mode_fb->isChecked())
//     {
//         cart.M.DoRotZ(euler_stepsize->Pin * toRadians * -1);
//         int ok = kinematic().Ik();
//         if (!ok)
//         {
//             cart.M.DoRotZ(euler_stepsize->Pin * toRadians);
//             ui->toolButton_status->setStyleSheet(red);
//         }
//         else
//         {
//             cart.M.DoRotZ(euler_stepsize->Pin * toRadians);

//             int ok = jog().jog_cart_euler(0, 0, 0, 0, 0, euler_stepsize->Pin * -1);
//             if (ok)
//             {
//                 okmove = true;
//                 ui->toolButton_status->setStyleSheet(green);
//             }
//             else
//             {
//                 ui->toolButton_status->setStyleSheet(red);
//             }
//         }
//     }
//     else
//     {
//         cart.M.DoRotZ(euler_stepsize->Pin * toRadians * -1);
//         int ok = kinematic().Ik();
//         if (!ok)
//         {
//             cart.M.DoRotZ(euler_stepsize->Pin * toRadians); // Restore value
//             ui->toolButton_status->setStyleSheet(red);
//         }
//         else
//         {
//             ui->toolButton_status->setStyleSheet(green);
//         }
//     }
// }

} // namespace rfgui
