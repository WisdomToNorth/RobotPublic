
#pragma once
#ifndef OBSERVER_H
#define OBSERVER_H

#include <algorithm>
#include <vector>
namespace rfapp
{
enum class KMode
{
    add,
    remove,
    hide,
    update, // change color, transform, etc.
    highlight,
};

template <typename T>
class Observer
{
public:
    virtual ~Observer()
    {
    }
    virtual void updateEvent(T *sub_, KMode mode) = 0;
};

template <typename T>
class Subject
{
public:
    void notifyAll(T *t, KMode mode)
    {
        for (const auto &obs : obs_list_)
        {
            obs->updateEvent(t, mode);
        }
    }

    void notifyAll(T *t, Observer<T> *cur, KMode mode)
    {
        // std::cout << "obs_list_.size() = " << obs_list_.size() << std::endl;
        // To avoid the problem of erasing elements while iterating over them
        auto obs_list_copy = obs_list_;
        for (const auto &obs : obs_list_copy)
        {
            // std::cout << "obs = " << obs << ", cur = " << cur << std::endl;
            if (obs != cur)
            {
                obs->updateEvent(t, mode);
            }
        }
    }

    void attach(Observer<T> *obs)
    {
        obs_list_.emplace_back(obs);
        // std::cout << "obs_list_.size() = " << obs_list_.size() << std::endl;
    }
    void detach(Observer<T> *obs)
    {
        obs_list_.erase(std::remove(obs_list_.begin(), obs_list_.end(), obs), obs_list_.end());
        // std::cout << "obs_list_.size() = " << obs_list_.size() << std::endl;
    }

private:
    std::vector<Observer<T> *> obs_list_;
};
} // namespace rfapp

#endif
