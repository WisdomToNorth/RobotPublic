#include <functional>
#include <iostream>

#include <gtest/gtest.h>

#include "rfdb/dbProperty.h"

TEST(dbproperty, test_property)
{
    rfdb::dbProperty property;

    auto fun = [](bool visiable) { std::cout << "visible: " << visiable << '\n'; };
    property.setVisibleChangeEvent(fun);
    property.setVisiable(false);
    EXPECT_EQ(property.isVisiable(), false);

    auto fun2 = [](int value) { std::cout << "value: " << value << '\n'; };
    property.setValueChangeEvent(fun2);
    property.setValue(1);
    EXPECT_EQ(property.getValue(), 1);
}
