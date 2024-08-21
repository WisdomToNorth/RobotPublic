#pragma once
#ifndef VM_UTILS_H
#define VM_UTILS_H

#include <vector>

#include <AIS_Shape.hxx>
#include <Bnd_OBB.hxx>
#include <Standard_Handle.hxx>
#include <TopoDS_Shape.hxx>

namespace rfgeo
{
class VMUtils
{
public:
    static void getAllChildren(Handle(AIS_Shape) shp, std::vector<Handle(AIS_Shape)> &allchild);

    static void getAllObb(Handle(AIS_Shape) shp, std::vector<Bnd_OBB> &res);

    static Bnd_OBB getRootObb(Handle(AIS_Shape) shp);

    static size_t getShapeNodeCount(const Handle(AIS_Shape) vm);

    static void getShapes(Handle(AIS_Shape) input_shp, std::vector<TopoDS_Shape> &shps);

    static void transformLeft(Handle(AIS_InteractiveObject) input_shp, const gp_Trsf &_trsf);

    static const gp_Trsf getAccuTrsf(Handle(AIS_Shape) input_shp);

    static bool sameShape(Handle(AIS_Shape) lhs, Handle(AIS_Shape) rhs);

    static gp_Pnt getAABBCenterPnt(Handle(AIS_Shape) input_shp);

    static gp_Pnt getObbCenterPnt(Handle(AIS_Shape) input_shp);

    static bool checkShapeValid(Handle(AIS_Shape) input_shp);
    static bool isShapeVoid(Handle(AIS_Shape) vm);
    static bool isShapeEmpty(Handle(AIS_Shape) input_shp);

    template <typename T>
    static opencascade::handle<T> getRootParent(opencascade::handle<T> shp)
    {
        static_assert(std::is_base_of<AIS_InteractiveObject, T>::value,
                      "T must be a subclass of AIS_InteractiveObject");

        if (shp.IsNull())
        {
            return nullptr;
        }

        opencascade::handle<T> parent = shp;
        while (parent->Parent())
        {
            parent = opencascade::handle<T>::DownCast(parent->Parent());
        }
        return parent;
    }
};

} // namespace rfgeo
#endif
