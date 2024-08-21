#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRep_Builder.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <Geom_Line.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Wire.hxx>
#include <gp_Circ.hxx>
#include <gp_Dir.hxx>
#include <gp_Lin.hxx>
#include <gp_Pln.hxx>
#include <gp_Pnt.hxx>
#include <gtest/gtest.h>

#include "rfgeo3d/geoutils.h"

namespace RobotTest
{
using namespace rfgeo;

TEST(geo3d_utils, testscalept)
{
    gp_Pnt p1{1.0, 2.0, 3.0};
    gp_Pnt expect_res{1.5, 3.0, 4.5};
    bool res = GeoUtils::scalePt(p1, 1.5).IsEqual(expect_res, 1e-6);
    EXPECT_TRUE(res);
}

TEST(geo3d_utils, testpulsminuspt)
{
    gp_Pnt p1{1.0, 2.0, 3.0};
    gp_Pnt p2{1.5, 3.0, 4.5};
    gp_Pnt expect_res{2.5, 5.0, 7.5};
    gp_Pnt expect_res_with_eps{2.5, 5.0, 7.49999};
    EXPECT_TRUE(GeoUtils::plusPt(p1, p2).IsEqual(expect_res, 1e-6));
    EXPECT_TRUE(GeoUtils::plusPt(p1, p2).IsEqual(expect_res_with_eps, 0.1));
    EXPECT_FALSE(GeoUtils::plusPt(p1, p2).IsEqual(expect_res_with_eps, 1e-6));
}

TEST(geo3d_utils, equal_trsf)
{
    gp_Trsf trsf1;
    gp_Pnt p1{1.0, 2.0, 3.0};
    gp_Pnt p2{1.5, 3.0, 4.5};
    trsf1.SetTranslation(p1, p2);

    gp_Trsf trsf2;
    trsf2.SetTranslation(p2, p1);

    EXPECT_TRUE(GeoUtils::isEqualTrsf(trsf1, trsf2.Inverted()));
}

TEST(geo3d_utils, two_tri_colli_1)
{
    std::vector<gp_Pnt> tri1{{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};

    std::vector<gp_Pnt> tri2{{0.0, 0.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 1.0}};

    std::vector<gp_Pnt> tri3{{5.0, 0.0, 0.0}, {0.0, 5.0, 0.0}, {0.0, 0.0, 5.0}};

    std::vector<gp_Pnt> tri4{{2.0, 2.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 1.0}};

    std::vector<gp_Pnt> tri5{{0.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 1.0}};

    std::vector<gp_Pnt> tri6{{0.5, 0.5, 0.0}, {0.0, 0.5, 0.5}, {0.5, 0.0, 0.5}};

    std::vector<gp_Pnt> tri7{{0.5, 0.5, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 0.0}};

    EXPECT_TRUE(GeoUtils::isTwoTriCollision(tri1, tri2));  // vertical
    EXPECT_FALSE(GeoUtils::isTwoTriCollision(tri2, tri3)); // vertical but not colli
    EXPECT_FALSE(GeoUtils::isTwoTriCollision(tri1, tri3)); // parallel
    EXPECT_FALSE(GeoUtils::isTwoTriCollision(tri1, tri1)); // same
    EXPECT_FALSE(GeoUtils::isTwoTriCollision(tri2, tri4)); // int same face and one Edge same
    EXPECT_FALSE(GeoUtils::isTwoTriCollision(tri2, tri5)); // int different face and one Edge same
    EXPECT_FALSE(GeoUtils::isTwoTriCollision(tri1, tri6)); // tri6 is in tri1
    // int different face and one Edge on the other's face
    EXPECT_TRUE(GeoUtils::isTwoTriCollision(tri1, tri7)); // TODO: result is error.
}

TEST(geo3d_utils, point_project_on_line)
{
    gp_Lin lin(gp_Pnt(0.0, 0, 0), gp_Vec(30.0, 40.0, 0.0));
    gp_Pnt expect_res{6.0, 8.0, 0.0};

    gp_Pnt res_pnt = GeoUtils::pointProjLine(gp_Pnt(6.0, 8.0, 20.0), lin);
    bool res = res_pnt.IsEqual(expect_res, 1e-6);

    EXPECT_TRUE(res);
}

// Test case for pointProjLine function
TEST(geo3d_utils, PointProjLine1)
{
    gp_Pnt pt{2.0, 0.0, 0.0};
    gp_Lin lin{gp_Pnt{0.0, 0.0, 0.0}, gp_Dir{1.0, 0.0, 0.0}};
    gp_Pnt expected{2.0, 0.0, 0.0};

    auto result = GeoUtils::pointProjLine(pt, lin);

    EXPECT_NEAR(result.X(), expected.X(), 1e-6);
    EXPECT_NEAR(result.Y(), expected.Y(), 1e-6);
    EXPECT_NEAR(result.Z(), expected.Z(), 1e-6);
}

TEST(geo3d_utils, PointProjLine2)
{
    gp_Pnt pt{1.0, 1.0, 1.0};
    gp_Lin lin{gp_Pnt{0.0, 0.0, 0.0}, gp_Dir{0.0, 1.0, 0.0}};
    gp_Pnt expected{0.0, 1.0, 0.0};

    auto result = GeoUtils::pointProjLine(pt, lin);

    EXPECT_NEAR(result.X(), expected.X(), 1e-6);
    EXPECT_NEAR(result.Y(), expected.Y(), 1e-6);
    EXPECT_NEAR(result.Z(), expected.Z(), 1e-6);
}

TEST(geo3d_utils, isEqualTrsf)
{
    gp_Trsf trsf1, trsf2;
    trsf1.SetValues(1, 0, 0, 1, 0, 1, 0, 2, 0, 0, 1, 3);
    trsf2.SetValues(1, 0, 0, 1, 0, 1, 0, 2, 0, 0, 1, 3);

    ASSERT_TRUE(GeoUtils::isEqualTrsf(trsf1, trsf2, 1e-6));

    trsf1.SetTranslationPart(gp_Vec(1, 2, 3));
    trsf2.SetTranslationPart(gp_Vec(2, 3, 4));

    ASSERT_FALSE(GeoUtils::isEqualTrsf(trsf1, trsf2, 1e-6));

    trsf1.SetScale(gp_Pnt(0, 0, 0), 2);
    trsf2.SetScale(gp_Pnt(0, 0, 0), 2.1);

    ASSERT_FALSE(GeoUtils::isEqualTrsf(trsf1, trsf2, 1e-6));
}

TEST(geo3d_utils, center_pt_of_circle)
{
    gp_Pnt p1(1, 0, 0);
    gp_Pnt p2(0, M_SQRT1_2, M_SQRT1_2);
    gp_Pnt p3(sqrt(3.0) / 3, sqrt(3.0) / 3, sqrt(3.0) / 3);
    gp_Pnt res_pnt = GeoUtils::getCenterOfCircle({p1, p2, p3});

    bool res = res_pnt.IsEqual(gp_Pnt(), 1e-6);
    EXPECT_TRUE(res);
}

TEST(geo3d_utils, GetNormalByThreePnts_NormalTest)
{
    std::vector<gp_Pnt> pts{{0, 0, 0}, {1, 0, 0}, {0, 1, 0}};
    gp_Dir normal = GeoUtils::getNormalByThreePnts(pts);
    ASSERT_NEAR(normal.X(), 0.0, 1e-6);
    ASSERT_NEAR(normal.Y(), 0.0, 1e-6);
    ASSERT_NEAR(normal.Z(), 1.0, 1e-6);
}

TEST(geo3d_utils, GetNormalByThreePnts_ZeroTest)
{
    std::vector<gp_Pnt> pts{{0, 0, 0}, {1, 0, 0}, {2, 0, 0}};
    ASSERT_ANY_THROW(GeoUtils::getNormalByThreePnts(pts));
}

TEST(geo3d_utils, cpuRot_no_rotation)
{
    gp_Ax2 left(gp_Pnt(0, 0, 0), gp_Dir(1, 0, 0), gp_Dir(0, 1, 0));
    gp_Ax2 right(gp_Pnt(1, 0, 0), gp_Dir(1, 0, 0), gp_Dir(0, 1, 0));

    gp_Trsf actual = GeoUtils::cpuRot(left, right);
    gp_Trsf expected;

    EXPECT_TRUE(GeoUtils::isEqualTrsf(actual, expected, 1e-6));
}

TEST(geo3d_utils, Discrete_LineEdge)
{
    gp_Pnt p1(0, 0, 0), p2(10, 0, 0);
    BRepBuilderAPI_MakeVertex v1(p1), v2(p2);
    BRepBuilderAPI_MakeEdge e(v1.Vertex(), v2.Vertex());

    double distance = 2.0;
    std::vector<std::pair<gp_Pnt, gp_Vec>> res;
    GeoUtils::discreteEdge(e.Edge(), distance, res);

    ASSERT_EQ(res.size(), 6);

    for (size_t i = 0; i < res.size(); ++i)
    {
        ASSERT_NEAR(res[i].first.X(), double(i) * distance, 1e-6);
        ASSERT_NEAR(res[i].first.Y(), 0, 1e-6);
        ASSERT_NEAR(res[i].first.Z(), 0, 1e-6);
        ASSERT_NEAR(res[i].second.X(), 1, 1e-6);
        ASSERT_NEAR(res[i].second.Y(), 0, 1e-6);
        ASSERT_NEAR(res[i].second.Z(), 0, 1e-6);
    }
}

TEST(geo3d_utils, Discrete_Curve)
{
    // Create a circle wire
    gp_Pnt center(0, 0, 0);
    double radius = 1.0;
    BRepBuilderAPI_MakeWire wire_builder;
    wire_builder.Add(BRepBuilderAPI_MakeEdge(gp_Circ(gp_Ax2(center, gp_Dir(0, 0, 1)), radius)));
    TopoDS_Wire wire = wire_builder.Wire();

    // Discretize the curve
    std::vector<std::pair<gp_Pnt, gp_Vec>> result;
    double distance = 0.1;
    for (TopExp_Explorer edge_exp(wire, TopAbs_EDGE); edge_exp.More(); edge_exp.Next())
    {
        const TopoDS_Edge &edge = TopoDS::Edge(edge_exp.Current());
        GeoUtils::discreteEdge(edge, distance, result);
    }

    // Check the result
    ASSERT_EQ(result.size(), static_cast<size_t>(64));
    // 64 points discretized with distance 0.1
    // TODO: length is 6.28. some points are added twice
    for (const auto &point_vec : result)
    {
        ASSERT_NEAR(point_vec.first.Distance(center), radius, 1e-6);
    }
}

TEST(geo3d_utils, twoPlanesIntersectionTest)
{
    // Test case 1
    gp_Pln plane1(gp_Pnt(1, 2, 3), gp_Dir(1, 0, 0));
    gp_Pln plane2(gp_Pnt(1, 2, 3), gp_Dir(0, 1, 0));
    gp_Lin result = GeoUtils::twoPlanesIntersection(plane1, plane2);
    gp_Pnt p1(1, 2, 3);
    gp_Vec v1(0, 0, 1);
    gp_Lin expected(p1, v1);
    ASSERT_TRUE(result.Location().IsEqual(expected.Location(), 1e-6));
    ASSERT_TRUE(result.Direction().IsEqual(expected.Direction(), 1e-6));

    // Test case 2
    gp_Pln plane3(gp_Pnt(0, 0, 0), gp_Dir(1, 0, 0));
    gp_Pln plane4(gp_Pnt(1, 0, 0), gp_Dir(0, 0, 1));
    gp_Lin result2 = GeoUtils::twoPlanesIntersection(plane3, plane4);
    gp_Pnt p2(0, 0, 0);
    gp_Vec v2(0, -1, 0); // CAUTION: parallel to (0,1,0)
    gp_Lin expected2(p2, v2);

    ASSERT_TRUE(result2.Location().IsEqual(expected2.Location(), 1e-6));
    ASSERT_TRUE(result2.Direction().IsEqual(expected2.Direction(), 1e-6));
}

} // namespace RobotTest
