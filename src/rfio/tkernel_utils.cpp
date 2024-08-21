#include "tkernel_utils.h"

#include <cmath>

#include <Message_ProgressIndicator.hxx>

namespace rfio
{

std::string ColorUtils::colorToHex(const Quantity_Color &color)
{
    // Converts a decimal digit to hexadecimal character
    auto fnHexDigit = [](uint8_t v) -> char
    {
        if (v < 10)
            return '0' + v;
        else
            return 'A' + (v - 10);
    };

    // Adds to 'str' the hexadecimal representation of 'v' belonging to [0, 255]
    auto fnAddHexColorComponent = [&](std::string &str, double v)
    {
        double iv = 0.;                            // Integral part of 'v'
        const double fv = std::modf(v / 16., &iv); // Fractional part of 'v'
        const auto a1 = uint8_t(iv);
        const auto a0 = uint8_t(fv * 16);
        str += fnHexDigit(a1);
        str += fnHexDigit(a0);
    };

    std::string strHex;
    strHex += '#';
    fnAddHexColorComponent(strHex, color.Red() * 255);
    fnAddHexColorComponent(strHex, color.Green() * 255);
    fnAddHexColorComponent(strHex, color.Blue() * 255);
    return strHex;
}

bool ColorUtils::colorFromHex(std::string_view strHex, Quantity_Color *color)
{
    if (!color)
        return true;

    if (strHex.empty())
        return false;

    if (strHex.at(0) != '#')
        return false;

    if (strHex.size() != 7)
        return false;

    // Converts an hexadecimal digit to decimal digit
    auto fnFromHex = [](char c)
    {
        if (c >= '0' && c <= '9')
            return int(c - '0');
        else if (c >= 'A' && c <= 'F')
            return int(c - 'A' + 10);
        else if (c >= 'a' && c <= 'f')
            return int(c - 'a' + 10);
        else
            return -1;
    };

    // Decodes a string containing an hexadecimal number to a decimal number
    auto fnHex2Int = [&](std::string_view str)
    {
        int result = 0;
        for (char c : str)
        {
            result = result * 16;
            const int h = fnFromHex(c);
            if (h < 0)
                return -1;

            result += h;
        }

        return result;
    };

    const int r = fnHex2Int(strHex.substr(1, 2));
    const int g = fnHex2Int(strHex.substr(3, 2));
    const int b = fnHex2Int(strHex.substr(5, 2));
    color->SetValues(r / 255., g / 255., b / 255., Quantity_TOC_RGB);
    return true;
}

Quantity_TypeOfColor ColorUtils::preferredRgbColorType()
{
    return Quantity_TOC_sRGB;
}

Quantity_Color ColorUtils::toLinearRgbColor(const Quantity_Color &color)
{
    return Quantity_Color{Quantity_Color::Convert_LinearRGB_To_sRGB(color.Rgb())};
}

} // namespace rfio
