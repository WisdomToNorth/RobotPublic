#pragma once
#ifndef WRITER_BASE_H
#define WRITER_BASE_H

#include "rfapp/task_progress.h"
#include "rfdb/dbVariant.h"

namespace rfio
{
class WriterBase
{
public:
    WriterBase() = default;
    virtual ~WriterBase(){};

    virtual bool save(const std::string &filepath, rfapp::TaskProgress *progress) = 0;
    virtual void transfer(const rfdb::dbVariant &var, rfapp::TaskProgress *progress,
                          bool concurrent) = 0;
};
} // namespace rfio
#endif
