#pragma once
#ifndef SELECT_MODE_HELPER_H
#define SELECT_MODE_HELPER_H

#include <stack>

#include "rfgeo3d/vmselectmode.h"

namespace rfgui
{
class SelectModeHelper
{
public:
    SelectModeHelper(const rfgeo::ShapeTypeSet &current_select_set,
                     const rfgeo::ShapeTypeSet &current_enable_set);
    SelectModeHelper(const rfgeo::ShapeTypes &current_select_types,
                     const rfgeo::ShapeTypes &current_enable_types);

    ~SelectModeHelper();
    void pushSelectType(const rfgeo::ShapeTypeSet &select_types);
    void popSelectType();
    void pushEnableType(const rfgeo::ShapeTypeSet &select_types);
    void popEnableType();

    void popAllBothType();

private:
    rfgeo::ShapeTypes previous_selected_types_;
    rfgeo::ShapeTypes previous_enabled_types_;
    std::stack<rfgeo::ShapeTypes> selected_types_stk_;
    std::stack<rfgeo::ShapeTypes> types_stk_;
};
} // namespace rfgui
#endif // !_COMMAND_IMPORTFILE_H_
