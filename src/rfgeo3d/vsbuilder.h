#pragma once
#ifndef VS_BUILDER_H
#define VS_BUILDER_H

#include <AIS_ColoredShape.hxx>

/// @brief Create some viewshape for preview, with no dbVariant binded.
namespace rfgeo
{
class VMBuilder
{
public:
    static Handle(AIS_ColoredShape)
        createDumbBellViewShape(const gp_Pnt &p1, const gp_Pnt &p2, double l_w_scale = 100.0);
};
} // namespace rfgeo
#endif
