
#pragma once
#ifndef GEODATA_IO_H
#define GEODATA_IO_H

#include <vector>

#include <Bnd_OBB.hxx>
#include <gp_Pnt.hxx>

#include "rfbase/rfjson.h"

class TopoDS_Shape;
class Quantity_Color;

namespace rfgeo
{
using rfbase::RfJson;
using rfbase::RfJsonArray;

class GeoDataIO
{
public:
    static gp_Pnt jsonArrayToPt(const RfJsonArray &arr);

    /**
     * @brief input or output like [1, 2, 3]
     */
    static std::string ptToStr(const gp_Pnt &pt, size_t precision = 6);
    static gp_Pnt strToPt(const std::string &str);

    static RfJson obbSaveToJson(const Bnd_OBB &bndObb);

    static Bnd_OBB obbLoadFromJson(const RfJson &json);

    static RfJson triPtsSaveToJson(const std::vector<gp_Pnt> &triPoints);

    [[nodiscard]] static bool triPtsLoadFromJson(const RfJson &json, std::vector<gp_Pnt> &res);

    static std::string shapeToBrepData(const TopoDS_Shape &shape);

    static TopoDS_Shape brepDataToShape(const std::string &str);

    template <typename T>
    static std::string occTypeToStr(const T &occType)
    {
        std::stringbuf strbuf;
        Standard_OStream os(&strbuf);
        occType.DumpJson(os);
        return strbuf.str();
    }

    template <typename T>
    static T strToOccType(const std::string &str)
    {
        T occType;
        Standard_SStream is;
        is << str;
        int p = 1;
        occType.InitFromJson(is, p);
        return occType;
    }
};
} // namespace rfgeo

#endif
