
#include "vmselectmode.h"

#include "stdafx.h"

namespace rfgeo
{

ShapeTypes SelectUtils::getSelectFlag(const std::unordered_set<TopAbs_ShapeEnum> &shape_types)
{
    ShapeTypes select_flag = 0;
    for (const auto &type : shape_types)
    {
        if (type == TopAbs_ShapeEnum::TopAbs_COMPOUND || type == TopAbs_ShapeEnum::TopAbs_COMPSOLID)
        {
            continue;
        }
        select_flag |= (1 << AIS_Shape::SelectionMode(type));
    }
    return select_flag;
}

void SelectUtils::getSelectTypes(ShapeTypes flag, std::unordered_set<TopAbs_ShapeEnum> &shape_types)
{
    // Clip from AIS_Shape::SelectionMode
    // case TopAbs_VERTEX: return 1;
    // case TopAbs_EDGE: return 2;
    // case TopAbs_WIRE: return 3;
    // case TopAbs_FACE: return 4;
    // case TopAbs_SHELL: return 5;
    // case TopAbs_SOLID: return 6;
    // case TopAbs_COMPSOLID: return 7;
    // case TopAbs_COMPOUND: return 8;
    // case TopAbs_SHAPE: return 0;

    shape_types.clear();
    // NOTE: max 9 types
    for (int i = 0; i < 9; ++i)
    {
        if (flag & (1 << i))
        {
            TopAbs_ShapeEnum type = AIS_Shape::SelectionType(i);
            if (type != TopAbs_ShapeEnum::TopAbs_COMPOUND &&
                type != TopAbs_ShapeEnum::TopAbs_COMPSOLID)
            {
                shape_types.insert(type);
            }
        }
    }
}

ShapeTypes SelectUtils::getSelectAllFlag()
{
    // bit of int 0~6 is 1;
    // int num = 0;
    // for (int i = 0; i <= 6; i++)
    // {
    //     num |= 1 << i;
    // }
    // std::cout << "num of flag: " << num << "\n";
    return 127;
}

ShapeTypes SelectUtils::getUnSelectAllFlag()
{
    return 0;
}
} // namespace rfgeo
