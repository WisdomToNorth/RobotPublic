#pragma once
#ifndef READER_TRACE_H
#define READER_TRACE_H

#include "reader_base.h"

namespace rfio
{
class RobotWriter : public FileReaderBase
{
public:
    RobotWriter();
    ~RobotWriter();

    bool readFile(const std::string &filepath, rfapp::TaskProgress *progress) override;
    rfdb::dbVariant transfer(rfapp::TaskProgress *progress, bool concurrent = false) override;

private:
    std::string file_path_;
};
} // namespace rfio
#endif
