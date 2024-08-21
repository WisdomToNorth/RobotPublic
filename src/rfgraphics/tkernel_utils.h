/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#pragma once

#include <string>
#include <string_view>

#include <Message_ProgressRange.hxx>
#include <Quantity_Color.hxx>
#include <Standard_Handle.hxx>
#include <Standard_Version.hxx>

namespace Mayo
{
// Provides helper functions for OpenCascade TKernel library
class TKernelUtils
{
public:
    template <typename TransientType>
    static opencascade::handle<TransientType> makeHandle(const TransientType *ptr)
    {
        return ptr;
    }

    static Message_ProgressRange
    start(const opencascade::handle<Message_ProgressIndicator> &progress);

    // Encodes 'color' into hexadecimal representation with #RRGGBB format
    static std::string colorToHex(const Quantity_Color &color);

    // Decodes a string containing a color with #RRGGBB format to a Quantity_Color object
    // RR, GG, BB are in hexadecimal notation
    static bool colorFromHex(std::string_view strHex, Quantity_Color *color);

    // Returns the type to be used(by default) for RGB colors, depending on OpenCascasde version
    static Quantity_TypeOfColor preferredRgbColorType();

    // Returns a linear-space RGB color from input 'color' expressed with preferredRgbColorType()
    static Quantity_Color toLinearRgbColor(const Quantity_Color &color);
};
} // namespace Mayo
