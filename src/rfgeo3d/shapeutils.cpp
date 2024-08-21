
#include "shapeutils.h"

#include "stdafx.h"

#include <ostream>

#include "geoutils.h"

namespace rfgeo
{

TopoDS_Edge ShapeUtils::getTrimedCurve(Handle(Geom_Curve) _curve, double _st_param,
                                       double _ed_param, double tol)
{
    Handle(Adaptor3d_Curve) hcurve_adapt =
        GeomAdaptor_Curve(_curve).Trim(_st_param, _ed_param, tol);

    // TODO: 10000 is a magic number
    GeomConvert_ApproxCurve c1(hcurve_adapt, tol, GeomAbs_Shape::GeomAbs_C0, 10000,
                               /*max degree*/ 360);

    GeomConvert_CompCurveToBSplineCurve resme;
    resme.Add(c1.Curve(), tol);

    BRepLib_MakeEdge _edge;
    _edge.Init(resme.BSplineCurve());
    _edge.Build();
    return TopoDS_Edge(_edge);
}

TopoDS_Edge ShapeUtils::convertToBSplineEdge(const TopoDS_Edge &edge)
{
    double st_para_, ed_para_;
    auto curve_ = BRep_Tool::Curve(edge, st_para_, ed_para_);
    GeomAdaptor_Curve ori_GAC(curve_);
    return getTrimedCurve(curve_, st_para_, ed_para_, 1e-6);
}

std::optional<gp_Pnt> ShapeUtils::pntProjectOnSurface(Handle(Geom_Surface) l_geom_detected_face,
                                                      gp_Pnt _pnt)
{
    GeomAPI_ProjectPointOnSurf projector;
    projector.Init(_pnt, l_geom_detected_face);
    const Standard_Integer nb_sol = projector.NbPoints();

    if (nb_sol)
        return projector.NearestPoint();
    else
        return std::nullopt;
}

bool ShapeUtils::isEqualBox(const Bnd_Box &lhs, const Bnd_Box &rhs, double eps)
{
    return lhs.CornerMin().IsEqual(rhs.CornerMin(), eps) &&
           lhs.CornerMax().IsEqual(rhs.CornerMax(), eps);
}

void ShapeUtils::obbTransform(Bnd_OBB &output, const gp_Trsf &trsf)
{
    gp_Ax3 ax3(output.Center(), output.ZDirection(), output.XDirection());
    ax3.Transform(trsf);

    output.SetXComponent(ax3.XDirection(), output.XHSize());
    output.SetYComponent(ax3.YDirection(), output.YHSize());
    output.SetZComponent(ax3.Direction(), output.ZHSize());
    output.SetCenter(ax3.Location());
}

Bnd_OBB ShapeUtils::obbTransformed(const Bnd_OBB &bndObb, const gp_Trsf &trsf)
{
    Bnd_OBB bndObb_return;
    gp_Ax3 ax3(bndObb.Center(), bndObb.ZDirection(), bndObb.XDirection());
    ax3.Transform(trsf);

    bndObb_return.SetXComponent(ax3.XDirection(), bndObb.XHSize());
    bndObb_return.SetYComponent(ax3.YDirection(), bndObb.YHSize());
    bndObb_return.SetZComponent(ax3.Direction(), bndObb.ZHSize());
    bndObb_return.SetCenter(ax3.Location());

    return bndObb_return;
}

TopoDS_Shape ShapeUtils::getObbShape(const Bnd_OBB &bndObb)
{
    Bnd_OBB bndObbt = bndObb;
    bndObbt.Enlarge(0.1);

    gp_Pnt vertex[8];
    bndObbt.GetVertex(vertex);

    BRepBuilderAPI_MakeWire aMakeWire;
    TopoDS_Edge anEdge = BRepBuilderAPI_MakeEdge(vertex[0], vertex[1]);
    aMakeWire.Add(anEdge);
    TopoDS_Edge anEdge2 = BRepBuilderAPI_MakeEdge(vertex[1], vertex[3]);
    aMakeWire.Add(anEdge2);
    TopoDS_Edge anEdge3 = BRepBuilderAPI_MakeEdge(vertex[3], vertex[2]);
    aMakeWire.Add(anEdge3);
    TopoDS_Edge anEdge4 = BRepBuilderAPI_MakeEdge(vertex[2], vertex[0]);
    aMakeWire.Add(anEdge4);

    gp_Vec gpVec(bndObbt.ZDirection());
    gpVec = gpVec * bndObbt.ZHSize() * 2;

    TopoDS_Wire aWire = aMakeWire.Wire();
    TopoDS_Face aFaceRect = BRepBuilderAPI_MakeFace(aWire);
    TopoDS_Shape BOX = BRepPrimAPI_MakePrism(aFaceRect, gpVec);
    return BOX;
}

double ShapeUtils::getBndObbDepth(const Bnd_OBB &bndObb)
{
    gp_Pnt vertex[8];
    bndObb.GetVertex(vertex);

    double minZ = vertex[0].Z();
    for (const gp_Pnt &v : vertex)
    {
        minZ = v.Z() < minZ ? v.Z() : minZ;
    }
    return minZ;
}

bool ShapeUtils::coliAABB(const Bnd_Box &box1, const Bnd_Box &box2)
{
    gp_Pnt box1_min = box1.CornerMin();
    gp_Pnt box1_max = box1.CornerMax();
    gp_Pnt box2_min = box2.CornerMin();
    gp_Pnt box2_max = box2.CornerMax();

    if (box1_min.X() > box2_max.X() || box1_max.X() < box2_min.X() || box1_min.Y() > box2_max.Y() ||
        box1_max.Y() < box2_min.Y() || box1_min.Z() > box2_max.Z() || box1_max.Z() < box2_min.Z())
        return false;

    return true;
}

gp_Pnt ShapeUtils::getBoxCenter(const Bnd_Box &box)
{
    return gp_Pnt((box.CornerMin().X() + box.CornerMax().X()) / 2,
                  (box.CornerMin().Y() + box.CornerMax().Y()) / 2,
                  (box.CornerMin().Z() + box.CornerMax().Z()) / 2);
}

Bnd_OBB ShapeUtils::getOBB(const TopoDS_Shape &shp, double EnlargeGap)
{
    Bnd_OBB box_obb;
    BRepBndLib ret;
    ret.AddOBB(shp, box_obb, true, true, true);
    box_obb.Enlarge(EnlargeGap);
    return box_obb;
}

Bnd_OBB ShapeUtils::getOBB(Handle(AIS_Shape) shp)
{
    Bnd_OBB obb = getOBB(shp->Shape());
    gp_Trsf temptrsf = shp->Transformation();
    obbTransform(obb, temptrsf);
    return obb;
}

double ShapeUtils::getBndOBBArea(const Bnd_OBB &bndObb, double enlarge)
{
    enlarge += 0.0005;
    if (bndObb.XHSize() < enlarge || bndObb.YHSize() < enlarge || bndObb.ZHSize() < enlarge)
    {
        double area1 = bndObb.XHSize() * bndObb.YHSize() * 4;
        double area2 = bndObb.XHSize() * bndObb.ZHSize() * 4;
        double area3 = bndObb.YHSize() * bndObb.ZHSize() * 4;
        double area = std::max(area1, area2);
        return std::max(area, area3);
    }
    else
    {
        double area1 = bndObb.XHSize() * bndObb.YHSize() * 4;
        double area2 = bndObb.XHSize() * bndObb.ZHSize() * 4;
        double area3 = bndObb.YHSize() * bndObb.ZHSize() * 4;

        double area = (area1 + area2 + area3) * 2;
        return area;
    }
}

TopoDS_Shape ShapeUtils::triPtShape(const std::vector<gp_Pnt> &triPoints)
{
    TopoDS_Edge e1 = BRepBuilderAPI_MakeEdge(triPoints[0], triPoints[1]);
    TopoDS_Edge e2 = BRepBuilderAPI_MakeEdge(triPoints[1], triPoints[2]);
    TopoDS_Edge e3 = BRepBuilderAPI_MakeEdge(triPoints[2], triPoints[0]);

    TopoDS_Wire aWire = BRepBuilderAPI_MakeWire(e1, e2, e3);
    TopoDS_Face Face = BRepBuilderAPI_MakeFace(aWire);
    return Face;
}

Bnd_Box ShapeUtils::getAABB(const TopoDS_Shape &shp)
{
    Bnd_Box box_b;
    BRepBndLib ret;
    box_b.SetGap(0.001);
    ret.AddOptimal(shp, box_b, Standard_True, true);
    return box_b;
}

Bnd_Box ShapeUtils::getFullAABB(Handle(AIS_Shape) shp)
{
    Bnd_Box t_box;
    if (shp->Children().Size() > 0)
    {
        for (PrsMgr_ListOfPresentableObjectsIter i(shp->Children()); i.More(); i.Next())
        {
            Handle(PrsMgr_PresentableObject) obj = i.Value();
            if (obj->IsKind(STANDARD_TYPE(AIS_Shape)))
            {
                Handle(AIS_Shape) child_model = Handle(AIS_Shape)::DownCast(obj);
                Bnd_Box child_box = getFullAABB(child_model);
                t_box.Add(child_box);
            }
        }
    }
    else
    {
        if (!shp->Shape().IsNull())
        {
            t_box = shp->BoundingBox();
            t_box = t_box.Transformed(shp->Transformation());
        }
    }
    return t_box;
}

TopoDS_Shape ShapeUtils::transformedShape(Handle(AIS_Shape) shp)
{
    const auto &tp_shp = shp->Shape();
    return BRepBuilderAPI_Transform(tp_shp, shp->Transformation()).Shape();
}

std::vector<gp_Pnt> ShapeUtils::getEdgeEnds(const TopoDS_Shape &shp)
{
    std::vector<gp_Pnt> res;
    double first, last;

    Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(shp), first, last);
    if (curve)
    {
        gp_Pnt pt1 = curve->Value(first);
        gp_Pnt pt2 = curve->Value(last);
        res.push_back(pt1);
        res.push_back(pt2);
    }
    return res;
}

