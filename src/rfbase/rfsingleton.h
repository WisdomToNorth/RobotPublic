#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory>
#include <mutex>

namespace rfbase
{
template <typename T>
class RfSingleton final
{
public:
    static T &instance()
    {
        std::call_once(once_flag_, []() { instance_ = std::make_unique<T>(); });
        return *instance_;
    }

    static T *instancePtr()
    {
        std::call_once(once_flag_, []() { instance_ = std::make_unique<T>(); });
        return instance_.get();
    }

    static void reset()
    {
        instance_.reset();
    }

protected:
    RfSingleton() = default;
    virtual ~RfSingleton() = default;

private:
    static std::unique_ptr<T> instance_;
    static std::once_flag once_flag_;
};

template <typename T>
std::unique_ptr<T> RfSingleton<T>::instance_ = nullptr;

template <typename T>
std::once_flag RfSingleton<T>::once_flag_;

} // namespace rfbase

#endif // SINGLETON_H
