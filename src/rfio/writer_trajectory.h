#pragma once
#ifndef WRITER_BREP_H
#define WRITER_BREP_H

#include <AIS_ColoredShape.hxx>

#include "writer_base.h"

namespace rfio
{
class TrajectoryWriter : public WriterBase
{
public:
    TrajectoryWriter() = default;
    ~TrajectoryWriter() override = default;

    bool save(const std::string &filepath, rfapp::TaskProgress *progress) override;
    void transfer(const rfdb::dbVariant &var, rfapp::TaskProgress *progress,
                  bool concurrent) override;
};
} // namespace rfio
#endif
