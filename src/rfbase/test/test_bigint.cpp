#include <gtest/gtest.h>

#include "rfbase/rfBigInt.h"

using namespace rfbase;

TEST(testbigint, bigintdeterminant)
{
    BigInt a(1);
    BigInt b(2);
    BigInt c(3);
    BigInt d(4);

    BigInt res = BigInt::determinant(a, b, c, d);
    ASSERT_EQ(res, BigInt(-2));
}
