#include <gtest/gtest.h>

#include "generated/config.h"
#include "rfbase/datautils.h"
#include "rfdb/dbRobot.h"
#include "rfio/reader_robot.h"

namespace RobotTest
{
using namespace rfio;

TEST(io, robot_read)
{
    std::string contextfolder = REG_FILE_FOLDER + "/context";
    std::string contextfile = contextfolder + "/default/default.cntxt";

    RobotReader robotreader;
    robotreader.setSourcePath(contextfolder + "/resources");

    bool succ = robotreader.readFile(contextfile, nullptr);
    ASSERT_TRUE(succ);
    rfdb::dbRobot *robot_var = robotreader.transfer(nullptr).toRobot();
    ASSERT_FALSE(robot_var == nullptr);
    // EXPECT_EQ(var.toPointCloud()->pointSize(), 60545);
}

} // namespace RobotTest
