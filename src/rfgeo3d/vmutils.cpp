#include "vmutils.h"

#include "stdafx.h"

#include <cassert>

#include "rfgeo3d/shapeutils.h"

namespace rfgeo
{

void VMUtils::getAllChildren(Handle(AIS_Shape) shp, std::vector<Handle(AIS_Shape)> &allchild)
{
    if (shp.IsNull())
    {
        return;
    }

    if (!shp->Shape().IsNull())
    {
        allchild.push_back(shp);
        if (shp->Children().Size() > 0)
        {
            assert(false);
        }
    }
    else
    {
        for (PrsMgr_ListOfPresentableObjectsIter i(shp->Children()); i.More(); i.Next())
        {
            Handle(PrsMgr_PresentableObject) t_object = i.Value();
            assert(t_object->IsKind(STANDARD_TYPE(AIS_Shape)));
            Handle(AIS_Shape) child_model = Handle(AIS_Shape)::DownCast(t_object);
            VMUtils::getAllChildren(child_model, allchild);
        }
    }
};

void VMUtils::getAllObb(Handle(AIS_Shape) shp, std::vector<Bnd_OBB> &res)
{
    std::vector<Handle(AIS_Shape)> l_all_model;
    getAllChildren(shp, l_all_model);

    for (const auto &a : l_all_model)
    {
        if (a->Shape().IsNull())
        {
            continue;
        }

        Bnd_OBB subobb = ShapeUtils::getOBB(a);
        res.emplace_back(subobb);
    }
}

Bnd_OBB VMUtils::getRootObb(Handle(AIS_Shape) shp)
{
    Bnd_OBB root_obb;
    std::vector<Bnd_OBB> _obbs;
    getAllObb(shp, _obbs);

    for (const auto &obb : _obbs)
    {
        root_obb.Add(obb);
    }
    return root_obb;
}

size_t VMUtils::getShapeNodeCount(const Handle(AIS_Shape) vm)
{
    size_t cnt = 0;
    if (vm->Children().Size() == 0 && !vm->Shape().IsNull())
    {
        cnt++;
    }
    for (PrsMgr_ListOfPresentableObjectsIter i(vm->Children()); i.More(); i.Next())
    {
        Handle(PrsMgr_PresentableObject) t_object = i.Value();
        assert(t_object->IsKind(STANDARD_TYPE(AIS_Shape)));
        Handle(AIS_Shape) child_model = Handle(AIS_Shape)::DownCast(t_object);

        if (child_model->Children().Size() > 0)
        {
            cnt += getShapeNodeCount(child_model);
        }
        else if (!child_model->Shape().IsNull())
        {
            cnt++;
        }
    }
    return cnt;
}

void VMUtils::getShapes(Handle(AIS_Shape) input_shp, std::vector<TopoDS_Shape> &shps)
{
    if (input_shp.IsNull())
    {
        return;
    }

    if (input_shp->Children().Size() == 0)
    {
        shps.push_back(TopoDS_Shape(
            BRepBuilderAPI_Transform(input_shp->Shape(), input_shp->Transformation())));
        return;
    }

    for (PrsMgr_ListOfPresentableObjectsIter i(input_shp->Children()); i.More(); i.Next())
    {
        auto t_object = i.Value();
        assert(t_object->IsKind(STANDARD_TYPE(AIS_Shape)));

        Handle(AIS_Shape) child_model = Handle(AIS_Shape)::DownCast(t_object);
        getShapes(child_model, shps);
    }
}

void VMUtils::transformLeft(Handle(AIS_InteractiveObject) input_shp, const gp_Trsf &_trsf)
{
    gp_Trsf transformation;

    if (input_shp->Parent())
    {
        gp_Trsf root_trsf = VMUtils::getRootParent(input_shp)->Transformation();
        transformation = root_trsf.Inverted() * _trsf * root_trsf;
    }
    else
    {
        transformation = _trsf;
    }
    transformation = transformation * input_shp->LocalTransformation();
    input_shp->SetLocalTransformation(transformation);
}

const gp_Trsf VMUtils::getAccuTrsf(Handle(AIS_Shape) input_shp)
{
    // gp_Trsf accutest = init_trsf_.Inverted() * this->Transformation();
    // return accutest;
    return input_shp->LocalTransformation(); // TODO: this is not correct
}
bool VMUtils::sameShape(Handle(AIS_Shape) input_shp, Handle(AIS_Shape) rhs)
{
    std::vector<Handle(AIS_Shape)> v1;
    std::vector<Handle(AIS_Shape)> v2;
    VMUtils::getAllChildren(rhs, v1);
    VMUtils::getAllChildren(input_shp, v2);

    if (v1.size() != v2.size())
    {
        return false;
    }
    for (size_t i = 0; i < v1.size(); ++i)
    {
        if (!v1[i]->Shape().IsEqual(v2[i]->Shape()))
        {
            return false;
        }
    }
    return true;
}

gp_Pnt VMUtils::getAABBCenterPnt(Handle(AIS_Shape) input_shp)
{
    Bnd_Box box = ShapeUtils::getFullAABB(input_shp);
    return ShapeUtils::getBoxCenter(box);
}

gp_Pnt VMUtils::getObbCenterPnt(Handle(AIS_Shape) input_shp)
{
    Bnd_OBB obb = VMUtils::getRootObb(input_shp);
    return obb.Center();
}

bool VMUtils::checkShapeValid(Handle(AIS_Shape) input_shp)
{
    return !input_shp.IsNull() &&
           (!input_shp->Shape().IsNull() ^ (input_shp->Children().Size() > 0));
}

bool VMUtils::isShapeEmpty(Handle(AIS_Shape) input_shp)
{
    return isShapeVoid(input_shp);
}

bool VMUtils::isShapeVoid(Handle(AIS_Shape) vm)
{
    return vm->Children().Size() == 0 && vm->Shape().IsNull();
}

} // namespace rfgeo
