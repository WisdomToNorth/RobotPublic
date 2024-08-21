#include "cmd_showinfo.h"

#include <unordered_set>

#include <QClipboard>
#include <QObject>
#include <QPlainTextEdit>
#include <QVBoxLayout>

#include <AIS_ColoredShape.hxx>
#include <AIS_Shape.hxx>
#include <AIS_TextLabel.hxx>
#include <BRepGProp.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRep_Builder.hxx>
#include <BRep_Tool.hxx>
#include <StdSelect_BRepOwner.hxx>
#include <TopoDS.hxx>

#include "rfgeo3d/geodataio.h"
#include "rfgeo3d/shapeutils.h"
#include "rfgeo3d/vmselectmode.h"
#include "rfgeo3d/vmutils.h"
#include "rflog/rfLog.h"

#include "app_flag.h"
#include "app_system.h"
#include "ui_cmd_showinfo.h"

namespace rfgui
{
CmdShowInfo::CmdShowInfo()
    : CmdBase(CmdType::Interactive)
{
    ui_ = new Ui::InfowidgetUi();
    info_widget_ = new QWidget();
    ui_->setupUi(info_widget_);

    connect(ui_->pb_save, &QPushButton::clicked, this, &CmdShowInfo::on_pb_save_pressed);
    connect(ui_->pb_done, &QPushButton::clicked, this, &CmdShowInfo::on_pb_done_pressed);

    QFont qf;
    qf.setFamily("Consolas");
    qf.setPointSize(12);
    ui_->info_plainedit->setFont(qf);
};

CmdShowInfo::~CmdShowInfo()
{
    delete ui_;
    delete info_widget_;
}

void CmdShowInfo::on_pb_save_pressed()
{
    // Save
}

void CmdShowInfo::on_pb_done_pressed()
{
    rflog::debug("cmd showinfo ok");
    this->done();
}

void CmdShowInfo::execute()
{
    GApp::instance().addCmdDock(info_widget_, "Show Info"); // TODO: refactory
    std::unordered_set<selectflag::Select> select_set{selectflag::kFace, selectflag::kEdge,
                                                      selectflag::kVertex, selectflag::kSolid};
    GApp::instance().setTempSelectMode(select_set, select_set);
}

void CmdShowInfo::setInfo(const std::string &info)
{
    ui_->info_plainedit->setPlainText(QString::fromStdString(info));
    QClipboard *clip = QApplication::clipboard();
    clip->setText(info.c_str());
}

void CmdShowInfo::processVEvent(const rfapp::ViewerEvent &info)
{
    if (info.type_flags & rfapp::LButtonDown)
    {
        if (info.picked_entity.IsNull())
        {
            return;
        }
        handleModelInfo(info.picked_entity);
    }
}

void CmdShowInfo::handleModelInfo(Handle(SelectMgr_EntityOwner) detect_owner_)
{
    Handle(StdSelect_BRepOwner) slt_owner = Handle(StdSelect_BRepOwner)::DownCast(detect_owner_);
    if (slt_owner.IsNull())
    {
        return;
    }

    switch (slt_owner->Shape().ShapeType())
    {
    case TopAbs_SOLID:
    {
        handleSolidInfo(detect_owner_);
        break;
    }
    case TopAbs_FACE:
    {
        handleFaceInfo(detect_owner_);
        break;
    }
    case TopAbs_EDGE:
    {
        handleEdgeInfo(detect_owner_);
        break;
    }
    case TopAbs_VERTEX:
    {
        handlePtInfo(detect_owner_);
        break;
    }
    case TopAbs_SHELL:
    case TopAbs_WIRE:
    case TopAbs_COMPOUND:
    case TopAbs_COMPSOLID:
    case TopAbs_SHAPE:
    {
        break;
    }
    }
}

void CmdShowInfo::handlePtInfo(Handle(SelectMgr_EntityOwner) detect_owner_)
{
    const TopoDS_Shape &S = Handle(StdSelect_BRepOwner)::DownCast(detect_owner_)->Shape();
    if (S.ShapeType() != TopAbs_VERTEX)
    {
        return;
    }
    const TopoDS_Vertex &shp = TopoDS::Vertex(S);
    gp_Pnt press_pt = BRep_Tool::Pnt(shp);
    press_pt.Transform(detect_owner_->Selectable()->Transformation());

    std::string info("Vertex\n\n");
    info.append("1. loc: \n[" + rfgeo::GeoDataIO::occTypeToStr(press_pt) + "]\n");
    info.append("\n2. Transfrom: \n" +
                rfgeo::GeoDataIO::occTypeToStr(detect_owner_->Selectable()->Transformation()) +
                '\n');
    info.append("\n3. OccData: \n" + rfgeo::GeoDataIO::occTypeToStr(press_pt) + '\n');
    setInfo(info);
}

void CmdShowInfo::handleEdgeInfo(Handle(SelectMgr_EntityOwner) detect_owner_)
{
    const TopoDS_Shape &S = Handle(StdSelect_BRepOwner)::DownCast(detect_owner_)->Shape();
    if (S.ShapeType() != TopAbs_EDGE)
    {
        return;
    }
    TopoDS_Edge shp = TopoDS::Edge(S);

    std::string info("Edge\n\n");

    info.append("1. length: " + std::to_string(rfgeo::ShapeUtils::cpuCurveLength(shp)) + '\n');
    info.append("\n2. transform: \n" +
                rfgeo::GeoDataIO::occTypeToStr(detect_owner_->Selectable()->Transformation()) +
                '\n');
    setInfo(info);
}

void CmdShowInfo::handleFaceInfo(Handle(SelectMgr_EntityOwner) detect_owner_)
{
    const TopoDS_Shape &S = Handle(StdSelect_BRepOwner)::DownCast(detect_owner_)->Shape();
    if (S.ShapeType() != TopAbs_FACE)
    {
        return;
    }
    const TopoDS_Face &shp = TopoDS::Face(S);

    std::string info("Face\n\n");

    const Handle(Geom_Surface) l_geom_detected_face = Handle(Geom_Surface)::DownCast(
        BRep_Tool::Surface(shp)->Transformed(detect_owner_->Selectable()->Transformation()));

    auto obb = rfgeo::ShapeUtils::getOBB(shp);
    rfgeo::ShapeUtils::obbTransform(obb, detect_owner_->Selectable()->Transformation());

    gp_Ax2 temp_ax2 = rfgeo::ShapeUtils::getAx2ViaObb(obb);
    temp_ax2.SetLocation(rfgeo::ShapeUtils::pntProjectOnSurface(l_geom_detected_face, obb.Center())
                             .value_or(gp_Pnt()));

    info.append("1. centerPt: \n[" + rfgeo::GeoDataIO::occTypeToStr(obb.Center()) + "] \n");
    info.append("\n2. normal: \n" + rfgeo::GeoDataIO::occTypeToStr(temp_ax2) + '\n');
    setInfo(info);
}

void CmdShowInfo::handleSolidInfo(Handle(SelectMgr_EntityOwner) detect_owner_)
{
    Handle(AIS_ColoredShape) ais_shp =
        Handle(AIS_ColoredShape)::DownCast(detect_owner_->Selectable());

    std::string info;
    info.append("Solid\n\n");

    info.append("\n1. centerPt(ori): \n[" +
                rfgeo::GeoDataIO::occTypeToStr(rfgeo::VMUtils::getObbCenterPnt(ais_shp)) + "]\n");
    info.append("\n2. Transformation: \n" +
                rfgeo::GeoDataIO::occTypeToStr(ais_shp->Transformation()) + '\n');

    Quantity_Color color;
    ais_shp->Color(color);
    info.append("\n3. Color: \n" + rfgeo::GeoDataIO::occTypeToStr(color) + '\n');
    setInfo(info);
}
} // namespace rfgui
