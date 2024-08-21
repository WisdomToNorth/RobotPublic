

#include "cmd_move.h"

#include <qglobal.h>

#include <AIS_InteractiveObject.hxx>
#include <StdSelect_BRepOwner.hxx>

#include "rfgeo3d/geodataio.h"
#include "rfgeo3d/geoutils.h"
#include "rfgeo3d/vmselectmode.h"
#include "rfgeo3d/vmutils.h"
#include "rflog/rfLog.h"

#include "app_flag.h"
#include "app_system.h"
#include "cmd_move_ui.h"
#include "ui_cmd_move.h"
#include "viewer_utils.h"
#include "viewnode_utils.h"

namespace rfgui
{
const std::string CmdMove::action_resetmove = "Reset Move";
const std::string CmdMove::action_move_x_pos = "Move X+";
const std::string CmdMove::action_move_x_neg = "Move X-";
const std::string CmdMove::action_move_y_pos = "Move Y+";
const std::string CmdMove::action_move_y_neg = "Move Y-";
const std::string CmdMove::action_move_z_pos = "Move Z+";
const std::string CmdMove::action_move_z_neg = "Move Z-";

CmdMove::CmdMove()
    : CmdBase(CmdType::Interactive)
{
    cmd_move_ui_ = new CmdMoveUi(this);
}

CmdMove::~CmdMove()
{
}

void CmdMove::execute()
{
    GApp::instance().addCmdDock(cmd_move_ui_->getWidget(), "Move"); // TODO: refactory
    std::unordered_set<selectflag::Select> select_set{selectflag::kShape};
    GApp::instance().setTempSelectMode(select_set, select_set);
}

void CmdMove::createRMenu()
{
    GApp::instance().addToRMenu(action_resetmove);
    GApp::instance().addToRMenu(action_move_x_pos);
    GApp::instance().addToRMenu(action_move_x_neg);
    GApp::instance().addToRMenu(action_move_y_pos);
    GApp::instance().addToRMenu(action_move_y_neg);
    GApp::instance().addToRMenu(action_move_z_pos);
    GApp::instance().addToRMenu(action_move_z_neg);
}

void CmdMove::processRMAction(const std::string &action_name)
{
    rflog::debug("CmdMove::processRMAction:{}", action_name);
    if (action_name == action_resetmove)
    {
        resetMove();
    }
    else if (action_name == action_move_x_pos)
    {
        moveXPos();
    }
    else if (action_name == action_move_x_neg)
    {
        moveXNeg();
    }
    else if (action_name == action_move_y_pos)
    {
        moveYPos();
    }
    else if (action_name == action_move_y_neg)
    {
        moveYNeg();
    }
    else if (action_name == action_move_z_pos)
    {
        moveZPos();
    }
    else if (action_name == action_move_z_neg)
    {
        moveZNeg();
    }
}

void CmdMove::processVEvent(const rfapp::ViewerEvent &info)
{
    if (info.type_flags & rfapp::LButtonDown)
    {
        if (info.picked_entity.IsNull())
        {
            return;
        }
        Handle(StdSelect_BRepOwner) slt_owner =
            Handle(StdSelect_BRepOwner)::DownCast(info.picked_entity);
        if (!slt_owner.IsNull())
        {
            setCurObj(slt_owner);
        }
    }
}

void CmdMove::setBenchmark(gp_Ax2 benchmark)
{
    benchmark_ = benchmark;
}

void CmdMove::setCurObj(Handle(SelectMgr_EntityOwner) _currentModel)
{
    Handle(AIS_InteractiveObject) ais_shp =
        Handle(AIS_InteractiveObject)::DownCast(_currentModel->Selectable());

    ais_shp = rfgeo::VMUtils::getRootParent(ais_shp);

    current_model_ = GApp::instance().getViewTreeByViewObj(ais_shp);
    if (current_model_)
    {
        rflog::debug("Cur select object is changed to:{}", current_model_->name_);
    }
    else
    {
        rflog::debug("Cur select object is changed to:{}", "null");
    }
}

gp_Pnt CmdMove::dirEnumToDir(Direction dir)
{
    switch (dir)
    {
    case rfgui::CmdMove::Direction::XP: return gp_Pnt(1.0, 0, 0);
    case rfgui::CmdMove::Direction::XM: return gp_Pnt(-1.0, 0, 0);
    case rfgui::CmdMove::Direction::YP: return gp_Pnt(0, 1.0, 0);
    case rfgui::CmdMove::Direction::YM: return gp_Pnt(0, -1.0, 0);
    case rfgui::CmdMove::Direction::ZP: return gp_Pnt(0, 0, 1.0);
    case rfgui::CmdMove::Direction::ZM: return gp_Pnt(0, 0, -1.0);
    }
    return gp_Pnt();
}

void CmdMove::transformObj(const gp_Trsf &myTrsf) const
{
    if (!current_model_)
    {
        return;
    }

    ViewNodeUtils::transform(current_model_, myTrsf);
    ViewerUtils::updateVTNodeViewObject(current_model_);
    ViewerUtils::updateCurrentViewer();
}

void CmdMove::setMoveStep(double step)
{
    this->move_step_ = step;
}

void CmdMove::setRotateStep(double step)
{
    this->rotate_step_ = step;
}

void CmdMove::setReverseDir(bool reverse)
{
    this->reverse_dir_ = reverse;
}

void CmdMove::objMove(Direction dir)
{
    const gp_Pnt &dir_pt = dirEnumToDir(dir);

    const gp_Pnt &scaled = rfgeo::GeoUtils::scalePt(dir_pt, this->move_step_);

    gp_Trsf myTrsf;
    myTrsf.SetTranslation(gp_Pnt(0, 0, 0), scaled);
    transformObj(myTrsf);
}

void CmdMove::objRotate(Direction dir_enum)
{
    const gp_Pnt &dir_pt = dirEnumToDir(dir_enum);
    const gp_Ax1 &ax1 = gp_Ax1(benchmark_.Location(), gp_Dir(dir_pt.X(), dir_pt.Y(), dir_pt.Z()));

    const double rot_num = this->rotate_step_ / 180.0 * M_PI;

    gp_Trsf myTrsf;
    myTrsf.SetRotation(ax1, rot_num);
    transformObj(myTrsf);
}

void CmdMove::cmdOk()
{
    rflog::debug("cmd move ok");
    this->done();
}

void CmdMove::cmdCancel()
{
    // TODO: roll back
    rflog::debug("cmd move cancel");
    this->done();
}

void CmdMove::moveXPos()
{
    objMove(Direction::XP);
}

void CmdMove::moveXNeg()
{
    objMove(Direction::XM);
}

void CmdMove::moveYPos()
{
    objMove(Direction::YP);
}

void CmdMove::moveYNeg()
{
    objMove(Direction::YM);
}

void CmdMove::moveZPos()
{
    objMove(Direction::ZP);
}

void CmdMove::moveZNeg()
{
    objMove(Direction::ZM);
}

void CmdMove::rotX()
{
    if (this->reverse_dir_)
        objRotate(Direction::XM);
    else
        objRotate(Direction::XP);
}

void CmdMove::rotY()
{
    if (this->reverse_dir_)
        objRotate(Direction::YM);
    else
        objRotate(Direction::YP);
};

void CmdMove::rotZ()
{
    if (this->reverse_dir_)
        objRotate(Direction::ZM);
    else
        objRotate(Direction::ZP);
};

void CmdMove::moveToOri() // 只移动不对齐
{
    // gp_Trsf move_trsf;
    // move_trsf.SetTranslation(temp_abassica_->getAx2().Location(),
    //                          ref_abassica_->getAx2().Location());

    // Handle(AIS_ColoredShape) cur_shp = this->getSelectedModel();
    // cur_shp = getSelectedModel();
    // if (!cur_shp)
    // {
    //     return;
    // }

    // rfgeo::VMUtils::transformLeft(cur_shp, move_trsf);
    // ViewerUtils::updateObject(cur_shp, false);

    // if (temp_abassica_)
    // {
    //     temp_abassica_->transformLeft(move_trsf);
    //     ViewerUtils::updateObject(temp_abassica_, false);
    // }
    // ViewerUtils::updateViewer(0);
}

void CmdMove::alignment()
{
    // gp_Trsf currentTrsf =
    //     GeoUtils::cpuRotZDirection(ref_abassica_->getAx2(), temp_abassica_->getAx2());
    // auto a = currentTrsf.Inverted();

    // gp_Ax2 world_benchmark(gp_Pnt(0, 0, 0), gp_Dir(gp_Vec(gp_Pnt(0, 0, 0), gp_Pnt(0, 0, 1))));
    // gp_Trsf currentTrsf_ori;
    // currentTrsf_ori.SetTranslation(world_benchmark.Location(),
    // temp_abassica_->getAx2().Location());

    // a = currentTrsf_ori * a * currentTrsf_ori.Inverted();

    // Handle(AIS_ColoredShape) cur_shp = this->getSelectedModel();
    // cur_shp = getSelectedModel();
    // if (!cur_shp)
    // {
    //     return;
    // }
    // rfgeo::VMUtils::transformLeft(cur_shp, a);

    // ViewerUtils::updateObject(cur_shp, false);

    // if (temp_abassica_)
    // {
    //     temp_abassica_->transformLeft(a);
    //     ViewerUtils::updateObject(temp_abassica_, false);
    // }
    // ViewerUtils::updateViewer(0);
}

void CmdMove::resetMove()
{
    // Handle(AIS_ColoredShape) cur_shp = this->getSelectedModel();
    // cur_shp = getSelectedModel();
    // if (!cur_shp)
    // {
    //     return;
    // }

    // // std::cout << GeoDataIO::occTypeToStr(cur_shp->getAccuTrsf());
    // gp_Trsf a; // TODO: = cur_shp->getAccuTrsf().Inverted();
    // rfgeo::VMUtils::transformLeft(cur_shp, a);

    // ViewerUtils::updateObject(cur_shp, false);

    // clearAuxiliaryModel();
}

void CmdMove::reverse()
{
    // if (AIS_ColoredShape::isShapeVoid(temp_abassica_))
    // {
    //     return;
    // }

    // temp_abassica_->reverse();
    // ViewerUtils::updateObject(temp_abassica_, true);
}

void CmdMove::setAsBenchmark() {
    // ViewerUtils::removeObject(ref_abassica_, false);
    // ref_abassica_->updateAbassica(temp_abassica_->getAx2(), SIZE_OF_ABASSICA);
    // ViewerUtils::addToViewer(ref_abassica_, -1, true);

    // QString l_info("zero point set as("
    //                +
    // QString::fromStdString(GeoDataIO::occTypeToStr(ref_abassica_->getAx2().Location()))
    //                + ")");

    // emit updateInfoSig(l_info);

    // auto m = vm_index_model_node_[ref_abassica_];
    // m->item_->setCheckState(0, Qt::CheckState::Checked);
};

void CmdMove::resetBenchmark() {
    // QString l_info("zero point set as (0,0,0)");
    // emit updateInfoSig(l_info);

    // ViewerUtils::removeObject(ref_abassica_, false);

    // ref_abassica_->updateAbassica(
    //     gp_Ax2(gp_Pnt(0, 0, 0), gp_Dir(gp_Vec(gp_Pnt(0, 0, 0), gp_Pnt(0, 0, 1)))),
    //     SIZE_OF_ABASSICA);
    // AuxItem *m = vm_index_model_node_[ref_abassica_];
    // m->item_->setCheckState(0, Qt::CheckState::Checked);

    // ViewerUtils::addToViewer(ref_abassica_, -1, true);
};
} // namespace rfgui
