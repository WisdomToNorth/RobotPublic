#pragma once
#ifndef DBMESH_H
#define DBMESH_H

#include <AIS_ColoredShape.hxx>
#include <MeshVS_Mesh.hxx> //TODO: split viewdata and data
#include <Quantity_Color.hxx>
#include <TopoDS_Shape.hxx>

#include "dbProperty.h"
namespace rfdb
{
class dbMesh
{
private:
    dbProperty *property_ = nullptr;
    Handle(MeshVS_Mesh) v_mesh_;
    TopoDS_Shape shape_;
    std::vector<Quantity_Color> mesh_colors_;

public:
    dbMesh(/* args */);
    ~dbMesh();

    void setShape(const TopoDS_Shape &shape);
    void setMesh(Handle(MeshVS_Mesh) mesh);
    void setMeshColors(const std::vector<Quantity_Color> &colors);
    Handle(MeshVS_Mesh) getMesh();
    TopoDS_Shape getShape() const;
    bool isEmpty() const;
    dbProperty *getProperty() const
    {
        return property_;
    }
};

} // namespace rfdb
#endif
