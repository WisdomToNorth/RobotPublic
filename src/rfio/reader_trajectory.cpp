#include "reader_trajectory.h"

#include "reader_base.h"

namespace rfio
{

TrajectoryReader::TrajectoryReader()
{
}
TrajectoryReader::~TrajectoryReader()
{
}

bool TrajectoryReader::readFile(const std::string & /*filepath*/,
                                rfapp::TaskProgress * /*progress*/)
{
    return true;
}

rfdb::dbVariant TrajectoryReader::transfer(rfapp::TaskProgress * /*progress*/, bool /*concurrent*/)
{
    return rfdb::dbVariant();
}

} // namespace rfio