gp_Pnt ShapeUtils::planeCtrPt(const TopoDS_Shape &fc)
{
    auto obb = getOBB(fc);
    return obb.Center();
}

bool ShapeUtils::edgeAtPlane(const gp_Pln &pln, const TopoDS_Shape &edge)
{
    std::vector<gp_Pnt> endPts = getEdgeEnds(edge);
    return pln.Distance(endPts[0]) < 1e-7 && pln.Distance(endPts[1]) < 1e-7;
}

TopoDS_Shape ShapeUtils::mergeShps(const std::vector<TopoDS_Shape> &shps)
{
    TopoDS_Compound compound;
    BRep_Builder l_build_tool;
    l_build_tool.MakeCompound(compound);
    for (const auto &s : shps)
    {
        l_build_tool.Add(compound, s);
    }

    return TopoDS_Shape(compound);
}

gp_Ax2 ShapeUtils::getAx2ViaObb(const Bnd_OBB &obb)
{
    gp_Ax2 l_ax2;
    double minD = Min(obb.XHSize(), obb.YHSize());
    minD = Min(minD, obb.ZHSize());

    if (minD == obb.XHSize())
    {
        l_ax2 = gp_Ax2(obb.Center(), obb.XDirection(), obb.YDirection());
    }
    else if (minD == obb.YHSize())
    {
        l_ax2 = gp_Ax2(obb.Center(), obb.YDirection(), obb.ZDirection());
    }
    else if (minD == obb.ZHSize())
    {
        l_ax2 = gp_Ax2(obb.Center(), obb.ZDirection(), obb.XDirection());
    }
    return l_ax2;
}

