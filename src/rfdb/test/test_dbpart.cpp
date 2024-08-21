#include <functional>
#include <iostream>

#include <AIS_ColoredShape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <gtest/gtest.h>

#include "rfdb/dbPart.h"

TEST(dbpart, test_part_visible)
{
    rfdb::dbPart part;

    auto fun = [](bool visiable) { std::cout << "visible: " << visiable << '\n'; };
    part.bindVisibleChangeFunc(fun);
    part.setVisiable(false);
    EXPECT_EQ(part.getVisiable(), false);
}

TEST(dbpart, test_part_set_shape)
{
    Handle(AIS_ColoredShape) shp1 = new AIS_ColoredShape(BRepPrimAPI_MakeBox(10, 10, 10).Shape());
    Handle(AIS_ColoredShape) shp2 = new AIS_ColoredShape(BRepPrimAPI_MakeBox(2, 2, 2).Shape());

    Handle(AIS_ColoredShape) root = new AIS_ColoredShape(TopoDS_Shape());
    root->AddChild(shp1);
    root->AddChild(shp2);

    rfdb::dbPart part;
    std::ignore = part.setShape(root);

    std::vector<rfdb::dbPart *> children;
    part.getChildren(children);
    ASSERT_EQ(children.size(), 2);
    ASSERT_FALSE(children[0]->getShape().IsNull());
    ASSERT_FALSE(children[1]->getShape().IsNull());
}

TEST(dbpart, test_part_get_shape)
{
    Handle(AIS_ColoredShape) shp1 = new AIS_ColoredShape(BRepPrimAPI_MakeBox(10, 10, 10).Shape());
    Handle(AIS_ColoredShape) shp2 = new AIS_ColoredShape(BRepPrimAPI_MakeBox(2, 2, 2).Shape());

    Handle(AIS_ColoredShape) root = new AIS_ColoredShape(TopoDS_Shape());
    root->AddChild(shp1);
    root->AddChild(shp2);

    rfdb::dbPart part;
    std::ignore = part.setShape(root);
    auto model = part.getShape();
    EXPECT_EQ(model->Children().Size(), 2);

    std::vector<rfdb::dbPart *> children;
    part.getChildren(children);
    ASSERT_EQ(children.size(), 2);
    ASSERT_FALSE(children[0]->getShape().IsNull());
    ASSERT_FALSE(children[1]->getShape().IsNull());
}
