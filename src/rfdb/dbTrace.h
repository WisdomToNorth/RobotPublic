#pragma once
#ifndef DBTRACE_H
#define DBTRACE_H

#include <vector>

#include <gp_Pnt.hxx>

#include "dbProperty.h"

namespace rfdb
{
class dbTrace
{
private:
    dbProperty *property_ = nullptr;
    std::vector<std::vector<gp_Pnt>> points_;

public:
    dbTrace(/* args */);
    ~dbTrace();

    dbProperty *getProperty() const;

    void getPoints(std::vector<std::vector<gp_Pnt>> &points) const;
    void getLayer(std::vector<gp_Pnt> &points, const int &layer) const;

    void setPoints(const std::vector<std::vector<gp_Pnt>> &points);
    void addLayer(const std::vector<gp_Pnt> &points);
    void addPoint(const gp_Pnt &point, const int &layer);
};
} // namespace rfdb
#endif
