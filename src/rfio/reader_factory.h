#pragma once
#ifndef READER_FACTORY_H
#define READER_FACTORY_H

#include "rfdb/dbVariant.h"

#include "reader_base.h"

namespace rfio
{
class ReaderFactory
{
public:
    static FileReaderBase *createReader(const std::string &suffix);
};

} // namespace rfio
#endif
