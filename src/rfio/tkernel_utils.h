// TODO: may be move this to base
#pragma once
#ifndef TKERNEL_UTILS_H
#define TKERNEL_UTILS_H
#include <string>
#include <string_view>

#include <Quantity_Color.hxx>

namespace rfio
{
class ColorUtils
{
public:
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

} // namespace rfio

#endif
