#include "apptheme.h"

#include <unordered_map>

namespace rfapp
{

// using ThemeType = rfapp::ThemeType;

ThemeType ThemeUtils::getThemeType(const std::string &theme_name)
{
    std::unordered_map<std::string, ThemeType> pre_defined_theme_type_{
        {"darkmode", ThemeType::kDark},
        {"daymode", ThemeType::kLight},
        {"steam", ThemeType::kDark},
        {"xslight", ThemeType::kLight},
        {"xsdark", ThemeType::kDark}};

    if (pre_defined_theme_type_.contains(theme_name))
    {
        return pre_defined_theme_type_[theme_name];
    }
    return ThemeType::kLight;
} // namespace rftheme
} // namespace rfapp
