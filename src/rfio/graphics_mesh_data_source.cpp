/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#include "graphics_mesh_data_source.h"

#include <Precision.hxx>
#include <Standard_Type.hxx>
#include <TColStd_DataMapOfIntegerInteger.hxx>
#include <TColStd_DataMapOfIntegerReal.hxx>
#include <TColgp_SequenceOfXYZ.hxx>

#include "mesh_utils.h"

namespace rfio
{
Standard_Address GraphicsMeshDataSource::GetAddr(const int /*ID*/, const bool /*IsElement*/) const
{
    return nullptr;
}
const TColStd_PackedMapOfInteger &GraphicsMeshDataSource::GetAllNodes() const
{
    return nodes_;
}
const TColStd_PackedMapOfInteger &GraphicsMeshDataSource::GetAllElements() const
{
    return elements_;
}

GraphicsMeshDataSource::GraphicsMeshDataSource(const Handle_Poly_Triangulation &mesh)
    : mesh_(mesh)
{
    if (!mesh_.IsNull())
    {
        const int lenCoords = mesh_->NbNodes();
        node_coords_ = new TColStd_HArray2OfReal(1, lenCoords, 1, 3);

        for (int i = 1; i <= lenCoords; ++i)
        {
            nodes_.Add(i);
            const gp_XYZ xyz = mesh_->Node(i).XYZ();
            node_coords_->SetValue(i, 1, xyz.X());
            node_coords_->SetValue(i, 2, xyz.Y());
            node_coords_->SetValue(i, 3, xyz.Z());
        }

        const Poly_Array1OfTriangle &aSeq = MeshUtils::triangles(mesh_);
        const int lenTriangles = aSeq.Length();
        elem_normals_ = new TColStd_HArray2OfReal(1, lenTriangles, 1, 3);
        elemnodes_ = new TColStd_HArray2OfInteger(1, lenTriangles, 1, 3);

        for (int i = 1; i <= lenTriangles; ++i)
        {
            elements_.Add(i);
            const Poly_Triangle &aTri = aSeq(i);

            int V[3];
            aTri.Get(V[0], V[1], V[2]);

            const gp_Vec aV1(mesh_->Node(V[0]), mesh_->Node(V[1]));
            const gp_Vec aV2(mesh_->Node(V[1]), mesh_->Node(V[2]));

            gp_Vec aN = aV1.Crossed(aV2);
            if (aN.SquareMagnitude() > Precision::SquareConfusion())
                aN.Normalize();
            else
                aN.SetCoord(0.0, 0.0, 0.0);

            for (int j = 0; j < 3; ++j)
                elemnodes_->SetValue(i, j + 1, V[j]);

            elem_normals_->SetValue(i, 1, aN.X());
            elem_normals_->SetValue(i, 2, aN.Y());
            elem_normals_->SetValue(i, 3, aN.Z());
        }
    }
}

bool GraphicsMeshDataSource::GetGeom(const int ID, const bool IsElement,
                                     TColStd_Array1OfReal &Coords, int &NbNodes,
                                     MeshVS_EntityType &Type) const
{
    if (mesh_.IsNull())
        return false;

    if (IsElement)
    {
        if (ID >= 1 && ID <= elements_.Extent())
        {
            Type = MeshVS_ET_Face;
            NbNodes = 3;
            for (int i = 1, k = 1; i <= 3; ++i)
            {
                const int IdxNode = elemnodes_->Value(ID, i);
                for (int j = 1; j <= 3; ++j, ++k)
                    Coords(k) = node_coords_->Value(IdxNode, j);
            }

            return true;
        }

        return false;
    }
    else
    {
        if (ID >= 1 && ID <= nodes_.Extent())
        {
            Type = MeshVS_ET_Node;
            NbNodes = 1;

            Coords(1) = node_coords_->Value(ID, 1);
            Coords(2) = node_coords_->Value(ID, 2);
            Coords(3) = node_coords_->Value(ID, 3);
            return true;
        }

        return false;
    }
}

bool GraphicsMeshDataSource::GetGeomType(const int, const bool IsElement,
                                         MeshVS_EntityType &Type) const
{
    Type = IsElement ? MeshVS_ET_Face : MeshVS_ET_Node;
    return true;
}

bool GraphicsMeshDataSource::GetNodesByElement(const int ID, TColStd_Array1OfInteger &theNodeIDs,
                                               int & /*theNbNodes*/) const
{
    if (mesh_.IsNull())
        return false;

    if (ID >= 1 && ID <= elements_.Extent() && theNodeIDs.Length() >= 3)
    {
        const int aLow = theNodeIDs.Lower();
        theNodeIDs(aLow) = elemnodes_->Value(ID, 1);
        theNodeIDs(aLow + 1) = elemnodes_->Value(ID, 2);
        theNodeIDs(aLow + 2) = elemnodes_->Value(ID, 3);
        return true;
    }

    return false;
}

bool GraphicsMeshDataSource::GetNormal(const int Id, const int Max, double &nx, double &ny,
                                       double &nz) const
{
    if (mesh_.IsNull())
        return false;

    if (Id >= 1 && Id <= elements_.Extent() && Max >= 3)
    {
        nx = elem_normals_->Value(Id, 1);
        ny = elem_normals_->Value(Id, 2);
        nz = elem_normals_->Value(Id, 3);
        return true;
    }

    return false;
}

} // namespace rfio
