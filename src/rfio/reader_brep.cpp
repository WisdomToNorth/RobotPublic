#include "reader_brep.h"

#include <thread>
#include <unordered_set>
#include <vector>

#include "rfbase/datautils.h"
#include "rfbase/rfjson.h"
#include "rfdb/dbPart.h"
#include "rfgeo3d/geodataio.h"

namespace rfio
{

bool BrepReader::readFile(const std::string &filepath, rfapp::TaskProgress *)
{
    file_path_ = filepath;
    std::string file_type = rfbase::DataUtils::getFileExtension(filepath);
    std::unordered_set<std::string> file_types = {"json", "brep", "rfmodel"};

    if (file_types.find(file_type) == file_types.end())
    {
        return false;
    }
    return rfbase::JsonUtils::loadJson(filepath, data_);
}

RfJson BrepReader::data() const
{
    return data_;
}

rfdb::dbVariant BrepReader::transfer(rfapp::TaskProgress *, bool concurrent)
{
    rfdb::dbPart *file = BrepReader::loadFromJson(data_, concurrent);
    file->getProperty()->setName(rfbase::DataUtils::getFileName(file_path_));
    return rfdb::dbVariant(file);
}

rfdb::dbPart *BrepReader::loadFromJson(const RfJson &a_obj, bool use_concurrent)
{
    if (a_obj.is_null())
    {
        return nullptr;
    }

    rfdb::dbPart *mdl = new rfdb::dbPart();

    std::string uuid = a_obj["uuid"].get<std::string>();
    const gp_Trsf &trsf = rfgeo::GeoDataIO::strToOccType<gp_Trsf>(a_obj["trsf"].get<std::string>());
    const Quantity_Color &color =
        rfgeo::GeoDataIO::strToOccType<Quantity_Color>(a_obj["color"].get<std::string>());

    mdl->getProperty()->setId(uuid);
    mdl->getProperty()->setInitTrsf(trsf);
    mdl->getProperty()->setColor(color);

    using namespace rfbase;

    const RfJsonArray &childs_arr = JsonUtils::getJsonArray(a_obj, "childs");
    if (childs_arr.size() == 0)
    {
        const std::string &data_str = a_obj["data"].get<std::string>();
        std::ignore = mdl->setShape(rfgeo::GeoDataIO::brepDataToShape(data_str));
    }
    else
    {
        if (use_concurrent)
        {
            size_t child_size = childs_arr.size();
            std::vector<std::thread> threads;
            std::vector<rfdb::dbPart *> child_mlds(child_size - 1);

            for (size_t i = 0; i < child_size - 1; ++i)
            {
                threads.emplace_back(std::thread(
                    [=, &child_mlds]
                    {
                        child_mlds[i] = loadFromJson(childs_arr[i], use_concurrent);
                        // loadFromJson(childs_arr.at(int(i)).toObject(), use_concurrent);
                    }));
            }
            mdl->addChild(loadFromJson(childs_arr.back(), use_concurrent));

            std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
            for (const auto &child : child_mlds)
            {
                mdl->addChild(child);
            }
        }
        else
        {
            for (const auto &child_obj : childs_arr)
            {
                rfdb::dbPart *child_mld = loadFromJson(child_obj, use_concurrent);
                mdl->addChild(child_mld);
            }
        }
    }

    return mdl;
}
} // namespace rfio
