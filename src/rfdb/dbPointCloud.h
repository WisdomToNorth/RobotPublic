#pragma once
#ifndef DBPOINTCLOUD_H
#define DBPOINTCLOUD_H
#include <Graphic3d_ArrayOfPoints.hxx>

#include "dbProperty.h"

namespace rfdb
{
class dbPointCloud
{
public:
    explicit dbPointCloud();
    explicit dbPointCloud(Handle(Graphic3d_ArrayOfPoints) pnts);
    ~dbPointCloud();

    void setPoints(Handle(Graphic3d_ArrayOfPoints) pnts);
    Handle(Graphic3d_ArrayOfPoints) getPoints();
    size_t pointSize() const;
    RfId getId() const;
    bool isEmpty() const;
    dbProperty *getProperty() const
    {
        return property_;
    }

private:
    Handle(Graphic3d_ArrayOfPoints) pnts_;
    dbProperty *property_ = nullptr;
};

} // namespace rfdb
#endif
