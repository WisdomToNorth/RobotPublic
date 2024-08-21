#pragma once
#ifndef RF_BIGINT_H
#define RF_BIGINT_H

#include <vector>

#include "rfSigUtils.h"

namespace rfbase
{
class BigIntImpl; // forward declaration
class BigInt
{
private:
    BigIntImpl *pimpl = nullptr; // pointer to implementation

public:
    static BigInt ZERO;
    BigInt();
    explicit BigInt(int value);
    explicit BigInt(long long value);
    explicit BigInt(double value);
    BigInt(const BigInt &other);
    BigInt &operator=(const BigInt &other);
    BigInt &operator=(int value);
    BigInt &operator=(long long value);
    ~BigInt();

    BigInt operator+(const BigInt &other) const;
    BigInt operator-(const BigInt &other) const;
    BigInt operator*(const BigInt &other) const;
    BigInt operator/(const BigInt &other) const;
    BigInt operator%(const BigInt &other) const;
    BigInt add(const BigInt &other) const;
    BigInt subtract(const BigInt &other) const;
    BigInt multiply(const BigInt &other) const;
    BigInt divide(const BigInt &other) const;

    bool operator==(const BigInt &other) const;
    bool operator!=(const BigInt &other) const;
    bool operator<(const BigInt &other) const;
    bool operator<=(const BigInt &other) const;
    bool operator>(const BigInt &other) const;
    bool operator>=(const BigInt &other) const;

    double toDouble() const;
    float toFloat() const;
    long long toLongLong() const;

    int sigNum() const;
    kSigType sigType() const;

    bool isZero() const;
    BigInt abs() const;
    BigInt sqrt() const;
    static BigInt sqrt(const BigInt &value);
    BigInt gcd(const BigInt &other) const;
    static BigInt gcd(const BigInt &a, const BigInt &b);
    BigInt negate() const;

    static BigInt determinant(BigInt x1, BigInt y1, BigInt x2, BigInt y2);
    static std::vector<BigInt> add_rational_coordinates(const std::vector<BigInt> &first,
                                                        const std::vector<BigInt> &second);
};
} // namespace rfbase

#endif // RF_BIGINTAjson
