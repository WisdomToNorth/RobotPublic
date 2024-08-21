#include <gtest/gtest.h>

#include "rfbase/ratiohelper.h"
#include "rfbase/rfmath.h"

using namespace rfbase;

TEST(testgeo, gcd)
{
    long long a = 1;
    long long b = 2;

    long long res = binaryGcd(a, b);
    ASSERT_EQ(res, 1);

    long long c = 64;
    long long d = 48;

    long long res2 = binaryGcd(c, d);
    ASSERT_EQ(res2, 16);
}

TEST(testgeo, roundAwayFromZero)
{
    double x = 1.5;
    double epsilon = 0.01;

    long long res = roundAwayFromZero(x, epsilon);
    ASSERT_EQ(res, 2);

    double y = -1.4;

    long long res2 = roundAwayFromZero(y, epsilon);
    ASSERT_EQ(res2, -2);
}

int64_t round_half_to_even(double value)
{
    double int_part;
    if (std::modf(value, &int_part) == 0.5)
    {
        return static_cast<int64_t>(int_part) % 2 == 0 ?
                   static_cast<int64_t>(int_part) :
                   static_cast<int64_t>(int_part + std::copysign(1.0, int_part));
    }
    return static_cast<int64_t>(std::round(value));
}
TEST(RoundingTest, RoundHalfAwayFromZero)
{
    // 测试五舍六入（std::round）
    EXPECT_EQ(std::round(4.5), 5);
    EXPECT_EQ(std::round(5.5), 6);
    EXPECT_EQ(std::round(-4.5), -5);
    EXPECT_EQ(std::round(-5.5), -6);
}

TEST(RoundingTest, RoundHalfToEven)
{
    // 测试四舍五入（round_half_to_even）
    EXPECT_EQ(round_half_to_even(4.5), 4);
    EXPECT_EQ(round_half_to_even(5.5), 6);
    EXPECT_EQ(round_half_to_even(6.5), 6);
    EXPECT_EQ(round_half_to_even(7.5), 8);
    EXPECT_EQ(round_half_to_even(-4.5), -5);
    EXPECT_EQ(round_half_to_even(-5.5), -6);
    EXPECT_EQ(round_half_to_even(-6.5), -7);
    EXPECT_EQ(round_half_to_even(-7.5), -8);
}

TEST(testratiohelper, ratiohelper)
{
    RatioHelper rh;
    rh.setRange(0.0, 1.0);
    double left_ratio1 = rh.getLowestRatio();
    double right_ratio1 = rh.getHighestRatio();
    EXPECT_DOUBLE_EQ(rh.getValueRatio(0.5), 0.5);

    rh.setRange(20, 30);
    double left_ratio2 = rh.getLowestRatio();
    double right_ratio2 = rh.getHighestRatio();
    EXPECT_DOUBLE_EQ(rh.getValueRatio(25), 0.5);
    EXPECT_DOUBLE_EQ(left_ratio1, left_ratio2);
    EXPECT_DOUBLE_EQ(right_ratio1, right_ratio2);

    rh.setRange(200, 300);
    double left_ratio3 = rh.getLowestRatio();
    double right_ratio3 = rh.getHighestRatio();
    EXPECT_DOUBLE_EQ(left_ratio1, left_ratio3);
    EXPECT_DOUBLE_EQ(right_ratio1, right_ratio3);

    rh.setRange(0.0, 0.0);
    double left_ratio4 = rh.getLowestRatio();
    double right_ratio4 = rh.getHighestRatio();
    EXPECT_DOUBLE_EQ(left_ratio1, left_ratio4);
    EXPECT_DOUBLE_EQ(right_ratio1, right_ratio4);
    EXPECT_DOUBLE_EQ(rh.getValueRatio(0.0), 0.5);
}
