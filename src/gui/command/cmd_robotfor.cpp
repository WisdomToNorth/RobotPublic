#include "cmd_robotfor.h"

#include <unordered_set>

#include "app_flag.h"
#include "app_system.h"
#include "ui_cmd_robotfor.h"
namespace rfgui
{

CmdRobotForward::CmdRobotForward() : CmdBase(CmdType::Interactive)
{
    ui_ = new Ui::robotForwardUI();
    robotfor_widget_ = new QWidget();
    ui_->setupUi(robotfor_widget_);

    connect(ui_->pb_save, &QPushButton::clicked, this, &CmdRobotForward::on_pb_cancel_pressed);
    connect(ui_->pb_done, &QPushButton::clicked, this, &CmdRobotForward::on_pb_ok_pressed);
}

void CmdRobotForward::processVEvent(const rfapp::ViewerEvent & /*info*/)
{
}

void CmdRobotForward::onDone()
{
}

void CmdRobotForward::execute()
{
    GApp::instance().addCmdDock(robotfor_widget_, "RobotForward"); // TODO: refactory
    std::unordered_set<selectflag::Select> select_set{selectflag::kShape};
    GApp::instance().setTempSelectMode(select_set, select_set);
}

void CmdRobotForward::on_pb_ok_pressed()
{
    rflog::debug("cmd move ok");
    this->done();
}

void CmdRobotForward::on_pb_cancel_pressed()
{
    // TODO: roll back
    rflog::debug("cmd move cancel");
    this->done();
}
} // namespace rfgui
