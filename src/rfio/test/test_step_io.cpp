
#include <AIS_ColoredShape.hxx>
#include <gtest/gtest.h>

#include "generated/config.h"
#include "rfbase/datautils.h"
#include "rfbase/rfjson.h"
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

void convertStepToBrepIntoRegressionFile()
{
    std::string config_path2 = REG_RESOURCE_FOLDER + "/step/assembled_part.STEP";

    StepReader reader;
    reader.readFile(config_path2, nullptr);
    auto var = reader.transfer(nullptr, false);
    Handle(AIS_ColoredShape) vm2 = var.toPart()->getShape();

    BrepWriter writer;
    writer.transfer(var, nullptr, false);
    const rfbase::RfJson &data = writer.getBuffer();
    std::string name = REG_RESOURCE_FOLDER + "/assembled_part.json";
    std::ignore = rfbase::JsonUtils::saveJson(data, name);
}

TEST(io, stepread_1)
{
    std::string golden_file = REG_RESOURCE_FOLDER + "/rfbrep" + "/tripart.json";

    rfio::BrepReader reader;
    bool succ = reader.readFile(golden_file, nullptr);
    rfdb::dbVariant var = reader.transfer(nullptr);

    Handle(AIS_ColoredShape) vm = var.toPart()->getShape();
    ASSERT_TRUE(succ);

    std::string config_path2 = REG_RESOURCE_FOLDER + "/step/tripart.STEP";

    StepReader reader2;
    reader2.readFile(config_path2, nullptr);
    auto var2 = reader2.transfer(nullptr, false);
    Handle(AIS_ColoredShape) vm2 = var2.toPart()->getShape();
    rfdb::dbPartUtils::printStructOfPart(var2.toPart(), 0);
    ASSERT_TRUE(succ);

    EXPECT_EQ(VMUtils::getShapeNodeCount(vm), 3);
    EXPECT_EQ(VMUtils::getShapeNodeCount(vm), VMUtils::getShapeNodeCount(vm2));
}

TEST(io, stepread_2)
{
    std::string golden_file = REG_RESOURCE_FOLDER + "/rfbrep" + "/assembled_part.json";

    rfio::BrepReader reader;
    bool succ = reader.readFile(golden_file, nullptr);
    ASSERT_TRUE(succ);
    rfdb::dbVariant var = reader.transfer(nullptr);
    rfdb::dbPartUtils::printStructOfPart(var.toPart(), 0);

    Handle(AIS_ColoredShape) vm = var.toPart()->getShape();

    std::string config_path2 = REG_RESOURCE_FOLDER + "/step/assembled_part.STEP";

    StepReader reader2;
    reader2.readFile(config_path2, nullptr);
    auto var2 = reader2.transfer(nullptr, false);
    std::cout << "var2" << '\n';
    rfdb::dbPartUtils::printStructOfPart(var2.toPart(), 0);
    Handle(AIS_ColoredShape) vm2 = var2.toPart()->getShape();

    ASSERT_TRUE(succ);

    EXPECT_EQ(VMUtils::getShapeNodeCount(vm), 6);
    EXPECT_EQ(VMUtils::getShapeNodeCount(vm), VMUtils::getShapeNodeCount(vm2));
}

TEST(io, stepread_3)
{
    std::string config_path2 = REG_RESOURCE_FOLDER + "/step/double_part.stp";

    StepReader reader2;
    reader2.readFile(config_path2, nullptr);
    auto var2 = reader2.transfer(nullptr, false);
    rfdb::dbPartUtils::printStructOfPart(var2.toPart(), 0);
    Handle(AIS_ColoredShape) vm2 = var2.toPart()->getShape();

    EXPECT_EQ(VMUtils::getShapeNodeCount(vm2), 1);
}

} // namespace RobotTest
