#pragma once
#ifndef SELECT_UTILS_H
#define SELECT_UTILS_H

#include <unordered_set>

#include <TopAbs_ShapeEnum.hxx>

namespace rfgeo
{
using ShapeTypeSet = std::unordered_set<TopAbs_ShapeEnum>;
using ShapeTypes = int;

class SelectUtils
{
public:
    static ShapeTypes getSelectFlag(const std::unordered_set<TopAbs_ShapeEnum> &shape_types);
    static void getSelectTypes(ShapeTypes flag, std::unordered_set<TopAbs_ShapeEnum> &shape_types);
    static ShapeTypes getSelectAllFlag();
    static ShapeTypes getUnSelectAllFlag();
};

} // namespace rfgeo
#endif
