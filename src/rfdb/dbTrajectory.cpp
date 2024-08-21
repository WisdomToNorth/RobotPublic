#include "dbTrajectory.h"

#include "dbProperty.h"
namespace rfdb
{

dbTrajectory::dbTrajectory(/* args */)
{
}

dbTrajectory::~dbTrajectory()
{
}

dbProperty *dbTrajectory::getProperty() const
{
    return property_;
}
} // namespace rfdb
