
#include <gtest/gtest.h>

#include "rfbase/concurrent_utils.h"

namespace rfbase
{
TEST(MultiUnisetTester, thread_count)
{
    auto c1 = rfbase::ConcurrentUtils::getThreadCount(10, 10);
    EXPECT_EQ(c1, 1);
    auto c2 = rfbase::ConcurrentUtils::getThreadCount(10, 100);
    EXPECT_EQ(c2, 1);
}
} // namespace rfbase
