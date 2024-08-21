#pragma once

#ifndef CONCURRENT_UTILS_H
#define CONCURRENT_UTILS_H

#include <thread>

namespace rfbase
{
class ConcurrentUtils
{
public:
    [[nodiscard]] static unsigned long long getThreadCount(unsigned long long datasize,
                                                           unsigned long long min_per_thread)
    {
        unsigned long long const max_thread = (datasize + min_per_thread - 1) / min_per_thread;
        unsigned long long const hardware_thread = std::thread::hardware_concurrency();
        // if headware_thread == 0, split thread to 2
        unsigned long long thread_cnt = hardware_thread < max_thread ? hardware_thread : max_thread;

        return (1 + thread_cnt) / 2; // core often 2 * thread cnt
    }
};
}; // namespace rfbase
#endif
