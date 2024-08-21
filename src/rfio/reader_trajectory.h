#pragma once
#ifndef READER_ROBOTPATH_H
#define READER_ROBOTPATH_H

#include "reader_base.h"

namespace rfio
{
class TrajectoryReader : public FileReaderBase
{
public:
    TrajectoryReader();
    ~TrajectoryReader();

    bool readFile(const std::string &filepath, rfapp::TaskProgress *progress) override;
    rfdb::dbVariant transfer(rfapp::TaskProgress *progress, bool concurrent = false) override;

private:
    std::string file_path_;
};
} // namespace rfio
#endif
