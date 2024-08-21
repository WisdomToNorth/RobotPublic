#include "dbTrace.h"

namespace rfdb
{

dbTrace::dbTrace(/* args */)
{
    property_ = new dbProperty();
}

dbTrace::~dbTrace()
{
    delete property_;
}
dbProperty *dbTrace::getProperty() const
{
    return property_;
}

void dbTrace::getPoints(std::vector<std::vector<gp_Pnt>> &points) const
{
    points = points_;
}

void dbTrace::getLayer(std::vector<gp_Pnt> &points, const int &layer) const
{
    points = points_[layer];
}

void dbTrace::setPoints(const std::vector<std::vector<gp_Pnt>> &points)
{
    points_ = points;
}

void dbTrace::addLayer(const std::vector<gp_Pnt> &points)
{
    points_.push_back(points);
}

void dbTrace::addPoint(const gp_Pnt &point, const int &layer)
{
    points_[layer].push_back(point);
}
} // namespace rfdb
