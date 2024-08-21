/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/
#pragma once
#ifndef BREP_UTILS_H
#define BREP_UTILS_H

#include <string>

#include <IMeshTools_Parameters.hxx>
#include <Poly_Triangulation.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>

// the @file annotation is needed for Doxygen to document the free
// functions in this file
/**
 * @file
 * @brief The main entry points of our demo
 */

namespace rfapp
{
class TaskProgress;
}

namespace rfio
{
using OccBRepMeshParameters = IMeshTools_Parameters;

// Provides helper functions for OpenCascade TKBRep library
struct BRepUtils
{
    /**
        @brief Creates a valid and empty TopoDS_Compound shape
        */
    static TopoDS_Compound makeEmptyCompound();

    /**
        @brief Creates a non-geometric TopoDS_Face shape from a given triangulation mesh
        @param mesh  the triangulation mesh to be wrapped
        */
    static TopoDS_Face makeFace(const Handle(Poly_Triangulation) & mesh);

    /**
        @brief Iterates over sub-shapes using a given explorer and executes a function for each
       sub-shape
        @param explorer  the explorer used for iteration
        @param fn  the function to be executed for each sub-shape
        */
    template <typename Function>
    static void forEachSubShape(TopExp_Explorer &explorer, Function fn);

    /**
        @brief Iterates over sub-shapes of a given type in a shape and executes a function for each
       sub-shape
        @param shape  the shape to be explored
        @param shapeType  the type of sub-shapes to be explored
        @param fn  the function to be executed for each sub-shape
        */
    template <typename Function>
    static void forEachSubShape(const TopoDS_Shape &shape, TopAbs_ShapeEnum shapeType, Function fn);

    /**
        @brief Iterates over sub-faces in a shape and executes a function for each sub-face
        @param shape  the shape to be explored
        @param fn  the function to be executed for each sub-face
        */
    template <typename Function>
    static void forEachSubFace(const TopoDS_Shape &shape, Function fn);

    /**
        @brief Checks if a shape type is more complex than another
        @param lhs  the first shape type
        @param rhs  the second shape type
        @return true if lhs is more complex than rhs, false otherwise
        */
    static bool moreComplex(TopAbs_ShapeEnum lhs, TopAbs_ShapeEnum rhs);

    /**
        @brief Computes a hash code for a given shape
        @param shape  the shape for which the hash code is to be computed
        @return the computed hash code
        */
    static int hashCode(const TopoDS_Shape &shape);

    /**
        @brief Serializes a shape into a string representation
        @param shape  the shape to be serialized
        @return the string representation of the shape
        */
    static std::string shapeToString(const TopoDS_Shape &shape);

    /**
        @brief Deserializes a string into a shape object
        @param str  the string to be deserialized
        @return the deserialized shape object
        */
    static TopoDS_Shape shapeFromString(const std::string &str);

    /**
        @brief Checks if an edge relies on a 3D curve or a curve on a surface
        @param edge  the edge to be checked
        @return true if the edge relies on a 3D curve or a curve on a surface, false otherwise
        */
    static bool isGeometric(const TopoDS_Edge &edge);

    /**
        @brief Checks if a face relies on a geometric surface
        @param face  the face to be checked
        @return true if the face relies on a geometric surface, false otherwise
        */
    static bool isGeometric(const TopoDS_Face &face);

    /**
        @brief Computes a mesh representation of a shape using the OpenCascade meshing algorithm
        @param shape  the shape for which the mesh is to be computed
        @param params  the parameters for the OpenCascade meshing algorithm
        @param progress  the progress of the task (optional)
        */
    static void computeMesh(const TopoDS_Shape &shape, const OccBRepMeshParameters &params,
                            rfapp::TaskProgress *progress = nullptr);
};

template <typename Function>
void BRepUtils::forEachSubShape(TopExp_Explorer &explorer, Function fn)
{
    while (explorer.More())
    {
        fn(explorer.Current());
        explorer.Next();
    }
}

template <typename Function>
void BRepUtils::forEachSubShape(const TopoDS_Shape &shape, TopAbs_ShapeEnum shapeType, Function fn)
{
    TopExp_Explorer expl(shape, shapeType);
    BRepUtils::forEachSubShape(expl, std::move(fn));
}

template <typename Function>
void BRepUtils::forEachSubFace(const TopoDS_Shape &shape, Function fn)
{
    for (TopExp_Explorer expl(shape, TopAbs_FACE); expl.More(); expl.Next())
    {
        fn(TopoDS::Face(expl.Current()));
    }
}
} // namespace rfio
#endif // _BREP_UTILS_H_
