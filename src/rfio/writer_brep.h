#pragma once
#ifndef WRITER_BREP_H
#define WRITER_BREP_H

#include "rfbase/rfjson.h"

#include "writer_base.h"

namespace rfio
{
using rfbase::RfJson;
class BrepWriter : public WriterBase
{
private:
    RfJson buffer_;

public:
    BrepWriter() = default;
    ~BrepWriter() override = default;

    bool save(const std::string &filepath, rfapp::TaskProgress *progress) override;
    void transfer(const rfdb::dbVariant &var, rfapp::TaskProgress *progress,
                  bool concurrent) override;
    RfJson getBuffer() const;

private:
    [[nodiscard]] static RfJson exportToJson(const rfdb::dbPart *_model);
};
} // namespace rfio
#endif
