#pragma once
#ifndef APPTHEME_H
#define APPTHEME_H

#include <string>

namespace rfapp
{
enum class ThemeType : char
{
    kLight = 0,
    kDark
};
class ThemeUtils
{
public:
    static ThemeType getThemeType(const std::string &theme_name);
};
} // namespace rfapp
#endif
