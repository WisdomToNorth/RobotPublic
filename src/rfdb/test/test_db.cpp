#include <functional>
#include <iostream>
#include <memory>

#include <AIS_ColoredShape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <gtest/gtest.h>

#include "rfdb/dbAssembly.h"
#include "rfdb/dbMesh.h"
#include "rfdb/dbPart.h"
#include "rfdb/dbVariant.h"

using namespace rfdb;
TEST(dbpart, test_dbtype)
{
    std::unique_ptr<dbPart> part(new dbPart());
    std::unique_ptr<dbAssembly> assembly(new dbAssembly());
    std::unique_ptr<dbMesh> mesh(new dbMesh());
    std::unique_ptr<dbPointCloud> pointCloud(new dbPointCloud());

    dbVariant variant1(part.get());
    dbVariant variant2(assembly.get());
    dbVariant variant3(mesh.get());
    dbVariant variant4(pointCloud.get());

    EXPECT_EQ(variant1.getType(), dbVariant::Type::Part);
    EXPECT_EQ(variant2.getType(), dbVariant::Type::Assembly);
    EXPECT_EQ(variant3.getType(), dbVariant::Type::Mesh);
    EXPECT_EQ(variant4.getType(), dbVariant::Type::PointCloud);

    EXPECT_EQ(variant1.isPart(), true);
    EXPECT_EQ(variant2.isAssembly(), true);
    EXPECT_EQ(variant3.isMesh(), true);
    EXPECT_EQ(variant4.isPointCloud(), true);
}
