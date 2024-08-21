#include "writer_robot.h"

#include "writer_base.h"

namespace rfio
{

RobotWriter::RobotWriter()
{
}

RobotWriter::~RobotWriter()
{
}

bool RobotWriter::readFile(const std::string & /*filepath*/, rfapp::TaskProgress * /*progress*/)
{
    return false;
}

rfdb::dbVariant RobotWriter::transfer(rfapp::TaskProgress * /*progress*/, bool /*concurrent*/)
{
    return rfdb::dbVariant();
}
} // namespace rfio
