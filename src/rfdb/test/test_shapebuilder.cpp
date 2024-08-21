#include <gtest/gtest.h>
#include <iostream>

#include "rfdb/dbutils/shapebuilder.h"

#include "rfdb/dbPart.h"

TEST(shp_builder, makeBottle)
{
    TopoDS_Shape shape = rfdb::ShapeBuilder::makeBottle();
    EXPECT_EQ(shape.IsNull(), false);
}

TEST(shp_builder, makeCubePlusQuarterBall)
{
    TopoDS_Shape shape = rfdb::ShapeBuilder::makeCubePlusQuarterBall();
    EXPECT_EQ(shape.IsNull(), false);
}

TEST(shp_builder, makeBall)
{
    TopoDS_Shape shape = rfdb::ShapeBuilder::makeBall(gp_Pnt(0, 0, 0));
    EXPECT_EQ(shape.IsNull(), false);
}

TEST(shp_builder, createDumbBellVariant)
{
    rfdb::dbPart *part =
        rfdb::ShapeBuilder::createDumbBellVariant(gp_Pnt(0, 0, 0), gp_Pnt(100, 100, 100));
    EXPECT_EQ(part->childrenCount(), 3);
    delete part;
}

TEST(shp_builder, arrowBuilder)
{
    TopoDS_Shape shape = rfdb::ShapeBuilder::arrowBuilder(gp_Pnt(0, 0, 0), gp_Pnt(100, 100, 100));
    EXPECT_EQ(shape.IsNull(), false);
}

TEST(shp_builder, buildAbassica_without_face)
{
    rfdb::dbPart *part =
        rfdb::ShapeBuilder::buildAbassica(gp_Ax2(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1)), false, 100);
    EXPECT_EQ(part->childrenCount(), 4);
    delete part;
}

TEST(shp_builder, buildAbassica_with_face)
{
    rfdb::dbPart *part =
        rfdb::ShapeBuilder::buildAbassica(gp_Ax2(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1)), true, 100);
    EXPECT_EQ(part->childrenCount(), 7);
    delete part;
}
