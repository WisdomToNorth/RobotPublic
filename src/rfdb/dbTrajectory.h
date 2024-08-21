#pragma once
#ifndef DBTRAJECTORY_H
#define DBTRAJECTORY_H

#include "dbProperty.h"
namespace rfdb
{
class dbTrajectory
{
private:
    dbProperty *property_ = nullptr;

public:
    dbTrajectory(/* args */);
    ~dbTrajectory();

    dbProperty *getProperty() const;
};
} // namespace rfdb
#endif
