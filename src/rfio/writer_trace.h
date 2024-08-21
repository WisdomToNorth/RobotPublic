#pragma once
#ifndef WRITER_TRACE_H
#define WRITER_TRACE_H

#include <AIS_ColoredShape.hxx>

#include "writer_base.h"

namespace rfio
{
class TraceWriter : public WriterBase
{
public:
    TraceWriter();
    ~TraceWriter();

    bool save(const std::string &filepath, rfapp::TaskProgress *progress) override;
    void transfer(const rfdb::dbVariant &var, rfapp::TaskProgress *progress,
                  bool concurrent) override;
};
} // namespace rfio
#endif
