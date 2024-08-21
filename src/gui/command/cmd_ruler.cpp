
#include "cmd_ruler.h"

#include <rfgeo3d/vsbuilder.h>

#include <QClipboard>
#include <QString>

#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRep_Builder.hxx>
#include <BRep_Tool.hxx>
#include <StdSelect_BRepOwner.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>

#include "rfgeo3d/geodataio.h"
#include "rflog/rfLog.h"

#include "app_flag.h"
#include "app_system.h"
#include "ui_cmd_showinfo.h"
#include "viewer_utils.h"

namespace rfgui
{

CmdRuler::CmdRuler()
    : CmdBase(CmdType::Interactive)
{
    ui_ = new Ui::InfowidgetUi();
    info_widget_ = new QWidget();
    ui_->setupUi(info_widget_);

    connect(ui_->pb_save, &QPushButton::clicked, this, &CmdRuler::on_pb_save_pressed);
    connect(ui_->pb_done, &QPushButton::clicked, this, &CmdRuler::on_pb_done_pressed);

    QFont qf;
    qf.setFamily("Consolas");
    qf.setPointSize(12);
    ui_->info_plainedit->setFont(qf);
};

CmdRuler::~CmdRuler()
{
    if (dist_hover_dialog_)
    {
        delete dist_hover_dialog_;
    }
    delete ui_;
    delete info_widget_;
}

void CmdRuler::onCancel()
{
    onDone();
}

void CmdRuler::onDone()
{
    if (dist_hover_dialog_)
    {
        dist_hover_dialog_->hide();
    }
    clearObj();
    rflog::debug("cmd showinfo done");
}

void CmdRuler::on_pb_save_pressed()
{
    // Save
}

void CmdRuler::on_pb_done_pressed()
{
    clearObj();
    rflog::debug("cmd showinfo ok");
    this->done();
}

void CmdRuler::execute()
{
    GApp::instance().addCmdDock(info_widget_, "Ruler"); // TODO: refactory

    setState(RulerState::FirstPick);
}

void CmdRuler::enableSelect()
{
    std::unordered_set<selectflag::Select> select_set{// selectflag::kFace, selectflag::kEdge,
                                                      selectflag::kVertex};
    GApp::instance().setTempSelectMode(select_set, select_set);
}

void CmdRuler::disableSelect()
{
    GApp::instance().popTempSelectMode();
}

void CmdRuler::clearObj()
{
    if (!aux_show_obj_.IsNull())
    {
        ViewerUtils::removeViewObjFromViewer(aux_show_obj_, GApp::instance().getCurrentViewer(),
                                             true);
        aux_show_obj_ = nullptr;
    }
}

void CmdRuler::processVEvent(const rfapp::ViewerEvent &info)
{
    if (info.type_flags == rfapp::LButtonDown)
    {
        switch (state_)
        {
        case RulerState::Init:
        {
            setState(RulerState::FirstPick);
            break;
        }
        case RulerState::FirstPick:
        {
            if (!info.picked_entity)
            {
                break;
            }
            first_pick_owner_ = info.picked_entity;
            last_event_ = info;
            setState(RulerState::SecondPick);

            break;
        }
        case RulerState::SecondPick:
        {
            if (!info.picked_entity)
            {
                break;
            }
            const auto &info_str = processCaculate(info.picked_entity);
            setInfo(info_str);
            setState(RulerState::Init);
            break;
        }
        default: break;
        }
    }
    else if (info.type_flags == rfapp::MouseMove)
    {
        switch (state_)
        {
        case RulerState::SecondPick:
        {
            if (dist_hover_dialog_)
            {
                const rfbase::Pnt2i prev_loc{last_event_.mouse_pos_x, last_event_.mouse_pos_y};
                const rfbase::Pnt2i cur_loc{info.mouse_pos_x, info.mouse_pos_y};
                double dist = sqrt((cur_loc.x - prev_loc.x) * (cur_loc.x - prev_loc.x) +
                                   (cur_loc.y - prev_loc.y) * (cur_loc.y - prev_loc.y));

                auto dist_bar = dist_hover_dialog_->getDistBar(kDist);

                dist_bar->setValue(0, {dist, dist * 1.5, dist * 0.9});
                auto x_dist_bar = dist_hover_dialog_->getDistBar(kDistX);

                double x_dist = cur_loc.x - prev_loc.x;
                x_dist_bar->setValue(0, {x_dist, x_dist * 1.1, x_dist * 0.9});
                auto y_dist_bar = dist_hover_dialog_->getDistBar(kDistY);

                double y_dist = cur_loc.y - prev_loc.y;
                y_dist_bar->setValue(0, {y_dist, y_dist * 1.5, y_dist * 0.75});
                dist_hover_dialog_->updateUI();
            }
            break;
        }
        case RulerState::Init:
        case RulerState::FirstPick:
        default: break;
        }
    }
}

void CmdRuler::setState(RulerState state)
{
    state_ = state;
    switch (state)
    {
    case RulerState::Init:
    {
        if (dist_hover_dialog_)
        {
            dist_hover_dialog_->hide();
        }
        disableSelect();
        break;
    }
    case RulerState::FirstPick:
    {
        clearObj();
        setInfo("press the first point: ");
        enableSelect();
        break;
    }
    case RulerState::SecondPick:
    {
        if (!dist_hover_dialog_)
        {
            REMainWindow *main_win = GApp::instance().getMainWindow();
            dist_hover_dialog_ = new RangeGroup(main_win);
            QWidget *viewer = GApp::instance().getCurrentViewer(); // TODO: refactory
            QPoint pos_in_viewer(last_event_.mouse_pos_x, last_event_.mouse_pos_y);
            QPoint pos_in_mainwindow = viewer->mapToGlobal(pos_in_viewer);
            dist_hover_dialog_->updateLocation(pos_in_mainwindow.x(), pos_in_mainwindow.y());
        }

        size_t i = 0;
        auto d_dist_bar = dist_hover_dialog_->getDistBar(kDist);
        d_dist_bar->setLabel(i, "Dist");
        d_dist_bar->setRange(i, 400, 800);

        auto x_dist_bar = dist_hover_dialog_->getDistBar(kDistX);
        x_dist_bar->setLabel(i, "X");
        x_dist_bar->setRange(i, 200, 300);

        auto y_dist_bar = dist_hover_dialog_->getDistBar(kDistY);
        y_dist_bar->setLabel(i, "Y");
        y_dist_bar->setRange(i, 200, 200);

        dist_hover_dialog_->updateUI();
        dist_hover_dialog_->show();
        setInfo("press the secont point: ");
        break;
    }
    default: break;
    }
}

TopAbs_ShapeEnum CmdRuler::getType(Handle(SelectMgr_EntityOwner) detect_owner)
{
    Handle(StdSelect_BRepOwner) brep_owner = Handle(StdSelect_BRepOwner)::DownCast(detect_owner);
    const TopoDS_Shape &shp = brep_owner->Shape();

    if (shp.IsNull())
    {
        return TopAbs_SHAPE;
    }

    return shp.ShapeType();
}

bool CmdRuler::toPoint(Handle(SelectMgr_EntityOwner) detect_owner, gp_Pnt &point)
{
    Handle(StdSelect_BRepOwner) brep_owner = Handle(StdSelect_BRepOwner)::DownCast(detect_owner);
    const TopoDS_Shape &shp = brep_owner->Shape();

    if (shp.IsNull())
    {
        return false;
    }

    if (shp.ShapeType() == TopAbs_VERTEX)
    {
        const TopoDS_Vertex &vertex_shp = TopoDS::Vertex(shp);
        point = gp_Pnt(BRep_Tool::Pnt(vertex_shp));
        return true;
    }

    return false;
}

std::string CmdRuler::processCaculate(Handle(SelectMgr_EntityOwner) second_pick_obj)
{
    if (!first_pick_owner_ || !second_pick_obj)
    {
        return "";
    }
    if (getType(first_pick_owner_) != TopAbs_VERTEX || getType(second_pick_obj) != TopAbs_VERTEX)
    {
        return "";
    }

    Handle(StdSelect_BRepOwner) brep_owner = Handle(StdSelect_BRepOwner)::DownCast(second_pick_obj);
    const TopoDS_Shape &shp = brep_owner->Shape();

    if (shp.IsNull() || shp.ShapeType() != TopAbs_VERTEX) // TODO: add edge and face support
    {
        return "";
    }

    gp_Pnt first_pnt, second_pnt;
    if (!toPoint(first_pick_owner_, first_pnt) || !toPoint(second_pick_obj, second_pnt))
    {
        return "";
    }

    std::string info;
    info.append("first point: \n[" + rfgeo::GeoDataIO::occTypeToStr(first_pnt) + "]\n");
    info.append("second point: \n[" + rfgeo::GeoDataIO::occTypeToStr(second_pnt) + "]\n");
    info.append("Distance: " + std::to_string(first_pnt.Distance(second_pnt)));

    if (!first_pnt.IsEqual(second_pnt, 1e-6))
    {
        aux_show_obj_ = rfgeo::VMBuilder::createDumbBellViewShape(first_pnt, second_pnt);
        ViewerUtils::addViewObjToViewer(aux_show_obj_, GApp::instance().getCurrentViewer(), true);
    }

    return info;
}

void CmdRuler::setInfo(const std::string &info)
{
    ui_->info_plainedit->setPlainText(QString::fromStdString(info));
    QClipboard *clip = QApplication::clipboard();
    clip->setText(info.c_str());
}

} // namespace rfgui
