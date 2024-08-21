#include <gtest/gtest.h>

#include "generated/config.h"
#include "rfbase/datautils.h"
#include "rfdb/dbPointCloud.h"
#include "rfdb/dbVariant.h"
#include "rfio/reader_ply.h"

namespace RobotTest
{
using namespace rfio;

TEST(io, ply_read_pointcloud)
{
    std::string plyfile = REG_RESOURCE_FOLDER + "/ply/FIN_1.ply";

    PlyReader plyreader;
    bool succ = plyreader.readFile(plyfile, nullptr);
    ASSERT_TRUE(succ);
    rfdb::dbVariant var = plyreader.transfer(nullptr);
    EXPECT_EQ(var.toPointCloud()->pointSize(), 60545);
}

TEST(io, ply_read_mesh)
{
    std::string plyfile = REG_RESOURCE_FOLDER + "/ply/weld_gun.ply";

    PlyReader plyreader;
    bool succ = plyreader.readFile(plyfile, nullptr);
    ASSERT_TRUE(succ);
    rfdb::dbVariant var = plyreader.transfer(nullptr);
    EXPECT_EQ(var.toMesh()->isEmpty(), false);
}
} // namespace RobotTest
