
#include <functional>
#include <iostream>
#include <vector>

#include <gtest/gtest.h>

#include "generated/config.h"
#include "rfbase/rfTimer.h"
#include "rfdb/dbProperty.h"
#include "rfdb/dbVariant.h"
#include "rfdb/dbutils/dbpartutils.h"
#include "rfio/reader_brep.h"

using namespace rfdb;

TEST(dbutil, get_all_shp)
{
    std::string brep_file = REG_RESOURCE_FOLDER + "/rfbrep/assembled_part.json";
    rfio::BrepReader reader;
    bool succ = reader.readFile(brep_file, nullptr);
    ASSERT_EQ(succ, true);
    rfdb::dbVariant var = reader.transfer(nullptr);

    std::vector<TopoDS_Shape> sub_shps;
    dbPartUtils::getAllShapes(var.toPart(), sub_shps);
    EXPECT_EQ(sub_shps.size(), 6);
}

TEST(dbutil, brep_combine)
{
    std::string brep_file = REG_RESOURCE_FOLDER + "/rfbrep/assembled_part.json";
    rfio::BrepReader reader;
    bool succ = reader.readFile(brep_file, nullptr);
    ASSERT_EQ(succ, true);
    rfdb::dbVariant var = reader.transfer(nullptr);
    dbPartUtils::printStructOfPart(var.toPart(), 0);

    EXPECT_EQ(var.toPart()->childrenCount(), 1); // Check

    rfbase::FrTimer timer;
    rfdb::dbPart *combined = dbPartUtils::createCombinedPart(var.toPart());
    std::cout << "Time used: " << timer.timeFromBegin() << '\n';
    dbPartUtils::printStructOfPart(combined, 0);

    std::vector<TopoDS_Shape> sub_shps;
    dbPartUtils::getAllShapes(combined, sub_shps);
    EXPECT_EQ(sub_shps.size(), 1);
}
