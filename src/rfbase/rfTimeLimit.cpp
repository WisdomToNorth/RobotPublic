#include "rfTimeLimit.h"

#include <algorithm>
#include <climits>

namespace rfbase
{
TimeLimit::TimeLimit(int p_milli_seconds)
    : time_stamp(std::chrono::high_resolution_clock::now())
    , time_limit(p_milli_seconds)
{
}

bool TimeLimit::limit_exceeded()
{
    auto curr_time = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(curr_time - time_stamp).count();
    return duration > time_limit;
}

void TimeLimit::multiply(double p_factor)
{
    if (p_factor <= 0)
    {
        return;
    }
    double new_limit = p_factor * time_limit;
    new_limit = std::min(new_limit, static_cast<double>(INT_MAX));
    time_limit = static_cast<int>(new_limit);
}
} // namespace rfbase
