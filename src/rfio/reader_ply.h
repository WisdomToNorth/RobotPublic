#pragma once
#ifndef READER_PLY_H
#define READER_PLY_H

#include <string>
#include <vector>

#include "reader_base.h"

namespace rfio
{
class PlyReader : public FileReaderBase
{
public:
    PlyReader() = default;
    ~PlyReader() override = default;
    PlyReader(const PlyReader &) = delete;
    PlyReader &operator=(const PlyReader &) = delete;

    bool readFile(const std::string &filepath, rfapp::TaskProgress *progress) override;
    rfdb::dbVariant transfer(rfapp::TaskProgress *progress, bool concurrent = false) override;

private:
    rfdb::dbVariant transferMesh(rfapp::TaskProgress *progress);
    rfdb::dbVariant transferPointCloud(rfapp::TaskProgress *progress);
    static rfdb::dbVariant createObject(const TopoDS_Shape &label,
                                        const std::vector<Quantity_Color> &spanNodeColor);

    uint32_t node_count_ = 0;
    std::vector<float> node_coords_;
    std::vector<int> vec_index_;
    std::vector<float> vec_normal_coord_;
    std::vector<uint8_t> vec_color_component_;
    std::string file_path_;
};

} // namespace rfio
#endif
