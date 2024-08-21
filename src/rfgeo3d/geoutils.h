
#pragma once
#ifndef GEO_UTILS_H
#define GEO_UTILS_H

#include <vector>

#include <Bnd_OBB.hxx>
#include <Precision.hxx>
#include <gp_Pnt.hxx>
#include <gp_Trsf.hxx>

class Quantity_Color;
class gp_Pln;
class TopoDS_Edge;
class gp_Lin;
class Bnd_Box;

namespace rfgeo
{

class GeoUtils
{
public:
    static gp_Pnt scalePt(const gp_Pnt &p1, double scale);

    static gp_Pnt minusPt(const gp_Pnt &p1, const gp_Pnt &p2);

    static gp_Pnt plusPt(const gp_Pnt &p1, const gp_Pnt &p2);

    static bool isEqualTrsf(const gp_Trsf &t1, const gp_Trsf &t2,
                            double eps = Precision::Confusion());

    /**
     * Determines whether a ray intersects with a triangle.
     *
     * @param tri_points The three vertices of the triangle.
     * @param st The starting point of the ray.
     * @param ed The ending point of the ray.
     * @return Whether the ray intersects with the triangle.
     */
    static bool rayInTriangle(const std::vector<gp_Pnt> &tri_points, const gp_Pnt &st,
                              const gp_Pnt &ed);

    // boundary collision is NOT considered as collision
    static bool isTwoTriCollision(const std::vector<gp_Pnt> &_tripoints1,
                                  const std::vector<gp_Pnt> &_tripoints2);

    static void triPntsTransformed(const std::vector<gp_Pnt> &input, const gp_Trsf &trsf,
                                   std::vector<gp_Pnt> &output);

    static double caculateTriangleArea(const std::vector<gp_Pnt> &triPoints);

    // euler is in Radian system
    static gp_Vec eulerToVec(const gp_Pnt &euler, const gp_Pnt &loc);
    static gp_Ax2 eulerToAx2(const gp_Pnt &euler, const gp_Pnt &loc);

    static gp_Trsf cpuRotZDirection(const gp_Ax2 &ax2_left, const gp_Ax2 &ax2_right);

    // From left to right
    static gp_Trsf cpuRot(const gp_Ax2 &ax2_left, const gp_Ax2 &ax2_right);

    static gp_Trsf cpuTrsf(const gp_Ax2 &ax2_left, const gp_Ax2 &ax2_right);

    static gp_Pnt cpuEuler(const gp_Ax2 &ax2_0, const gp_Ax2 &ax2_1);

    static void discreteEdge(const TopoDS_Edge &edge, double distance,
                             std::vector<std::pair<gp_Pnt, gp_Vec>> &res);

    static gp_Pnt getCenterOfCircle(const std::vector<gp_Pnt> &points);

    static gp_Pnt pointProjLine(const gp_Pnt &pt, const gp_Lin &lin);

    static gp_Lin twoPlanesIntersection(const gp_Pln &plane1, const gp_Pln &plane2);

    static gp_Pnt newPointOnDir(const gp_Pnt &begin_point, const gp_Dir &dir, double length);

    static gp_Pnt getCenterOfPnts(const std::vector<gp_Pnt> &ori_pts);

    static gp_Vec getNormalByThreePnts(const std::vector<gp_Pnt> &three_pts);

    // Returns linear position of a plane along its normal direction
    static double planePosition(const gp_Pln &plane);

    static Bnd_OBB getPntsObb(const std::vector<gp_Pnt> &_pnts);
};
} // namespace rfgeo
#endif
