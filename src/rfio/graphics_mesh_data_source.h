/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#pragma once

#include <MeshVS_DataSource.hxx>
#include <MeshVS_EntityType.hxx>
#include <Poly_Triangulation.hxx>
#include <TColStd_HArray2OfInteger.hxx>
#include <TColStd_HArray2OfReal.hxx>
#include <TColStd_PackedMapOfInteger.hxx>

namespace rfio
{

class GraphicsMeshDataSource : public MeshVS_DataSource
{
public:
    GraphicsMeshDataSource(const Handle_Poly_Triangulation &mesh);

    bool GetGeom(const int ID, const bool IsElement, TColStd_Array1OfReal &Coords, int &NbNodes,
                 MeshVS_EntityType &Type) const override;
    bool GetGeomType(const int ID, const bool IsElement, MeshVS_EntityType &Type) const override;
    bool GetNodesByElement(const int ID, TColStd_Array1OfInteger &NodeIDs,
                           int &NbNodes) const override;
    Standard_Address GetAddr(const int /*ID*/, const bool /*IsElement*/) const override;
    const TColStd_PackedMapOfInteger &GetAllNodes() const override;
    const TColStd_PackedMapOfInteger &GetAllElements() const override;
    bool GetNormal(const int Id, const int Max, double &nx, double &ny, double &nz) const override;

private:
    Handle_Poly_Triangulation mesh_;
    TColStd_PackedMapOfInteger nodes_;
    TColStd_PackedMapOfInteger elements_;
    Handle_TColStd_HArray2OfInteger elemnodes_;
    Handle_TColStd_HArray2OfReal node_coords_;
    Handle_TColStd_HArray2OfReal elem_normals_;
};

} // namespace rfio
