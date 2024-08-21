#include <iostream>
#include <vector>

#include <gtest/gtest.h>

#include "generated/config.h"
#include "rfio/reader_robot.h"
#include "rfrobot/rfRobotDefine.h"

namespace RobotTest
{
using namespace rfrobot;

TEST(test_load, test_load_para_1)
{
    std::string filename = REG_FILE_FOLDER + "/context/default/colorful_robot.rfpara";
    std::vector<JointParams> arm_params;
    bool load_succ = rfio::RobotReader::loadParams(filename, arm_params);
    ASSERT_TRUE(load_succ);

    ASSERT_EQ(arm_params.size(), 6);
    EXPECT_EQ(arm_params[0].type_, JType::RotZ);
    EXPECT_EQ(arm_params[1].type_, JType::RotY);
    EXPECT_EQ(arm_params[2].type_, JType::RotY);
    EXPECT_EQ(arm_params[3].type_, JType::RotX);
    EXPECT_EQ(arm_params[4].type_, JType::RotY);
    EXPECT_EQ(arm_params[5].type_, JType::RotX);

    EXPECT_EQ(arm_params[0].arm_rot_pnt_.IsEqual(gp_Pnt(0, 0, 278), 1e-6), true);
    EXPECT_EQ(arm_params[0].dh_info_.IsEqual(gp_Pnt(150.0, 0.0, 450.0), 1e-6), true);
    EXPECT_EQ(arm_params[0].min_v_degree_, -180);
    EXPECT_EQ(arm_params[0].max_v_degree_, 180);
    EXPECT_EQ(arm_params[0].models_name_list_.front(), "{8c0cc229-7f6e-462b-9601-2c6474b6a7f9}");
}

} // namespace RobotTest
