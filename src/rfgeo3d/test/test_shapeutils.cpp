#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRep_Builder.hxx>
#include <BRep_Tool.hxx>
#include <GeomAbs_CurveType.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <Geom_BSplineCurve.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Edge.hxx>
#include <gp_Pnt.hxx>
#include <gtest/gtest.h>

#include "rfgeo3d/shapeutils.h"
namespace RobotTest
{
using namespace rfgeo;

TEST(geo3d_utils, convertToBSplineEdge)
{
    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(gp_Pnt(0, 0, 0), gp_Pnt(1, 0, 0));
    double _1, _2;
    Handle(Geom_Curve) ori = BRep_Tool::Curve(edge, _1, _2);
    GeomAdaptor_Curve ori_GAC(ori);

    TopoDS_Edge res = ShapeUtils::convertToBSplineEdge(edge);
    Handle(Geom_Curve) res_curve = BRep_Tool::Curve(res, _1, _2);
    GeomAdaptor_Curve res_GAC(res_curve);
    EXPECT_EQ(ori_GAC.GetType(), GeomAbs_CurveType::GeomAbs_Line);
    EXPECT_EQ(res_GAC.GetType(), GeomAbs_CurveType::GeomAbs_BSplineCurve);
}
} // namespace RobotTest