double ShapeUtils::cpuCurveLength(const TopoDS_Edge &edge)
{
    double st_para_, ed_para_;
    auto curve_ = BRep_Tool::Curve(edge, st_para_, ed_para_);
    GeomAdaptor_Curve ori_GAC(curve_);
    if (ori_GAC.GetType() == GeomAbs_Line)
    {
        std::cout << "Curve type: line\n";
        gp_Pnt p1, p2;
        gp_Vec v1, v2;
        curve_->D1(st_para_, p1, v1);
        curve_->D1(ed_para_, p2, v2);
        return p1.Distance(p2);
    }
    else // for other curve, convert to BSpine
    {
        GeomAdaptor_Curve GAC(BRep_Tool::Curve(convertToBSplineEdge(edge), st_para_, ed_para_));
        double length = GCPnts_AbscissaPoint::Length(GAC);
        return length;
    }
}

TopoDS_Shape ShapeUtils::makeCompound(const std::vector<TopoDS_Shape> &shps)
{
    TopoDS_Compound t_compound;
    BRep_Builder l_build_tool;
    l_build_tool.MakeCompound(t_compound);
    for (auto &shp : shps)
    {
        l_build_tool.Add(t_compound, shp);
    }
    return t_compound;
}

size_t ShapeUtils::getShapeHash(const TopoDS_Shape &shape)
{
#if OCC_VERSION_HEX >= 0x070800
    std::hash<TopoDS_Shape> hasher;
    return hasher(shape);
#else
    return shape.HashCode(INT_MAX);
#endif
}
}; // namespace rfgeo
