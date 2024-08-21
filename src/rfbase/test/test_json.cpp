
#include <gtest/gtest.h>

#include "generated/config.h"
#include "rfbase/datautils.h"
#include "rfbase/rfjson.h"

namespace rfbase
{
// rfsettings.json
// {
// "bg_color": [
//     "\"RGB\": [1, 0, 0]",
//     "\"RGB\": [0, 1, 0]",
//     "\"RGB\": [0, 0, 1]"
// ],
// "edit_time": "2023-11-07 11:06:10",
// "setting_id": "{4fd63631-a654-4629-8172-7a1af6cf4a30}",
// "theme_file_name": "xslight.qss",
// "font_family": "Consolas",
// "font_size": 12
// }
TEST(base_json, read)
{
    RfJson json;
    std::string file_path = REG_FILE_FOLDER + "/config/rfsettings.json";
    std::cout << file_path << std::endl;
    bool succ = JsonUtils::loadJson(file_path, json);
    EXPECT_TRUE(succ);
    const std::string &id = json.at("setting_id").get<std::string>();
    EXPECT_EQ(id, "{4fd63631-a654-4629-8172-7a1af6cf4a30}");
    const std::string &theme_file_name = json.at("theme_file_name").get<std::string>();
    EXPECT_EQ(theme_file_name, "xslight.qss");
    const std::string &font_family = json.at("font_family").get<std::string>();
    EXPECT_EQ(font_family, "Consolas");
    const int font_size = json.at("font_size").get<int>();
    EXPECT_EQ(font_size, 12);
    const std::string &edit_time = json.at("edit_time").get<std::string>();
    EXPECT_EQ(edit_time, "2023-11-07 11:06:10");
    const std::vector<std::string> &bg_color = json.at("bg_color").get<std::vector<std::string>>();
    EXPECT_EQ(bg_color.size(), 3);
}

TEST(base_json, save)
{
    RfJson json;
    json["setting_id"] = "{4fd63631-a654-4629-8172-7a1af6cf4a30}";
    json["theme_file_name"] = "xslight.qss";
    json["font_family"] = "Consolas";
    json["font_size"] = 12;
    json["edit_time"] = "2023-11-07 11:06:10";
    std::vector<std::string> bg_color = {"\"RGB\": [1, 0, 0]", "\"RGB\": [0, 1, 0]",
                                         "\"RGB\": [0, 0, 1]"};
    json["bg_color"] = bg_color;
    std::string save_path = REG_OUTPUT_FOLDER + "/test_json_output.json";
    bool succ = JsonUtils::saveJson(json, save_path);
    EXPECT_TRUE(succ);
    std::string golden = REG_FILE_FOLDER + "/config/rfsettings.json";
    bool same = DataUtils::isSameFileWithDiffLineDebug(save_path, golden);
    EXPECT_TRUE(same);
}
} // namespace rfbase
