
#include <functional>
#include <iostream>
#include <vector>

#include <AIS_ColoredShape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <gtest/gtest.h>

#include "generated/config.h"
#include "rfbase/rfTimer.h"
#include "rfdb/dbProperty.h"
#include "rfdb/dbVariant.h"
#include "rfdb/dbutils/dbcheck.h"
#include "rfdb/dbutils/dbpartutils.h"

using namespace rfdb;

TEST(dbpart, test_shp_invalid)
{
    Handle(AIS_ColoredShape) shp1 = new AIS_ColoredShape(BRepPrimAPI_MakeBox(10, 10, 10).Shape());
    Handle(AIS_ColoredShape) shp2 = new AIS_ColoredShape(BRepPrimAPI_MakeBox(2, 2, 2).Shape());
    Handle(AIS_ColoredShape) root = new AIS_ColoredShape(
        BRepPrimAPI_MakeBox(3, 3, 3).Shape()); // node with both children and shape is not allowed
    root->AddChild(shp1);
    root->AddChild(shp2);

    ASSERT_FALSE(root->Shape().IsNull());
    ASSERT_FALSE(root->Children().Size() == 0);

    rfdb::dbPart part;
    std::ignore = part.setShape(root);
    ASSERT_EQ(part.childrenCount(), 0);
}

TEST(dbcheck, dbpart_1)
{
    rfdb::dbPart *root = new rfdb::dbPart();
    std::ignore = root->setShape(new AIS_ColoredShape(BRepPrimAPI_MakeBox(10, 10, 10).Shape()));
    ASSERT_TRUE(rfdb::dbCheck::checkPart(root));

    rfdb::dbPart *child = new rfdb::dbPart();
    std::ignore = child->setShape(new AIS_ColoredShape(BRepPrimAPI_MakeBox(10, 10, 10).Shape()));
    root->addChild(child);
    ASSERT_FALSE(rfdb::dbCheck::checkPart(root));
}
