
#pragma once
#ifndef DB_ROBOTDEFINE_H
#define DB_ROBOTDEFINE_H

#include <gp_Pnt.hxx>

namespace rfdb
{
enum class Converntion
{
    Rzyx,
    Rxyz,
    Rzxz
};

struct Quaternion
{
    double x_ = 0.0;
    double y_ = 0.0;
    double z_ = 0.0;
    double w_ = 0.0;
};

struct Posture
{
    gp_Pnt location_;
    gp_Pnt euler_; // Degree system
    Posture()
        : Posture(gp_Pnt(), gp_Pnt())
    {
    }
    Posture(const gp_Pnt &loc, const gp_Pnt &euler)
        : location_(loc)
        , euler_(euler)
    {
    }
};
} // namespace rfdb
#endif
