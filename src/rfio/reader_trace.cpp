#include "reader_trace.h"

#include "reader_base.h"

namespace rfio
{

TraceReader::TraceReader(){};
TraceReader::~TraceReader()
{
}

bool TraceReader::readFile(const std::string & /*filepath*/, rfapp::TaskProgress * /*progress*/)
{
    return false;
}
rfdb::dbVariant TraceReader::transfer(rfapp::TaskProgress * /*progress*/, bool /*concurrent*/)
{
    return rfdb::dbVariant();
}

} // namespace rfio
