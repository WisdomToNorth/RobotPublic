#pragma once
#ifndef APPFLAG_H
#define APPFLAG_H

#include <TopAbs_ShapeEnum.hxx>

namespace rfgui
{
using rfFlag = unsigned long long;

namespace selectflag
{
using Select = TopAbs_ShapeEnum;
constexpr Select kCompound = TopAbs_ShapeEnum::TopAbs_COMPOUND;
constexpr Select kCompSolid = TopAbs_ShapeEnum::TopAbs_COMPSOLID;
constexpr Select kSolid = TopAbs_ShapeEnum::TopAbs_SOLID;
constexpr Select kShell = TopAbs_ShapeEnum::TopAbs_SHELL;
constexpr Select kFace = TopAbs_ShapeEnum::TopAbs_FACE;
constexpr Select kWire = TopAbs_ShapeEnum::TopAbs_WIRE;
constexpr Select kEdge = TopAbs_ShapeEnum::TopAbs_EDGE;
constexpr Select kVertex = TopAbs_ShapeEnum::TopAbs_VERTEX;
constexpr Select kShape = TopAbs_ShapeEnum::TopAbs_SHAPE;
} // namespace selectflag
} // namespace rfgui
#endif
