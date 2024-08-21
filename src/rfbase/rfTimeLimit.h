#pragma once
#ifndef RF_TIMELIMIT_H
#define RF_TIMELIMIT_H

#include <chrono>
namespace rfbase
{
class TimeLimit
{
private:
    std::chrono::high_resolution_clock::time_point time_stamp;
    int time_limit;

public:
    explicit TimeLimit(int p_milli_seconds);

    bool limit_exceeded();

    void multiply(double p_factor);
};
} // namespace rfbase
#endif
