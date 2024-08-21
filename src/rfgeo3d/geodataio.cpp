
#include "geodataio.h"

#include "stdafx.h"

#include <sstream>

namespace rfgeo
{

gp_Pnt GeoDataIO::jsonArrayToPt(const RfJsonArray &arr)
{
    return gp_Pnt(arr[0].get<double>(), arr[1].get<double>(), arr[2].get<double>());
}

std::string GeoDataIO::ptToStr(const gp_Pnt &pt, size_t precision)
{
    std::ostringstream stream;
    stream.precision(precision);
    stream << std::fixed << "[" << pt.X() << ", " << pt.Y() << ", " << pt.Z() << "]";
    return stream.str();
}

gp_Pnt GeoDataIO::strToPt(const std::string &str)
{
    RfJsonArray arr = rfbase::RfJson::parse(str);
    return gp_Pnt(arr[0].get<double>(), arr[1].get<double>(), arr[2].get<double>());
}

RfJson GeoDataIO::obbSaveToJson(const Bnd_OBB &bnd_obb)
{
    RfJson json;
    json["centerX"] = bnd_obb.Center().X();
    json["centerY"] = bnd_obb.Center().Y();
    json["centerZ"] = bnd_obb.Center().Z();

    json["XDirectionX"] = bnd_obb.XDirection().X();
    json["XDirectionY"] = bnd_obb.XDirection().Y();
    json["XDirectionZ"] = bnd_obb.XDirection().Z();

    json["YDirectionX"] = bnd_obb.YDirection().X();
    json["YDirectionY"] = bnd_obb.YDirection().Y();
    json["YDirectionZ"] = bnd_obb.YDirection().Z();

    json["ZDirectionX"] = bnd_obb.ZDirection().X();
    json["ZDirectionY"] = bnd_obb.ZDirection().Y();
    json["ZDirectionZ"] = bnd_obb.ZDirection().Z();

    json["XHSize"] = bnd_obb.XHSize();
    json["YHSize"] = bnd_obb.YHSize();
    json["ZHSize"] = bnd_obb.ZHSize();

    return json;
}

Bnd_OBB GeoDataIO::obbLoadFromJson(const RfJson &json)
{
    double centerX = json.at("centerX").get<double>();
    double centerY = json.at("centerY").get<double>();
    double centerZ = json.at("centerZ").get<double>();

    double XDirectionX = json.at("XDirectionX").get<double>();
    double XDirectionY = json.at("XDirectionY").get<double>();
    double XDirectionZ = json.at("XDirectionZ").get<double>();

    double YDirectionX = json.at("YDirectionX").get<double>();
    double YDirectionY = json.at("YDirectionY").get<double>();
    double YDirectionZ = json.at("YDirectionZ").get<double>();

    double ZDirectionX = json.at("ZDirectionX").get<double>();
    double ZDirectionY = json.at("ZDirectionY").get<double>();
    double ZDirectionZ = json.at("ZDirectionZ").get<double>();

    double XHSize = json.at("XHSize").get<double>();
    double YHSize = json.at("YHSize").get<double>();
    double ZHSize = json.at("ZHSize").get<double>();

    gp_XYZ center(centerX, centerY, centerZ);
    gp_XYZ XDirection(XDirectionX, XDirectionY, XDirectionZ);
    gp_XYZ YDirection(YDirectionX, YDirectionY, YDirectionZ);
    gp_XYZ ZDirection(ZDirectionX, ZDirectionY, ZDirectionZ);

    Bnd_OBB bndObb(center, XDirection, YDirection, ZDirection, XHSize, YHSize, ZHSize);
    return bndObb;
}

RfJson GeoDataIO::triPtsSaveToJson(const std::vector<gp_Pnt> &tri_pnts)
{
    RfJson json;
    json["point1X"] = tri_pnts[0].X();
    json["point1Y"] = tri_pnts[0].Y();
    json["point1Z"] = tri_pnts[0].Z();

    json["point2X"] = tri_pnts[1].X();
    json["point2Y"] = tri_pnts[1].Y();
    json["point2Z"] = tri_pnts[1].Z();

    json["point3X"] = tri_pnts[2].X();
    json["point3Y"] = tri_pnts[2].Y();
    json["point3Z"] = tri_pnts[2].Z();

    return json;
}

bool GeoDataIO::triPtsLoadFromJson(const RfJson &json, std::vector<gp_Pnt> &res)
{
    if (!json.contains("point1X"))
    {
        return false;
    }

    double point1X = json.at("point1X").get<double>();
    double point1Y = json.at("point1Y").get<double>();
    double point1Z = json.at("point1Z").get<double>();

    double point2X = json.at("point2X").get<double>();
    double point2Y = json.at("point2Y").get<double>();
    double point2Z = json.at("point2Z").get<double>();

    double point3X = json.at("point3X").get<double>();
    double point3Y = json.at("point3Y").get<double>();
    double point3Z = json.at("point3Z").get<double>();

    gp_Pnt point1(point1X, point1Y, point1Z);
    gp_Pnt point2(point2X, point2Y, point2Z);
    gp_Pnt point3(point3X, point3Y, point3Z);

    res.emplace_back(point1);
    res.emplace_back(point2);
    res.emplace_back(point3);

    return true;
}

std::string GeoDataIO::shapeToBrepData(const TopoDS_Shape &shape)
{
    std::stringbuf strbuf;
    Standard_OStream os(&strbuf);
    BRepTools::Write(shape, os);
    return strbuf.str();
}

TopoDS_Shape GeoDataIO::brepDataToShape(const std::string &str)
{
    BRep_Builder builder;
    TopoDS_Shape shape;

    Standard_SStream is;
    is << str;
    BRepTools::Read(shape, is, builder);
    return shape;
}
} // namespace rfgeo
