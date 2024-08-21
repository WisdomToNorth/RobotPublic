
#include "ratiohelper.h"

#include <cmath>

namespace rfbase
{

bool RatioHelper::inRange(double value) const
{
    if (!left_bound_.has_value() && !right_bound_.has_value())
    {
        return true;
    }
    else if (left_bound_.has_value() && !right_bound_.has_value())
    {
        return value >= left_bound_.value();
    }
    else if (!left_bound_.has_value() && right_bound_.has_value())
    {
        return value <= right_bound_.value();
    }
    else // both bounds are set
    {
        return value >= left_bound_.value() && value <= right_bound_.value();
    }
}

double RatioHelper::getLowestRatio() const
{
    if (left_bound_.has_value())
    {
        return getRatio(left_bound_, right_bound_, left_bound_.value(), ValueType::left_bound);
    }
    return getRatio(left_bound_, right_bound_, left_ult_);
}

double RatioHelper::getHighestRatio() const
{
    if (right_bound_.has_value())
    {
        return getRatio(left_bound_, right_bound_, right_bound_.value(), ValueType::right_bound);
    }
    return getRatio(left_bound_, right_bound_, right_ult_);
}

double RatioHelper::getValueRatio(double value) const
{
    return getRatio(left_bound_, right_bound_, value);
}

double RatioHelper::getRatio(std::optional<double> left_bound, std::optional<double> right_bound,
                             double value, ValueType valuetype)
{
    if (!left_bound.has_value() && !right_bound.has_value())
    {
        return 0.5;
    }
    else if (left_bound.has_value() && !right_bound.has_value())
    {
        return sigmoidScaling(value - left_bound.value());
    }
    else if (!left_bound.has_value() && right_bound.has_value())
    {
        return 1.0 - sigmoidScaling(right_bound.value() - value);
    }
    else
    {
        if (right_bound.value() != left_bound.value())
        {
            double range = right_bound.value() - left_bound.value();
            return sigmoidScaling(value - left_bound.value(), range);
        }

        else
        {
            const double half_range = 50.0;

            const double real_pivot = left_bound.value();
            double adjusted_left_bound = left_bound.value() - half_range;
            double adjusted_right_bound = right_bound.value() + half_range;

            if ((value < real_pivot) || (valuetype == ValueType::left_bound))
            {
                double adjusted_value = value - half_range;
                return getRatio(adjusted_left_bound, adjusted_right_bound, adjusted_value);
            }
            else if ((value > real_pivot) || (valuetype == ValueType::right_bound))
            {
                double adjusted_value = value + half_range;
                return getRatio(adjusted_left_bound, adjusted_right_bound, adjusted_value);
            }
            else // value == real_pivot
            {
                return 0.5;
            }
        }
    }
}

double RatioHelper::sigmoidScaling(double delta, double scale)
{
    // Use hyperbolic tangent to adjust the scale to match the distribution of values
    return 0.5 + 0.5 * tanh((delta - scale * 0.5) / scale);
}

bool RatioHelper::lowestEqualHighest() const
{
    return left_bound_ == right_bound_;
}
} // namespace rfbase
