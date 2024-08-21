#include "writer_brep.h"

#include "stdafx.h"

#include <thread>
#include <vector>

#include "rfbase/datautils.h"
#include "rfbase/rfjson.h"
#include "rfdb/dbPart.h"
#include "rfgeo3d/geodataio.h"
#include "rfgeo3d/vmutils.h"

namespace rfio
{
bool BrepWriter::save(const std::string &filepath, rfapp::TaskProgress * /*progress*/)
{
    if (buffer_.empty())
    {
        return false;
    }
    return rfbase::JsonUtils::saveJson(buffer_, filepath);
}

void BrepWriter::transfer(const rfdb::dbVariant &var, rfapp::TaskProgress * /*progress*/,
                          bool /*concurrent*/)
{
    if (var.getType() != rfdb::dbVariant::Type::Part)
    {
        return;
    }
    buffer_ = BrepWriter::exportToJson(var.toPart());
}

RfJson BrepWriter::exportToJson(const rfdb::dbPart *_model)
{
    RfJson data;
    const auto &property = _model->getProperty();
    data["uuid"] = property->getId().c_str();
    data["trsf"] = rfgeo::GeoDataIO::occTypeToStr(property->getInitTrsf());
    data["color"] = rfgeo::GeoDataIO::occTypeToStr(property->getColor());

    if (_model->childrenCount() == 0)
    {
        data["data"] = rfgeo::GeoDataIO::shapeToBrepData(_model->getTopoShape());
        data["childs"] = rfbase::RfJsonArray();
    }
    else
    {
        data["data"] = "";
        rfbase::RfJsonArray child_arr;
        std::vector<rfdb::dbPart *> childs;
        _model->getChildren(childs);
        for (const auto &child_model : childs)
        {
            child_arr.push_back(exportToJson(child_model)); // we may use concurrent here
        }
        data["childs"] = child_arr;
    }

    return data;
}

RfJson BrepWriter::getBuffer() const
{
    return buffer_;
}

} // namespace rfio
