#include <gtest/gtest.h>

#include "rfbase/rfTimer.h"
#include "rfbase/rfspan.h"

TEST(rfbase_span, testspan)
{
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::span<int> s(v);
    ASSERT_EQ(spanItemIndex(s, v[2]), 2);
    ASSERT_EQ(cSpanItemIndex(v, v[3]), 3);

    rfbase::FrTimer t;
    for (size_t i = 0; i < 1000000; ++i)
    {
        spanItemIndex(s, v[2]);
        cSpanItemIndex(v, v[3]);
    }
    std::cout << "time of spanItemIndex: " << t.timeFromLastSee<std::milli>() << " ms" << '\n';

    rfbase::FrTimer t2; // time of raw vector
    for (size_t i = 0; i < 1000000; ++i)
    {
        std::ignore = std::distance(v.begin(), std::find(v.begin(), v.end(), v[2]));
        std::ignore = std::distance(v.begin(), std::find(v.begin(), v.end(), v[3]));
    }
    std::cout << "time of vec Index: " << t2.timeFromLastSee<std::milli>() << " ms" << '\n';
}
