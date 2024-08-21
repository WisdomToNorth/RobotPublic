#include "writer_trace.h"

#include "writer_base.h"

namespace rfio
{

TraceWriter::TraceWriter()
{
}
TraceWriter::~TraceWriter()
{
}

bool TraceWriter::save(const std::string & /*filepath*/, rfapp::TaskProgress * /*progress*/)
{
    return true;
}

void TraceWriter::transfer(const rfdb::dbVariant & /*var*/, rfapp::TaskProgress * /*progress*/,
                           bool /*concurrent*/)
{
}
} // namespace rfio
