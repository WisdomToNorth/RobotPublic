
#pragma once
#ifndef CONTEXTINFO_H
#define CONTEXTINFO_H

#include <SelectMgr_EntityOwner.hxx>

namespace rfapp
{
enum VType : unsigned int
{
    None = 1 << 0,
    MouseMove = 1 << 1,
    MouseWheel = 1 << 2,
    LButtonDown = 1 << 3,
    LButtonUp = 1 << 4,
    RButtonDown = 1 << 5,
    RButtonUp = 1 << 6,
    MButtonDown = 1 << 7,
    MButtonUp = 1 << 8,
    KeyDown = 1 << 9,
    KeyUp = 1 << 10,
    LRButtonDown = LButtonDown | RButtonDown
};

using VTypes = unsigned int;

struct ViewerEvent
{
    VTypes type_flags = VType::None;

    double view_pos_x;
    double view_pos_y;
    double view_pos_z;

    int mouse_pos_x;
    int mouse_pos_y;

    int wheel_delta_x;
    int wheel_delta_y;

    Handle(SelectMgr_EntityOwner) picked_entity = NULL;
};
} // namespace rfapp
#endif
