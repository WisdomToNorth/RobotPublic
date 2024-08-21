
#pragma once
#ifndef K_SHAPE_BUILDER_H
#define K_SHAPE_BUILDER_H

#include <vector>

#include <TopoDS_Shape.hxx>
#include <gp_Pnt.hxx>

namespace rfdb
{
class dbPart;
class ShapeBuilder
{
public:
    // Free to change this function
    static void makeTestShapes(const double myWidth, const double myThickness,
                               std::vector<TopoDS_Shape> &res);

    static TopoDS_Shape makeBottle(const double myWidth = 80, const double myHeight = 80,
                                   const double myThickness = 30);

    static TopoDS_Shape makeCubePlusQuarterBall(double _dx = 50, double _dy = 60, double _dz = 80);

    static TopoDS_Shape makeBall(gp_Pnt loc, double R = 40);

    static dbPart *createDumbBellVariant(const gp_Pnt &p1, const gp_Pnt &p2,
                                         double l_w_scale = 100.0);

    static TopoDS_Shape arrowBuilder(gp_Pnt p1, gp_Pnt p2, double arrow_scale = 0.1);

    static dbPart *buildAbassica(const gp_Ax2 &_loc, bool with_face, double _size);

private:
    static gp_Pnt plusPt(const gp_Pnt &p1, const gp_Pnt &p2);

    static void createFace(dbPart *part, const gp_Ax2 &_loc, double _size);
};
} // namespace rfdb
#endif
