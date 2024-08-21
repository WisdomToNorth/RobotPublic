#pragma once
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

#include "concurrent_utils.h"
#include "unionset_basic.h"

namespace rfbase
{
template <typename Element>
class MultiUnionFind
{
private:
    typedef std::function<bool(Element *, Element *)> mergeCheckCallBack;
    const std::vector<Element *> *data_ = nullptr;
    mergeCheckCallBack shouldMerge = NULL;

    // max thread count will override min_per_thread_
    unsigned long long min_per_thread_ = 100;
    unsigned long long max_thread_count_ = UINT16_MAX;

public:
    MultiUnionFind(const std::vector<Element *> *objs, unsigned long long min_per_thread)
    {
        data_ = objs;
        min_per_thread_ = min_per_thread;
    }

    ~MultiUnionFind() = default;

    void setMergeCheckCallBack(mergeCheckCallBack func)
    {
        shouldMerge = func;
    }

    void setMaxThreadCount(unsigned long long max_thread_count)
    {
        max_thread_count_ = max_thread_count;
    }

    void buildUnionFinder(std::vector<std::unordered_set<Element *>> &result)
    {
        int data_count = (int)data_->size();
        unsigned long long data_count_copy = data_count;
        unsigned long long data_count_minus_one = data_count - 1;
        data_count_copy % 2 == 0 ? data_count_copy /= 2 : data_count_minus_one /= 2;
        // Each object compare with others, so the total compare count is n*(n-1)/2
        // Max data_.size() which is allowed is 2^32-1

        unsigned long long caculate_cnt = data_count_copy * data_count_minus_one;
        unsigned long long ideal_thread_count =
            ConcurrentUtils::getThreadCount(caculate_cnt, min_per_thread_);
        unsigned long long num_of_thread =
            static_cast<unsigned long long>(std::min(ideal_thread_count, max_thread_count_));

        unsigned long long block_size = caculate_cnt / num_of_thread;
        std::cout << "num_of_thread: " << num_of_thread << '\n';
        std::vector<std::thread> threads(num_of_thread - 1);
        std::vector<UnionFind> unionfinders(num_of_thread, UnionFind(data_count));

        /* Concurrent caculate union*/
        // represents the number of tasks, from 1 to n, using a trailing index, so n+1
        unsigned long long l_start = 1;
        for (unsigned long long thread_index = 0; thread_index + 1 < num_of_thread; ++thread_index)
        {
            unsigned long long l_end = l_start + block_size;
            threads[thread_index] =
                std::thread(std::mem_fn(&MultiUnionFind::caculateUnion), this, l_start, l_end,
                            std::ref(unionfinders[thread_index + 1]));
            l_start = l_end;
        }
        this->caculateUnion(l_start, caculate_cnt + 1,
                            std::ref(unionfinders[0])); // 0 is main thread

        std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
        // std::cout << "######## Join ########\n";

        /* Combine all unionfinders*/
        for (unsigned long long numofunion = 1; numofunion < num_of_thread; ++numofunion)
        {
            unionfinders[0].combine(unionfinders[numofunion]);
        }

        // Get result
        std::vector<std::pair<int, std::unordered_set<int>>> final_set;
        unionfinders[0].getResult(final_set);

        // //debug start
        // std::cout << "final_set size: " << final_set.size() <<'\n';
        // for (auto &re : final_set)
        // {
        //     // std::cout << "##:" << re.first << " " <<'\n';
        //     for (auto &subre : re.second)
        //     {
        //         std::cout << subre << " ";
        //     }
        //     std::cout <<'\n';
        // }
        // //debug end

        for (auto it = final_set.begin(); it != final_set.end(); ++it)
        {
            std::unordered_set<Element *> tmp_set;
            for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
            {
                tmp_set.insert((*data_)[*it2]);
            }
            result.push_back(tmp_set);
        }
    }

    // 1,
    // 2, 3
    // 4, 5, 6
    // 7, 8, 9, 10
    static std::pair<int, int> getIndexLocation(unsigned long long index)
    {
        unsigned long long aux_num = static_cast<unsigned long long>(std::floor(sqrt(2 * index)));
        unsigned long long aux_num_square = aux_num * aux_num;
        if (!((aux_num_square - aux_num < 2 * index) && (aux_num_square + aux_num >= 2 * index)))
        {
            aux_num += 1;
        }
        assert((aux_num * (aux_num - 1) < 2 * index) && (aux_num * (aux_num + 1) >= 2 * index));
        unsigned long long m = aux_num - 1;
        unsigned long long n = index - aux_num * (aux_num - 1) / 2 - 1;
        int a = static_cast<int>(m) + 1;
        int b = static_cast<int>(n);

        return {a, b}; // m,n counting from 0, a counting from 1, b counting from 0
    }

private:
    bool caculateUnion(unsigned long long l_start_index, unsigned long long l_end_index,
                       UnionFind &finder)
    {
        unsigned long long cal_cnt = l_end_index - l_start_index;
        std::pair<size_t, size_t> loc = getIndexLocation(l_start_index);
        unsigned long long cal_real_cnt = 0;

        size_t m = loc.first, n = loc.second;
        for (size_t i = m; i < data_->size(); ++i)
        {
            for (size_t j = 0; j < m; ++j)
            {
                // Initialize the position of j the first time enter the loop,
                // subsequently set n to zero.
                j = j + n;
                n = 0;

                if (shouldMerge((*data_)[i], (*data_)[j]))
                {
                    finder.merge(static_cast<int>(i), static_cast<int>(j));
                }

                cal_real_cnt++;
                if (cal_cnt == cal_real_cnt)
                {
                    return true;
                }
            };
            m++;
        }
        assert(cal_cnt == cal_real_cnt);
        return true;
    }
};
}; // namespace rfbase
