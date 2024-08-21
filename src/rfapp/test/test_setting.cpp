
#include <gtest/gtest.h>

#include "generated/config.h"
#include "rfapp/app_settings.h"

namespace rfapp
{
TEST(test_app, test_setting_save)
{
    RfSettings *setting = RfSettings::createNewSetting();
    setting->setNewThemeFile("xslight1k.qss");
    setting->setBGColor({Quantity_Color(1, 0, 0, Quantity_TOC_RGB),
                         Quantity_Color(0, 1, 0, Quantity_TOC_RGB),
                         Quantity_Color(0, 0, 1, Quantity_TOC_RGB)});

    std::string save_path = REG_OUTPUT_FOLDER + "/test_setting_output.json";
    setting->changeSettingSavePath(save_path);
    bool saved = setting->save();
    EXPECT_TRUE(saved);
}

TEST(test_app, test_setting_load)
{
    RfSettings *setting = RfSettings::createNewSetting();

    std::string file_path = REG_FILE_FOLDER + "/config/rfsettings.json";
    bool loaded = RfSettings::loadSetting(*setting, file_path);
    EXPECT_TRUE(loaded);
    EXPECT_EQ(setting->getId(), "{4fd63631-a654-4629-8172-7a1af6cf4a30}");
    EXPECT_EQ(setting->getThemeFileName(), "xslight.qss");

    std::vector<Quantity_Color> colors;
    setting->getBGColor(colors);
    ASSERT_EQ(colors.size(), 3);
    EXPECT_EQ(colors[2].Red(), 0);
    ASSERT_TRUE(rfapp::RfSettings::isSettingValid(*setting));
}

} // namespace RobotTest
