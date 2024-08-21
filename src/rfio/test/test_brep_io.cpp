
#include <AIS_ColoredShape.hxx>
#include <gtest/gtest.h>

#include "generated/config.h"
#include "rfbase/datautils.h"
#include "rfbase/rfTimer.h"
#include "rfdb/dbutils/dbpartutils.h"
#include "rfgeo3d/shapeutils.h"
#include "rfgeo3d/vmutils.h"
#include "rfio/reader_brep.h"
#include "rfio/reader_step.h"
#include "rfio/writer_brep.h"

namespace RobotTest
{
using namespace rfio;
using namespace rfgeo;

TEST(io, brep_reader_1)
{
    std::string brep_file = REG_RESOURCE_FOLDER + "/rfbrep" + "/colorful_robot.json";
    BrepReader reader;
    bool succ = reader.readFile(brep_file, nullptr);
    ASSERT_TRUE(succ);

    rfbase::FrTimer timer;
    rfdb::dbVariant var = reader.transfer(nullptr);
    std::cout << "Time used: " << timer.timeFromBegin() << '\n';
    Handle(AIS_ColoredShape) vm = var.toPart()->getShape();
    ASSERT_FALSE(vm.IsNull());
    size_t child_count = rfgeo::VMUtils::getShapeNodeCount(vm);
    EXPECT_EQ(child_count, 8);
}

TEST(io, brep_reader_2)
{
    std::string brep_file = REG_RESOURCE_FOLDER + "/rfbrep" + "/assembled_part.json";
    BrepReader reader;
    bool succ = reader.readFile(brep_file, nullptr);
    ASSERT_TRUE(succ);

    rfbase::FrTimer timer;
    rfdb::dbVariant var = reader.transfer(nullptr);
    std::cout << "Time used: " << timer.timeFromBegin() << '\n';
    Handle(AIS_ColoredShape) vm = var.toPart()->getShape();
    ASSERT_FALSE(vm.IsNull());
    size_t child_count = rfgeo::VMUtils::getShapeNodeCount(vm);
    EXPECT_EQ(child_count, 6);
}

TEST(io, brep_reader_concurrent_1)
{
    std::string brep_file = REG_RESOURCE_FOLDER + "/rfbrep" + "/colorful_robot.json";
    BrepReader reader;
    bool succ = reader.readFile(brep_file, nullptr);
    ASSERT_TRUE(succ);

    rfbase::FrTimer timer;
    rfdb::dbVariant var = reader.transfer(nullptr, true);
    std::cout << "Time used: " << timer.timeFromBegin() << '\n';
    Handle(AIS_ColoredShape) vm = var.toPart()->getShape();
    ASSERT_FALSE(vm.IsNull());
    size_t child_count = rfgeo::VMUtils::getShapeNodeCount(vm);
    EXPECT_EQ(child_count, 8);
}

TEST(io, brep_reader_concurrent_2)
{
    std::string brep_file = REG_RESOURCE_FOLDER + "/rfbrep" + "/assembled_part.json";
    BrepReader reader;
    bool succ = reader.readFile(brep_file, nullptr);
    ASSERT_TRUE(succ);

    rfbase::FrTimer timer;
    rfdb::dbVariant var = reader.transfer(nullptr, true);
    std::cout << "Time used: " << timer.timeFromBegin() << '\n';
    Handle(AIS_ColoredShape) vm = var.toPart()->getShape();
    ASSERT_FALSE(vm.IsNull());
    size_t child_count = rfgeo::VMUtils::getShapeNodeCount(vm);
    EXPECT_EQ(child_count, 6);
}

TEST(io, brep_writer_1)
{
    std::string brep_file = REG_RESOURCE_FOLDER + "/rfbrep" + "/tripart.json";
    BrepReader reader;
    bool succ = reader.readFile(brep_file, nullptr);
    ASSERT_TRUE(succ);
    rfdb::dbVariant var = reader.transfer(nullptr);

    BrepWriter writer;
    writer.transfer(var, nullptr, false);
    std::string out_file = REG_OUTPUT_FOLDER + "/tripart_out.json";
    writer.save(out_file, nullptr);
    // NOTE: Data of this depends on version of OCC, so we can't compare it.
    // rfbase::RfJson input_data = reader.data();
    // rfbase::RfJson output = writer.getBuffer();
    // writer.save(out_file, nullptr);
    // EXPECT_EQ(input_data, output);

    BrepReader reader2;
    succ = reader2.readFile(out_file, nullptr);
    rfdb::dbVariant var_new = reader2.transfer(nullptr);
    EXPECT_EQ(rfdb::dbPartUtils::isPartEqual(var.toPart(), var_new.toPart()), true);
}

TEST(io, brep_writer_2)
{
    std::string brep_file = REG_RESOURCE_FOLDER + "/rfbrep" + "/assembled_part.json";
    BrepReader reader;
    bool succ = reader.readFile(brep_file, nullptr);
    ASSERT_TRUE(succ);
    rfdb::dbVariant var = reader.transfer(nullptr);

    BrepWriter writer;
    writer.transfer(var, nullptr, false);
    std::string out_file = REG_OUTPUT_FOLDER + "/assembled_part_output.json";
    writer.save(out_file, nullptr);

    BrepReader reader2;
    succ = reader2.readFile(out_file, nullptr);
    rfdb::dbVariant var_new = reader2.transfer(nullptr);

    EXPECT_EQ(rfdb::dbPartUtils::isPartEqual(var.toPart(), var_new.toPart()), true);
}
} // namespace RobotTest
