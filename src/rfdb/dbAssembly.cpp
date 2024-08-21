#include "dbAssembly.h"

namespace rfdb
{
dbAssembly::dbAssembly()
{
    property_ = new dbProperty();
}

dbAssembly::~dbAssembly()
{
    delete property_;
}

// Compration of Id
bool dbAssembly::operator==(const dbAssembly &part)
{
    return property_->getId() == part.getId();
}

void dbAssembly::bindVisibleChangeFunc(const std::function<void(bool)> &func)
{
    property_->setVisibleChangeEvent(func);
}

void dbAssembly::setVisiable(bool visiable)
{
    property_->setVisiable(visiable);
    for (auto part : parts_)
    {
        part->setVisiable(visiable);
    }
}

void dbAssembly::setShape(Handle(AIS_ColoredShape) model)
{
    // TODO
}
bool dbAssembly::getVisiable() const
{
    return property_->isVisiable();
}
bool dbAssembly::isEmpty() const
{
    return parts_.empty();
}

RfId dbAssembly::getId() const
{
    return property_->getId();
}

Handle(AIS_ColoredShape) getShape()
{
    return nullptr;
}
} // namespace rfdb
