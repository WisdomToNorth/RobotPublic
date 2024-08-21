#pragma once
#ifndef READER_BASE_H
#define READER_BASE_H

#include "rfdb/dbVariant.h"

namespace rfapp
{
class TaskProgress;
};

namespace rfio
{
class FileReaderBase
{
public:
    FileReaderBase() = default;
    virtual ~FileReaderBase(){};

    virtual bool readFile(const std::string &filepath, rfapp::TaskProgress *progress) = 0;
    virtual rfdb::dbVariant transfer(rfapp::TaskProgress *progress, bool concurrent) = 0;
};

} // namespace rfio
#endif
