#include "dbPointCloud.h"

namespace rfdb
{

void dbPointCloud::setPoints(Handle(Graphic3d_ArrayOfPoints) pnts)
{
    this->pnts_ = pnts;
}

Handle(Graphic3d_ArrayOfPoints) dbPointCloud::getPoints()
{
    return pnts_;
}

dbPointCloud::dbPointCloud()
{
    property_ = new dbProperty();
}

dbPointCloud::dbPointCloud(Handle(Graphic3d_ArrayOfPoints) pnts)
{
    property_ = new dbProperty();
    this->setPoints(pnts);
}

dbPointCloud::~dbPointCloud()
{
    delete property_;
}

size_t dbPointCloud::pointSize() const
{
    return pnts_->VertexNumber();
}

RfId dbPointCloud::getId() const
{
    return property_->getId();
}

bool dbPointCloud::isEmpty() const
{
    return pnts_.IsNull();
}
} // namespace rfdb
