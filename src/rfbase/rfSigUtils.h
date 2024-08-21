#pragma once
#ifndef RF_SIGNUM_H
#define RF_SIGNUM_H

#include <string>

namespace rfbase
{
enum class kSigType : int
{
    Positive = 1,
    Negative = -1,
    Zero = 0
};
class rfSigUtils
{
public:
    template <typename T>
    static kSigType sigType(T p_value)
    {
        return sigTypeP(static_cast<double>(p_value));
    }

    template <typename T>
    static int toSignumInt(T p_value)
    {
        return toSignumIntP(static_cast<double>(p_value));
    }
    template <typename T>
    static double toSignumDouble(T p_value)
    {
        return toSignumDoubleP(static_cast<double>(p_value));
    }

private:
    static kSigType sigTypeP(double p_value);
    static int toSignumIntP(double p_value);
    static double toSignumDoubleP(double p_value);
    static kSigType negateSignum(const kSigType &signum);
};
} // namespace rfbase

#endif
