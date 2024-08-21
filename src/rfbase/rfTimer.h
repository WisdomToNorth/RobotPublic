#pragma once
#ifndef FRTIMER_H
#define FRTIMER_H

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace rfbase
{
typedef std::chrono::high_resolution_clock _timer_core_;
// typedef std::chrono::steady_clock _timer_core_;

class FrTimer
{
public:
    FrTimer(const std::string &event_set_name = "Root task", int deep = 0);

    // clear all data, Equal to new timer
    void reset();

    template <typename T = std::milli>
    [[nodiscard]] double timeFromBegin()
    {
        if (!is_stop_)
        {
            return timeFromBeginP<T>();
        }
        else
        {
            return std::chrono::duration<double, T>(t3_stoper_ - t1_).count();
        }
    }

    // NOTE:
    // if timer runs, calling of timeFromBegin will alse influence result of "timeFromLastSee"
    // if timer stops, calling of all api will not affect the result of "timeFromLastSee"
    template <typename T = std::milli>
    [[nodiscard]] double timeFromLastSee()
    {
        if (!is_stop_)
        {
            return timeFromLastSeeP<T>();
        }
        else
        {
            return std::chrono::duration<double, T>(t3_stoper_ - t2_).count();
        }
    }

    FrTimer *getSubTimerByName(const std::string &str);

    FrTimer *startSubTimer(const std::string &str);

    void stopSubTimer(const std::string &str);

    void stop();

    void restore();

    template <typename T = std::milli>
    void printTimeReport(std::string time_name)
    {
        std::string time_type;
        if constexpr (std::is_same_v<T, std::nano>)
            time_type = "ns";
        else if constexpr (std::is_same_v<T, std::micro>)
            time_type = "μs";
        else if constexpr (std::is_same_v<T, std::milli>)
            time_type = "ms";
        else if constexpr (std::is_same_v<T, std::centi>)
            time_type = "cs";
        else if constexpr (std::is_same_v<T, std::deci>)
            time_type = "ds";
        else if constexpr (std::is_same_v<T, std::ratio<1>>)
            time_type = "s";
        else
            time_type = "";

        if (deep_ == 0)
        {
            std::cout << "########### " << time_name << " ###########\n";
        }
        std::cout << getPreFix() << std::left << std::setw(15) << event_name_ + ':' << std::right
                  << std::setw(10) << timeFromBegin<T>() << time_type << '\n';

        for (auto &subtimer : sub_timers_)
        {
            subtimer.printTimeReport<T>(time_name);
        }

        if (deep_ == 0 && sub_timers_.size() > 0)
        {
            std::cout << "---------------------\n";
            std::cout << getPreFix() << std::left << std::setw(15) << event_name_ + ':'
                      << std::right << std::setw(10) << timeFromBegin<T>() << time_type << '\n';
            std::cout << "#####################\n" << std::endl;
        }
    }

    // For test
    template <typename T = std::milli>
    [[nodiscard]] bool checkValid()
    {
        bool succ = true;
        int root_time = static_cast<int>(timeFromBegin<T>());
        int subtime_cnt = 0;
        for (auto &subtimer : sub_timers_)
        {
            if (subtimer.sub_timers_.size() > 0)
            {
                succ &= subtimer.checkValid();
            }
            subtime_cnt += static_cast<int>(subtimer.timeFromBegin<T>());
        }
        succ &= subtime_cnt <= root_time;
        return succ;
    }

private:
    template <typename T>
    double timeFromBeginP()
    {
        t2_ = _timer_core_::now();
        double dr_ms = std::chrono::duration<double, T>(t2_ - t1_).count();
        return dr_ms;
    }

    template <typename T>
    double timeFromLastSeeP()
    {
        auto t3 = _timer_core_::now();
        double dr_ms = std::chrono::duration<double, T>(t3 - t2_).count();
        t2_ = t3;

        return dr_ms;
    }

    std::string getPreFix();

public:
    std::vector<FrTimer> sub_timers_;
    std::string event_name_;
    int deep_;

private:
    _timer_core_::time_point t3_stoper_;

    _timer_core_::time_point t2_;
    _timer_core_::time_point t1_;
    bool is_stop_;
};
} // namespace rfbase

#endif
