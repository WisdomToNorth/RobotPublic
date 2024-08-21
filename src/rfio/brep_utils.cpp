/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#include "brep_utils.h"

#include <climits>
#include <sstream>

#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepTools.hxx>
#include <BRep_Builder.hxx>
#include <BRep_Tool.hxx>
#include <Message_ProgressIndicator.hxx>
#include <TopoDS_Compound.hxx>

#include "rfgeo3d/shapeutils.h"

#include "occ_progress_indicator.h"
#include "tkernel_utils.h"

namespace rfio
{

TopoDS_Compound BRepUtils::makeEmptyCompound()
{
    const BRep_Builder builder;
    TopoDS_Compound comp;
    builder.MakeCompound(comp);
    return comp;
}

TopoDS_Face BRepUtils::makeFace(const Handle(Poly_Triangulation) & mesh)
{
    TopoDS_Face face;
    const BRep_Builder builder;
    builder.MakeFace(face);
    builder.UpdateFace(face, mesh);
    return face;
}

bool BRepUtils::moreComplex(TopAbs_ShapeEnum lhs, TopAbs_ShapeEnum rhs)
{
    return lhs < rhs;
}

int BRepUtils::hashCode(const TopoDS_Shape &shape)
{
    if (shape.IsNull())
    {
        return -1;
    }
    size_t hash = rfgeo::ShapeUtils::getShapeHash(shape);
    return static_cast<int>(hash);
}

std::string BRepUtils::shapeToString(const TopoDS_Shape &shape)
{
    std::ostringstream oss(std::ios_base::out);
    BRepTools::Write(shape, oss);
    return oss.str();
}

TopoDS_Shape BRepUtils::shapeFromString(const std::string &str)
{
    TopoDS_Shape shape;
    const BRep_Builder brepBuilder;
    std::istringstream iss(str, std::ios_base::in);
    BRepTools::Read(shape, iss, brepBuilder);
    return shape;
}

bool BRepUtils::isGeometric(const TopoDS_Edge &edge)
{
    return BRep_Tool::IsGeometric(edge);
}

bool BRepUtils::isGeometric(const TopoDS_Face &face)
{
    return BRep_Tool::IsGeometric(face);
}

void BRepUtils::computeMesh(const TopoDS_Shape &shape, const OccBRepMeshParameters &params,
                            rfapp::TaskProgress *progress)
{
    const Handle_Message_ProgressIndicator indicator = new OccProgressIndicator(progress);
    [[maybe_unused]] BRepMesh_IncrementalMesh mesher(shape, params,
                                                     Message_ProgressIndicator::Start(indicator));
}
} // namespace rfio
