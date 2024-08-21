
#include "app_settings.h"

#include <string>

#include <Standard_OStream.hxx>
#include <magic_enum/magic_enum.hpp>

#include "generated/config.h"
#include "rfapp/rfsignal.h"
#include "rfbase/datautils.h"
#include "rfbase/rfjson.h"

namespace rfapp
{
struct RfSettings::Implementation
{
    rfapp::RfSignal<std::string> theme_file_change_signal_;
    rfapp::RfSignal<> all_refresh_signal_;
    rfapp::RfSignal<std::string, unsigned int> font_change_signal_;
};
namespace
{
static std::string occTypeToStr(const Quantity_Color &color)
{
    std::stringbuf strbuf;
    Standard_OStream os(&strbuf);
    color.DumpJson(os);

    return strbuf.str();
}

static Quantity_Color strToColor(const std::string &str)
{
    Quantity_Color color;
    Standard_SStream is;
    is << str;
    int p = 1;
    color.InitFromJson(is, p);
    return color;
}
} // namespace

RfSettings::RfSettings()
{
    impl_ = new Implementation();
    data_.setting_id_ = rfbase::DataUtils::generateUuid();
}

RfSettings::~RfSettings()
{
    delete impl_;
}

void RfSettings::apply()
{
    impl_->all_refresh_signal_.send();
}

std::string RfSettings::getId() const
{
    return data_.setting_id_;
}

std::string RfSettings::getFilePath() const
{
    return data_.file_path_.string();
}

void RfSettings::setBGColor(const std::vector<Quantity_Color> &color)
{
    data_.bg_colors_.assign(color.begin(), color.end());
}

void RfSettings::getBGColor(std::vector<Quantity_Color> &color) const
{
    color.assign(data_.bg_colors_.begin(), data_.bg_colors_.end());
}

void RfSettings::setThemeChangeEvent(std::function<void(std::string)> func)
{
    impl_->theme_file_change_signal_.connectSlot(func);
}

void RfSettings::setNewThemeFile(const std::string &theme_name)
{
    if (data_.qss_file_ == theme_name)
    {
        return;
    }
    data_.qss_file_ = theme_name;
    impl_->theme_file_change_signal_.send(theme_name);
}

void RfSettings::setAllUIRefreshEvent(std::function<void()> func)
{
    impl_->all_refresh_signal_.connectSlot(func);
}

std::string RfSettings::getThemeFileName() const
{
    return data_.qss_file_;
}

bool RfSettings::loadSetting(RfSettings &setting, const std::string &setting_file_path)
{
    rfbase::RfJson settings;
    if (!rfbase::JsonUtils::loadJson(setting_file_path, settings))
    {
        return false;
    }
    setting.data_.file_path_ = std::filesystem::path(setting_file_path);
    setting.data_.setting_id_ = rfbase::JsonUtils::getString(settings, "setting_id");
    setting.data_.qss_file_ = rfbase::JsonUtils::getString(settings, "theme_file_name");
    setting.data_.edit_time_ = rfbase::JsonUtils::getString(settings, "edit_time");
    setting.data_.font_pt_size_ = rfbase::JsonUtils::getUInt(settings, "font_size");
    setting.data_.font_family_ = rfbase::JsonUtils::getString(settings, "font_family");
    std::vector<std::string> bg_color_str;
    rfbase::JsonUtils::getArrayValue<std::string>(settings, "bg_color", bg_color_str);
    for (const auto &color_str : bg_color_str)
    {
        setting.data_.bg_colors_.push_back(strToColor(color_str));
    }

    return true;
}

RfSettings *RfSettings::createNewSetting()
{
    RfSettings *setting = new RfSettings();
    setting->data_.setting_id_ = rfbase::DataUtils::generateUuid();
    setting->data_.edit_time_ = rfbase::DataUtils::getCurrentTimeStr();
    setting->data_.qss_file_ = "xslight.qss";
    setting->data_.font_pt_size_ = 10;
    setting->data_.file_path_ = std::filesystem::path(APP_CONFIG_PATH + "/rfsettings.json");

    return setting;
}

RfSettings *RfSettings::createNewSetting(const RfSettings *rhs)
{
    if (rhs == nullptr)
    {
        return createNewSetting();
    }
    RfSettings *setting = new RfSettings();
    setting->data_.setting_id_ = rhs->data_.setting_id_;
    setting->data_.edit_time_ = rhs->data_.edit_time_;
    setting->data_.qss_file_ = rhs->data_.qss_file_;
    setting->data_.font_pt_size_ = rhs->data_.font_pt_size_;
    setting->data_.file_path_ = rhs->data_.file_path_;
    setting->data_.bg_colors_ = rhs->data_.bg_colors_;
    return setting;
}

void RfSettings::deleteSetting(RfSettings *&setting)
{
    delete setting;
    setting = nullptr;
}

bool RfSettings::save()
{
    return RfSettings::saveSetting(*this);
}

bool RfSettings::saveSetting(const RfSettings &setting)
{
    if (!RfSettings::isSettingValid(setting))
    {
        return false;
    }
    rfbase::RfJson settings;
    settings["setting_id"] = setting.data_.setting_id_;
    settings["theme_file_name"] = setting.data_.qss_file_;
    settings["edit_time"] = setting.data_.edit_time_;
    rfbase::RfJsonArray bg_color_arr;
    for (size_t i = 0; i < setting.data_.bg_colors_.size(); ++i)
    {
        bg_color_arr.push_back(occTypeToStr(setting.data_.bg_colors_[i]));
    }
    settings["bg_color"] = bg_color_arr;
    settings["font_size"] = static_cast<int>(setting.data_.font_pt_size_);
    settings["font_family"] = setting.data_.font_family_;
    return rfbase::JsonUtils::saveJson(settings, setting.data_.file_path_.string());
}

void RfSettings::changeSettingSavePath(const std::string &path)
{
    data_.file_path_ = path;
}

std::string RfSettings::getFontFamily() const
{
    return data_.font_family_;
}

void RfSettings::setFontFamily(const std::string &family)
{
    data_.font_family_ = family;
    impl_->font_change_signal_.send(family, data_.font_pt_size_);
}

void RfSettings::setFont(const std::string &family, unsigned int size)
{
    if (data_.font_family_ == family && data_.font_pt_size_ == size)
    {
        return;
    }
    data_.font_family_ = family;
    data_.font_pt_size_ = size;

    impl_->font_change_signal_.send(family, size);
}

void RfSettings::setFontChangeEvent(std::function<void(std::string, unsigned int)> func)
{
    impl_->font_change_signal_.connectSlot(func);
}

unsigned int RfSettings::getFontSize() const
{
    return data_.font_pt_size_;
}

void RfSettings::setFontSize(unsigned int size)
{
    data_.font_pt_size_ = size;
    impl_->font_change_signal_.send(data_.font_family_, size);
}

bool RfSettings::isSettingValid(const RfSettings &setting)
{
    bool is_valid = true;
    is_valid &= (!setting.data_.file_path_.empty());
    is_valid &= (!setting.data_.qss_file_.empty());
    is_valid &= (!setting.data_.setting_id_.empty());
    is_valid &= (!setting.data_.edit_time_.empty());
    return is_valid;
}

} // namespace rfapp
