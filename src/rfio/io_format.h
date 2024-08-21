/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/
#pragma once
#ifndef IO_FORMAT_H
#define IO_FORMAT_H

#include <span>
#include <string_view>

namespace rfio
{

// Predefined I/O formats
enum class Format
{
    Unknown,
    RfModel,
    RfContext,
    Image,
    STEP,
    IGES,
    OCCBREP,
    STL,
    OBJ,
    GLTF,
    VRML,
    AMF,
    DXF,
    PLY,
    OFF
};

class FormatUtils
{
public:
    // Returns identifier(unique short name) corresponding to 'format'
    static std::string_view formatIdentifier(Format format);

    static Format formatFromSuffix(const std::string_view &prefix);

    // Returns name(eg ISO designation) corresponding to 'format'
    static std::string_view formatName(Format format);

    // Returns array of applicable file suffixes(extensions) corresponding to 'format'
    static std::span<std::string_view> formatFileSuffixes(Format format);

    // Does 'format' provide BRep model ?
    static bool formatProvidesBRep(Format format);

    // Does 'format' provide mesh model ?
    static bool formatProvidesMesh(Format format);
};
} // namespace rfio
#endif // _IO_FORMAT_H_
