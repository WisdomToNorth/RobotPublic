

#pragma once
#ifndef RFSETTINGS_H
#define RFSETTINGS_H

#include <filesystem>
#include <functional>
#include <string>
#include <vector>

#include <Quantity_Color.hxx>

#include "rfbase/rfsingleton.h"

namespace rfapp
{
class RfSettings
{
    struct Implementation;

private:
    struct Data
    {
        /*path of the setting file*/
        std::filesystem::path file_path_;

        /*info data*/
        std::vector<Quantity_Color> bg_colors_;
        std::string setting_id_;
        std::string edit_time_;

        /*need update*/
        std::string qss_file_;
        std::string font_family_;
        unsigned int font_pt_size_; // pt
    };

    Data data_;
    Implementation *impl_;

    RfSettings();
    ~RfSettings();

public:
    static RfSettings *createNewSetting();
    static RfSettings *createNewSetting(const RfSettings *);
    static void deleteSetting(RfSettings *&setting);

    std::string getFilePath() const;
    std::string getId() const;
    void getBGColor(std::vector<Quantity_Color> &color) const;
    void setBGColor(const std::vector<Quantity_Color> &color);

    /*theme*/
    void setNewThemeFile(const std::string &path);
    std::string getThemeFileName() const;
    void setThemeChangeEvent(std::function<void(std::string /*.qss path*/)> func);

    void setAllUIRefreshEvent(std::function<void()> func);

    /*Font*/
    std::string getFontFamily() const;
    unsigned int getFontSize() const;
    void setFontFamily(const std::string &family);
    void setFont(const std::string &family, unsigned int size);
    void setFontSize(unsigned int size);
    void setFontChangeEvent(std::function<void(std::string, unsigned int)> func);

    /*save*/
    void changeSettingSavePath(const std::string &path);
    bool save();
    void apply();
    [[nodiscard]] static bool saveSetting(const RfSettings &setting);
    [[nodiscard]] static bool isSettingValid(const RfSettings &setting);
    [[nodiscard]] static bool loadSetting(RfSettings &setting,
                                          const std::string &setting_file_path);
};
} // namespace rfapp

#endif
