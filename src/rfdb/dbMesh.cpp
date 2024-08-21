#include "dbMesh.h"

namespace rfdb
{

dbMesh::dbMesh(/* args */)
{
    property_ = new dbProperty();
}

dbMesh::~dbMesh()
{
    delete property_;
}

void dbMesh::setShape(const TopoDS_Shape &shape)
{
    shape_ = shape;
}
void dbMesh::setMesh(Handle(MeshVS_Mesh) mesh)
{
    v_mesh_ = mesh;
}
void dbMesh::setMeshColors(const std::vector<Quantity_Color> &colors)
{
    mesh_colors_ = colors;
}

Handle(MeshVS_Mesh) dbMesh::getMesh()
{
    return v_mesh_;
}
TopoDS_Shape dbMesh::getShape() const
{
    return shape_;
}

bool dbMesh::isEmpty() const
{
    return shape_.IsNull();
}
} // namespace rfdb
