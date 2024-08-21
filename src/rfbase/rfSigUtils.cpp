#include "rfSigUtils.h"

namespace rfbase
{
kSigType rfSigUtils::sigTypeP(double p_value)
{
    if (p_value > 0)
    {
        return kSigType::Positive;
    }
    else if (p_value < 0)
    {
        return kSigType::Negative;
    }
    else
    {
        return kSigType::Zero;
    }
}

int rfSigUtils::toSignumIntP(double p_value)
{
    if (p_value > 0)
    {
        return 1;
    }
    else if (p_value < 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

double rfSigUtils::toSignumDoubleP(double p_value)
{
    if (p_value > 0)
    {
        return 1.0;
    }
    else if (p_value < 0)
    {
        return -1.0;
    }
    else
    {
        return 0.0;
    }
}

kSigType rfSigUtils::negateSignum(const kSigType &signum)
{
    switch (signum)
    {
    case kSigType::Positive: return kSigType::Negative;
    case kSigType::Negative: return kSigType::Positive;
    case kSigType::Zero: return kSigType::Zero;
    }
    return kSigType::Zero;
}
} // namespace rfbase
