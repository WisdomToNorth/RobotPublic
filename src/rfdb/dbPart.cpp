#include "dbPart.h"

#include <algorithm>
#include <cassert>
#include <ranges>

#include <BRepBuilderAPI_Transform.hxx>

#include "rfdb/dbutils/dbcheck.h"
#include "rfgeo3d/vmutils.h"

namespace rfdb
{

dbPart::dbPart()
{
    property_ = new dbProperty();
}

dbPart::dbPart(Handle(AIS_ColoredShape) model)
{
    property_ = new dbProperty();
    std::ignore = this->setShape(model);
}

dbPart::dbPart(const TopoDS_Shape &shape)
{
    property_ = new dbProperty();
    std::ignore = this->setShape(shape);
}

dbPart::~dbPart()
{
    delete property_;
    for (auto &child : children_)
    {
        delete child;
    }
}

bool dbPart::setShape(const TopoDS_Shape &shape)
{
    if (children_.empty())
    {
        data_ = shape;
        return true;
    }
    return false;
}

bool dbPart::setShape(Handle(AIS_ColoredShape) model)
{
    if (!rfgeo::VMUtils::checkShapeValid(model) || rfgeo::VMUtils::isShapeEmpty(model))
    {
        return false;
    }

    if (model->Children().Size() == 0)
    {
        data_ = TopoDS_Shape(BRepBuilderAPI_Transform(model->Shape(), model->Transformation()));
        return true;
    }

    for (PrsMgr_ListOfPresentableObjectsIter i(model->Children()); i.More(); i.Next())
    {
        Handle(PrsMgr_PresentableObject) t_object = i.Value();
        Handle(AIS_ColoredShape) child_model = Handle(AIS_ColoredShape)::DownCast(t_object);
        dbPart *child = new dbPart();
        this->addChild(child);
        child->setParent(this);
        [[maybe_unused]] bool succ = child->setShape(child_model);
        assert(succ);
        // child_model->SetLocalTransformation(model->Transformation());
    }
    return true;
}

void dbPart::bindVisibleChangeFunc(const std::function<void(bool)> &func)
{
    property_->setVisibleChangeEvent(func);
}

void dbPart::setVisiable(bool visiable)
{
    for (auto &child : children_)
    {
        child->setVisiable(visiable);
    }
    this->property_->setVisiable(visiable);
}

bool dbPart::getVisiable() const
{
    return property_->isVisiable();
}

dbProperty *dbPart::getProperty() const
{
    return property_;
}

bool dbPart::isEmpty() const
{
    return children_.empty() && data_.IsNull();
}

TopoDS_Shape dbPart::getTopoShape() const
{
    return data_;
}

Handle(AIS_ColoredShape) dbPart::getShape()
{
    if (this->isEmpty())
    {
        return nullptr;
    }

    if (!data_.IsNull())
    {
        assert(dbCheck::checkPart(this));
        Handle(AIS_ColoredShape) res = new AIS_ColoredShape(data_);
        res->SetLocalTransformation(property_->getInitTrsf() * res->LocalTransformation());
        res->SetColor(property_->getColor());
        return res;
    }

    Handle(AIS_ColoredShape) res = new AIS_ColoredShape(TopoDS_Shape());
    res->SetLocalTransformation(property_->getInitTrsf() * res->LocalTransformation());
    // res->SetColor(property_->getColor());

    for (auto &child : children_)
    {
        auto child_model = child->getShape();
        if (child_model.IsNull())
        {
            continue;
        }
        res->AddChild(child_model);
    }
    return res;
}

RfId dbPart::getId() const
{
    return property_->getId();
}

std::string dbPart::getName() const
{
    return property_->getName();
}

bool dbPart::operator==(const dbPart &part)
{
    return this->getId() == part.getId();
}

void dbPart::addChild(dbPart *child)
{
    if (!child->isRoot() && child->getParent())
    {
        child->getParent()->removeChild(child);
    }

    child->setParent(this);
    children_.push_back(child);
}

void dbPart::addChildren(std::vector<dbPart *> children)
{
    for (auto &child : children)
    {
        this->addChild(child);
    }
}

void dbPart::removeChild(dbPart *child)
{
    if (!child->isRoot())
    {
        child->setParent(nullptr);
    }
    children_.erase(std::remove(children_.begin(), children_.end(), child), children_.end());
}

void dbPart::setParent(dbPart *parent)
{
    parent_ = parent;
}

dbPart *dbPart::getParent() const
{
    return parent_;
}

void dbPart::getChildren(std::vector<dbPart *> &children) const
{
    children = children_;
}

bool dbPart::hasChild(const dbPart *child) const
{
    return std::ranges::find(children_, child) != children_.end();
}

bool dbPart::isRoot() const
{
    return parent_ == nullptr;
}

bool dbPart::isLeaf() const
{
    return children_.empty();
}

size_t dbPart::childrenCount() const
{
    return children_.size();
}

void dbPart::setTransparency(double transparency)
{
    for (auto &child : children_)
    {
        child->setTransparency(transparency);
    }
    this->property_->setTransparency(transparency);
}
} // namespace rfdb
