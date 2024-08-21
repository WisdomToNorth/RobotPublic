#include "rfBigInt.h"

#include <algorithm>
#include <cstdint>

#include <boost/multiprecision/cpp_int.hpp>

namespace rfbase
{
class BigIntImpl
{
public:
    boost::multiprecision::cpp_int value;
};

BigInt BigInt::ZERO = BigInt(0);

BigInt::BigInt()
    : pimpl(new BigIntImpl())
{
}

BigInt::BigInt(int value)
{
    pimpl = new BigIntImpl();
    pimpl->value = value;
}
BigInt &BigInt::operator=(int value)
{
    pimpl->value = value;
    return *this;
}
BigInt &BigInt::operator=(long long value)
{
    pimpl->value = value;
    return *this;
}
BigInt::BigInt(long long value)
    : pimpl(new BigIntImpl())
{
    pimpl->value = value;
}

BigInt::BigInt(double value)
    : pimpl(new BigIntImpl())
{
    pimpl->value = static_cast<long long>(value);
}

BigInt::BigInt(const BigInt &other)
    : pimpl(new BigIntImpl(*other.pimpl))
{
}

double BigInt::toDouble() const
{
    return pimpl->value.convert_to<double>();
}

float BigInt::toFloat() const
{
    return pimpl->value.convert_to<float>();
}

long long BigInt::toLongLong() const
{
    return pimpl->value.convert_to<long long>();
}

int BigInt::sigNum() const
{
    return pimpl->value.sign();
}

kSigType BigInt::sigType() const
{
    return rfSigUtils::sigType(pimpl->value.convert_to<long long>());
}

BigInt BigInt::abs() const
{
    BigInt result;
    result.pimpl->value = boost::multiprecision::abs(pimpl->value);
    return result;
}

bool BigInt::isZero() const
{
    return pimpl->value == 0;
}

BigInt &BigInt::operator=(const BigInt &other)
{
    if (this != &other)
    {
        delete pimpl;
        pimpl = new BigIntImpl(*other.pimpl);
    }
    return *this;
}

BigInt::~BigInt()
{
    delete pimpl;
}

BigInt BigInt::operator+(const BigInt &other) const
{
    BigInt result;
    result.pimpl->value = this->pimpl->value + other.pimpl->value;
    return result;
}

BigInt BigInt::operator-(const BigInt &other) const
{
    BigInt result;
    result.pimpl->value = this->pimpl->value - other.pimpl->value;
    return result;
}

BigInt BigInt::operator%(const BigInt &other) const
{
    return *this - (*this / other) * other;
}

BigInt BigInt::operator*(const BigInt &other) const
{
    BigInt result;
    result.pimpl->value = this->pimpl->value * other.pimpl->value;
    return result;
}

BigInt BigInt::operator/(const BigInt &other) const
{
    BigInt result;
    result.pimpl->value = this->pimpl->value / other.pimpl->value;
    return result;
}

bool BigInt::operator==(const BigInt &other) const
{
    return this->pimpl->value == other.pimpl->value;
}

bool BigInt::operator!=(const BigInt &other) const
{
    return this->pimpl->value != other.pimpl->value;
}
bool BigInt::operator<(const BigInt &other) const
{
    return this->pimpl->value < other.pimpl->value;
}
bool BigInt::operator<=(const BigInt &other) const
{
    return this->pimpl->value <= other.pimpl->value;
}
bool BigInt::operator>(const BigInt &other) const
{
    return this->pimpl->value > other.pimpl->value;
}
bool BigInt::operator>=(const BigInt &other) const
{
    return this->pimpl->value >= other.pimpl->value;
}

BigInt BigInt::add(const BigInt &other) const
{
    BigInt result;
    result.pimpl->value = this->pimpl->value + other.pimpl->value;
    return result;
}

BigInt BigInt::subtract(const BigInt &other) const
{
    BigInt result;
    result.pimpl->value = this->pimpl->value - other.pimpl->value;
    return result;
}

BigInt BigInt::multiply(const BigInt &other) const
{
    BigInt result;
    result.pimpl->value = this->pimpl->value * other.pimpl->value;
    return result;
}

BigInt BigInt::divide(const BigInt &other) const
{
    BigInt result;
    result.pimpl->value = this->pimpl->value / other.pimpl->value;
    return result;
}

BigInt BigInt::sqrt(const BigInt &value)
{
    BigInt result;
    result.pimpl->value = boost::multiprecision::sqrt(value.pimpl->value);
    return result;
}

BigInt BigInt::sqrt() const
{
    return sqrt(*this);
}

BigInt BigInt::gcd(const BigInt &other) const
{
    BigInt result;
    result.pimpl->value = boost::multiprecision::gcd(this->pimpl->value, other.pimpl->value);
    return result;
}

BigInt BigInt::gcd(const BigInt &a, const BigInt &b)
{
    return a.gcd(b);
}

BigInt BigInt::negate() const
{
    BigInt result;
    result.pimpl->value = -this->pimpl->value;
    return result;
}

BigInt BigInt::determinant(BigInt x1, BigInt y1, BigInt x2, BigInt y2)
{
    BigInt tmp1 = x1 * y2;
    BigInt tmp2 = x2 * y1;
    return tmp1 - tmp2;
}

std::vector<BigInt> BigInt::add_rational_coordinates(const std::vector<BigInt> &first,
                                                     const std::vector<BigInt> &second)
{
    std::vector<BigInt> result(3);
    if (first[2] == second[2])
    {
        // both rational numbers have the same denominator
        result[2] = first[2];
        result[0] = first[0] + second[0];
        result[1] = first[1] + second[1];
    }
    else
    {
        // multiply both denominators for the new denominator
        // to be on the save side:
        // taking the least common multiple would be optimal
        result[2] = first[2] * second[2];
        BigInt tmp1 = first[0] * second[2];
        BigInt tmp2 = second[0] * first[2];
        result[0] = tmp1 + tmp2;
        tmp1 = first[1] * second[2];
        tmp2 = second[1] * first[2];
        result[1] = tmp1 + tmp2;
    }
    return result;
}

} // namespace rfbase
