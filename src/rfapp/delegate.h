#pragma once
#ifndef DELEGATE_H
#define DELEGATE_H

#include <type_traits>
#include <unordered_map>

namespace rfapp
{
template <typename T>
class Delegate final
{
public:
    Delegate() = default;
    ~Delegate() = default;

    Delegate(const Delegate &) = default;
    Delegate(Delegate &&) = default;
    Delegate &operator=(const Delegate &) = default;
    Delegate &operator=(Delegate &&) = default;

    int attach(T &&func)
    {
        return attachDelegate(std::forward<T>(func));
    }

    int attach(const T &func)
    {
        return attachDelegate(func);
    }

    void clear()
    {
        delegates_.clear();
        index_ = 0;
    }

    template <typename... Ts>
    void notify(Ts &&...args)
    {
        for (const auto &[_, func] : delegates_)
        {
            func(std::forward<Ts>(args)...);
        }
    }

private:
    template <typename F>
    int attachDelegate(F &&func)
    {
        if (func)
        {
            ++index_;
            delegates_.try_emplace(index_, std::forward<F>(func));
            return index_;
        }
        else
        {
            return 0;
        }
    }

private:
    int index_ = 0;
    std::unordered_map<int, T> delegates_;
};
} // namespace rfapp

#endif
