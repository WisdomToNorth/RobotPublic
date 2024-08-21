#include "rfTimer.h"

namespace rfbase
{

FrTimer::FrTimer(const std::string &event_set_name, int deep)
    : event_name_(event_set_name)
    , deep_(deep)
    , is_stop_(false)
{
    t2_ = _timer_core_::now();
    t1_ = t2_;
}

// clear all data, Equal to new timer
void FrTimer::reset()
{
    std::vector<FrTimer> new_subs;
    sub_timers_.swap(new_subs);
    is_stop_ = false;
    t2_ = _timer_core_::now();
    t1_ = _timer_core_::now();
    t3_stoper_ = _timer_core_::now();
}

FrTimer *FrTimer::getSubTimerByName(const std::string &str)
{
    auto it = sub_timers_.begin();
    while (it != sub_timers_.end())
    {
        if (it->event_name_ == str)
        {
            return &(*it);
        }
        it++;
    }

    sub_timers_.emplace_back(FrTimer(str, deep_ + 1));
    return &sub_timers_.back();
}

FrTimer *FrTimer::startSubTimer(const std::string &str)
{
    FrTimer *sub_timer = getSubTimerByName(str);
    sub_timer->restore();
    return sub_timer;
}

void FrTimer::stopSubTimer(const std::string &str)
{
    FrTimer *sub_timer = getSubTimerByName(str);
    sub_timer->stop();
}

void FrTimer::stop()
{
    is_stop_ = true;
    t3_stoper_ = _timer_core_::now();
}

void FrTimer::restore()
{
    if (!is_stop_)
        return;

    is_stop_ = false;
    auto gap = _timer_core_::now() - t3_stoper_;
    t1_ += gap;
    t2_ += gap;
}

std::string FrTimer::getPreFix()
{
    std::string res;
    int it_cnt = deep_;
    while (it_cnt--)
    {
        res.append("   ");
    }
    return res;
}

} // namespace rfbase
