#include <climits>

#include <gtest/gtest.h>

#include "rfbase/concurrent_unionset.h"
#include "rfbase/rfTimer.h"

namespace RobotTest
{
using namespace rfbase;

// 1,
// 2, 3
// 4, 5, 6
// 7, 8, 9, 10
TEST(MultiUnisetTester, loc_in_triangle)
{
    auto res1 = MultiUnionFind<int>::getIndexLocation(1);
    EXPECT_EQ(res1.first, 1);
    EXPECT_EQ(res1.second, 0);

    auto res2 = MultiUnionFind<int>::getIndexLocation(5);
    EXPECT_EQ(res2.first, 3);
    EXPECT_EQ(res2.second, 1);

    auto res3 = MultiUnionFind<int>::getIndexLocation(10);
    EXPECT_EQ(res3.first, 4);
    EXPECT_EQ(res3.second, 3);
}

TEST(MultiUnisetTester, multiunionset_int)
{
    size_t data_size = 2000;
    std::vector<int *> data(data_size);
    for (size_t i = 0; i < data_size; ++i)
    {
        data[i] = new int(static_cast<int>(i));
    }

    auto fun = [](int *a, int *b) -> bool
    {
        if ((*a % 2 == 0 && *b % 2 == 0) || (*a % 2 == 1 && *b % 2 == 1))
        {
            return true;
        }
        else
        {
            return false;
        }
    };

    MultiUnionFind<int> finder(&data, INT_MAX);
    finder.setMergeCheckCallBack(fun);
    std::vector<std::unordered_set<int *>> res;

    rfbase::FrTimer t;
    finder.buildUnionFinder(res);
    std::cout << "time of single: " << t.timeFromLastSee<std::milli>() << " ms\n";

    ASSERT_EQ(res.size(), 2);
    ASSERT_EQ(res[0].size(), 1000);
    ASSERT_EQ(res[1].size(), 1000);

    // Test_multi
    MultiUnionFind<int> multi_finder(&data, 10);
    multi_finder.setMergeCheckCallBack(fun);

    std::vector<std::unordered_set<int *>> res_2;

    t.reset();
    multi_finder.buildUnionFinder(res_2);
    std::cout << "time of multi: " << t.timeFromLastSee<std::milli>() << " ms\n";

    ASSERT_EQ(res_2.size(), 2);
    ASSERT_EQ(res_2[0].size(), 1000);
    ASSERT_EQ(res_2[1].size(), 1000);
    for (size_t i = 0; i < data_size; ++i)
    {
        delete data[i];
    }
}

struct DummyStruct
{
    size_t i, j, k;
};

TEST(MultiUnisetTester, multiunionset_multi_2)
{
    // Data preparation
    size_t data_size = 2000;
    std::vector<DummyStruct *> data(data_size);
    for (size_t i = 0; i < data_size; ++i)
    {
        data[i] = new DummyStruct{i, i + 2, i + 4};
    }

    auto fun = [](DummyStruct *a, DummyStruct *b) -> bool
    {
        if ((a->i % 2 == 0 && b->i % 2 == 0) || (a->i % 2 == 1 && b->i % 2 == 1))
        {
            return true;
        }
        else
        {
            return false;
        }
    };

    // Test
    MultiUnionFind<DummyStruct> finder(&data, INT_MAX);
    finder.setMergeCheckCallBack(fun);

    std::vector<std::unordered_set<DummyStruct *>> res;

    rfbase::FrTimer t;
    finder.buildUnionFinder(res);
    std::cout << "time of multi: " << t.timeFromLastSee<std::milli>() << " ms\n";

    ASSERT_EQ(res.size(), 2);
    ASSERT_EQ(res[0].size(), 1000);
    ASSERT_EQ(res[1].size(), 1000);

    // Test multi
    MultiUnionFind<DummyStruct> multi_finder(&data, 10);
    multi_finder.setMergeCheckCallBack(fun);

    std::vector<std::unordered_set<DummyStruct *>> res_2;

    t.reset();
    multi_finder.buildUnionFinder(res_2);
    std::cout << "time of multi: " << t.timeFromLastSee<std::milli>() << " ms\n";

    ASSERT_EQ(res_2.size(), 2);
    ASSERT_EQ(res_2[0].size(), 1000);
    ASSERT_EQ(res_2[1].size(), 1000);

    for (size_t i = 0; i < data_size; ++i)
    {
        delete data[i];
    }
}

} // namespace RobotTest
