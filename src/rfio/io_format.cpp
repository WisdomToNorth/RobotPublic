/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#include "io_format.h"

#include <algorithm>
#include <string_view>
#include <unordered_map>

namespace rfio
{

std::string_view FormatUtils::formatIdentifier(Format format)
{
    switch (format)
    {
    case Format::Unknown: return "";
    case Format::RfModel: return "RFModel";
    case Format::RfContext: return "RFContext";
    case Format::Image: return "Image";
    case Format::STEP: return "STEP";
    case Format::IGES: return "IGES";
    case Format::OCCBREP: return "OCCBREP";
    case Format::STL: return "STL";
    case Format::OBJ: return "OBJ";
    case Format::GLTF: return "GLTF";
    case Format::VRML: return "VRML";
    case Format::AMF: return "AMF";
    case Format::DXF: return "DXF";
    case Format::PLY: return "PLY";
    case Format::OFF: return "OFF";
    default: return "";
    }
}

Format FormatUtils::formatFromSuffix(const std::string_view &prefix)
{
    std::unordered_map<std::string_view, Format> map = {
        {"rfmodel", Format::RfModel}, {"json", Format::RfModel}, {"cntxt", Format::RfContext},
        {"image", Format::Image},     {"step", Format::STEP},    {"stp", Format::STEP},
        {"iges", Format::IGES},       {"occ", Format::OCCBREP},  {"stl", Format::STL},
        {"obj", Format::OBJ},         {"gltf", Format::GLTF},    {"vrml", Format::VRML},
        {"amf", Format::AMF},         {"dxf", Format::DXF},      {"ply", Format::PLY},
        {"off", Format::OFF}};

    if (map.contains(prefix))
    {
        return map.at(prefix);
    }
    else
    {
        return Format::Unknown;
    }
}

std::string_view FormatUtils::formatName(Format format)
{
    switch (format)
    {
    case Format::Unknown: return "Format_Unknown";
    case Format::RfModel: return "RFModel";
    case Format::RfContext: return "RFContext";
    case Format::Image: return "Image";
    case Format::STEP: return "STEP (ISO 10303)";
    case Format::IGES: return "IGES (ASME Y14.26M)";
    case Format::OCCBREP: return "OpenCascade BREP";
    case Format::STL: return "STL (STereo-Lithography)";
    case Format::OBJ: return "Wavefront OBJ";
    case Format::GLTF: return "glTF (GL Transmission Format)";
    case Format::VRML: return "VRML (ISO/CEI 14772-2)";
    case Format::AMF: return "Additive manufacturing file format (ISO/ASTM 52915:2016)";
    case Format::DXF: return "Drawing Exchange Format";
    case Format::PLY: return "PolygonL File Format";
    case Format::OFF: return "Object File Format";
    default: return "";
    }
}

std::span<std::string_view> FormatUtils::formatFileSuffixes(Format format)
{
    static std::string_view img_suffix[] = {"bmp", "jpeg", "jpg", "png", "gif", "ppm", "tiff"};
    static std::string_view step_suffix[] = {"step", "stp"};
    static std::string_view rfmodel_suffix[] = {"rfmodel", "json"};
    static std::string_view rfcontext_suffix[] = {"cntxt"};
    static std::string_view iges_suffix[] = {"iges", "igs"};
    static std::string_view occ_suffix[] = {"brep", "rle", "occ"};
    static std::string_view stl_suffix[] = {"stl"};
    static std::string_view obj_suffix[] = {"obj"};
    static std::string_view gltf_suffix[] = {"gltf", "glb"};
    static std::string_view vrml_suffix[] = {"wrl", "wrz", "vrml"};
    static std::string_view amf_suffix[] = {"amf"};
    static std::string_view dxf_suffix[] = {"dxf"};
    static std::string_view ply_suffix[] = {"ply"};
    static std::string_view off_suffix[] = {"off"};

    switch (format)
    {
    case Format::Unknown: return {};
    case Format::RfModel: return rfmodel_suffix;
    case Format::RfContext: return rfcontext_suffix;
    case Format::Image: return img_suffix;
    case Format::STEP: return step_suffix;
    case Format::IGES: return iges_suffix;
    case Format::OCCBREP: return occ_suffix;
    case Format::STL: return stl_suffix;
    case Format::OBJ: return obj_suffix;
    case Format::GLTF: return gltf_suffix;
    case Format::VRML: return vrml_suffix;
    case Format::AMF: return amf_suffix;
    case Format::DXF: return dxf_suffix;
    case Format::PLY: return ply_suffix;
    case Format::OFF: return off_suffix;
    default: return {};
    }
}

bool FormatUtils::formatProvidesBRep(Format format)
{
    static const Format brepFormats[] = {Format::STEP, Format::IGES, Format::OCCBREP, Format::DXF};
    return std::any_of(std::cbegin(brepFormats), std::cend(brepFormats),
                       [=](Format candidate) { return candidate == format; });
}

bool FormatUtils::formatProvidesMesh(Format format)
{
    return !formatProvidesBRep(format) && format != Format::Unknown && format != Format::Image;
}
} // namespace rfio
