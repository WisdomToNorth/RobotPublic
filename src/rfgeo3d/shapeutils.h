
#pragma once
#ifndef SHAPE_UTILS_H
#define SHAPE_UTILS_H

#include <optional>
#include <vector>

#include <Standard_Handle.hxx>
#include <TopLoc_Location.hxx>
#include <TopoDS_Shape.hxx>

class AIS_Shape;
class Bnd_Box;
class Bnd_OBB;
class Geom_Curve;
class Geom_Surface;
class gp_Lin;
class gp_Ax2;
class gp_Pnt;
class gp_Pln;
class gp_Trsf;
class SelectMgr_EntityOwner;
class TopoDS_Compound;
class TopoDS_Edge;
class TopoDS_Face;
class TopoDS_Shape;
class TopoDS_Vertex;

namespace rfgeo
{
class ShapeUtils
{
public:
    static TopoDS_Edge getTrimedCurve(Handle(Geom_Curve) _curve, double _st_param, double _ed_param,
                                      double tol = 1e-6);

    static TopoDS_Edge convertToBSplineEdge(const TopoDS_Edge &edge);

    static std::optional<gp_Pnt> pntProjectOnSurface(Handle(Geom_Surface) _geom_detected_face,
                                                     gp_Pnt _pnt);

    static void obbTransform(Bnd_OBB &output, const gp_Trsf &trsf);

    static Bnd_OBB obbTransformed(const Bnd_OBB &bndObb, const gp_Trsf &trsf);

    static TopoDS_Shape getObbShape(const Bnd_OBB &bndObb);

    static double getBndObbDepth(const Bnd_OBB &bndObb);

    static bool coliAABB(const Bnd_Box &box1, const Bnd_Box &box2);

    static bool edgeAtPlane(const gp_Pln &pln, const TopoDS_Shape &edge);

    static gp_Pnt getBoxCenter(const Bnd_Box &box);

    static Bnd_OBB getOBB(const TopoDS_Shape &shp, double enlarge = 1e-6);

    // input must be leaf node
    static Bnd_OBB getOBB(Handle(AIS_Shape) shp);

    static double getBndOBBArea(const Bnd_OBB &bndObb, double enlarge = 1e-6);

    static TopoDS_Shape triPtShape(const std::vector<gp_Pnt> &triPoints);

    static TopoDS_Shape mergeShps(const std::vector<TopoDS_Shape> &shps);

    static Bnd_Box getAABB(const TopoDS_Shape &shp);

    static Bnd_Box getFullAABB(Handle(AIS_Shape) shp);

    static bool isEqualBox(const Bnd_Box &lhs, const Bnd_Box &rhs, double eps = 1e-6);

    static TopoDS_Shape transformedShape(Handle(AIS_Shape) shp);

    static std::vector<gp_Pnt> getEdgeEnds(const TopoDS_Shape &shp);

    static gp_Pnt planeCtrPt(const TopoDS_Shape &fc);

    static gp_Ax2 getAx2ViaObb(const Bnd_OBB &obb);

    static double cpuCurveLength(const TopoDS_Edge &edge);

    static TopoDS_Shape makeCompound(const std::vector<TopoDS_Shape> &shps);

    static size_t getShapeHash(const TopoDS_Shape &shape);
};

} // namespace rfgeo
#endif
