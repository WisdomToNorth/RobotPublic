/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#pragma once
#ifndef MATH_UTILS_H
#define MATH_UTILS_H
#include <Poly_Triangulation.hxx>

class gp_XYZ;

namespace rfio
{

// Provides helper functions for mesh and triangle objects
struct MeshUtils
{
    static double triangleSignedVolume(const gp_XYZ &p1, const gp_XYZ &p2, const gp_XYZ &p3);
    static double triangleArea(const gp_XYZ &p1, const gp_XYZ &p2, const gp_XYZ &p3);

    static double triangulationVolume(const Handle_Poly_Triangulation &triangulation);
    static double triangulationArea(const Handle_Poly_Triangulation &triangulation);

    using Poly_Triangulation_NormalType = gp_Vec3f;

    static void setNode(const Handle_Poly_Triangulation &triangulation, int index,
                        const gp_Pnt &pnt);
    static void setTriangle(const Handle_Poly_Triangulation &triangulation, int index,
                            const Poly_Triangle &triangle);
    static void setNormal(const Handle_Poly_Triangulation &triangulation, int index,
                          const Poly_Triangulation_NormalType &n);
    static void allocateNormals(const Handle_Poly_Triangulation &triangulation);

    static const Poly_Array1OfTriangle &triangles(const Handle_Poly_Triangulation &triangulation)
    {
        return triangulation->InternalTriangles();
    }

    enum class Orientation
    {
        Unknown,
        Clockwise,
        CounterClockwise
    };

    class AdaptorPolyline2d
    {
    public:
        virtual ~AdaptorPolyline2d() = default;
        virtual gp_Pnt2d pointAt(int index) const = 0;
        virtual int pointCount() const = 0;
        virtual bool empty() const
        {
            return this->pointCount() <= 0;
        }
    };

    class AdaptorPolyline3d
    {
    public:
        virtual ~AdaptorPolyline3d() = default;
        virtual const gp_Pnt &pointAt(int i) const = 0;
        virtual int pointCount() const = 0;
        virtual int empty() const
        {
            return this->pointCount() <= 0;
        }
    };

    static Orientation orientation(const AdaptorPolyline2d &polyline);
    static gp_Vec directionAt(const AdaptorPolyline3d &polyline, int i);
};

} // namespace rfio
#endif
