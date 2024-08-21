#include "writer_trajectory.h"

#include "writer_base.h"

namespace rfio
{

bool TrajectoryWriter::save(const std::string & /*filepath*/, rfapp::TaskProgress * /*progress*/)
{
    return false;
}

void TrajectoryWriter::transfer(const rfdb::dbVariant & /*var*/, rfapp::TaskProgress * /*progress*/,
                                bool /*concurrent*/)
{
}
} // namespace rfio
