
#ifndef RANGEHELPER_H
#define RANGEHELPER_H

#include <climits>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace rfbase
{
class RatioHelper
{
    const double left_ult_ = double(-INT_MAX);
    const double right_ult_ = double(INT_MAX);
    std::optional<double> left_bound_;
    std::optional<double> right_bound_;

public:
    void setRange(std::optional<double> lowest_value, std::optional<double> highest_value)
    {
        left_bound_ = lowest_value;
        right_bound_ = highest_value;
    }
    inline std::optional<double> getLeftBound() const
    {
        return left_bound_;
    }
    inline std::optional<double> getRightBound() const
    {
        return right_bound_;
    }
    inline bool hasLeftBound() const
    {
        return left_bound_.has_value();
    }
    inline bool hasRightBound() const
    {
        return right_bound_.has_value();
    }

    bool inRange(double value) const;
    double getLowestRatio() const;
    double getHighestRatio() const;
    double getValueRatio(double value) const;
    bool lowestEqualHighest() const;

private:
    enum class ValueType
    {
        left_bound,
        right_bound,
        value
    };
    static double getRatio(std::optional<double> left_bound, std::optional<double> right_bound,
                           double value, ValueType get_bound_ratio = ValueType::value);
    static double sigmoidScaling(double delta, double scale = 1000);
};
} // namespace rfbase
#endif
