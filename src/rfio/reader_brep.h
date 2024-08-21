#pragma once
#ifndef READER_BREP_H
#define READER_BREP_H

#include "rfbase/rfjson.h"

#include "reader_base.h"

namespace rfio
{
using rfbase::RfJson;
class BrepReader : public FileReaderBase
{
public:
    BrepReader(){};
    BrepReader(const RfJson &data)
        : data_(data){};
    ~BrepReader() override = default;

    bool readFile(const std::string &filepath, rfapp::TaskProgress *progress) override;
    rfdb::dbVariant transfer(rfapp::TaskProgress *progress, bool concurrent = false) override;
    RfJson data() const;

private:
    static rfdb::dbPart *loadFromJson(const RfJson &a_obj, bool use_concurrent);

private:
    RfJson data_;
    std::string file_path_;
};
} // namespace rfio
#endif
