#include "selectmodehelper.h"

#include "rfgeo3d/vmselectmode.h"

#include "app_system.h"

namespace rfgui
{
SelectModeHelper::SelectModeHelper(const rfgeo::ShapeTypeSet &current_select_set,
                                   const rfgeo::ShapeTypeSet &current_enable_set)
{
    previous_selected_types_ = rfgeo::SelectUtils::getSelectFlag(current_select_set);
    previous_enabled_types_ = rfgeo::SelectUtils::getSelectFlag(current_enable_set);
}
SelectModeHelper::SelectModeHelper(const rfgeo::ShapeTypes &current_select_types,
                                   const rfgeo::ShapeTypes &current_enable_types)
{
    previous_selected_types_ = current_select_types;
    previous_enabled_types_ = current_enable_types;
}

SelectModeHelper::~SelectModeHelper()
{
    GApp::instance().setSelectEnabledFlag(previous_enabled_types_);
    GApp::instance().setViewerSelectFlag(previous_selected_types_);
}

void SelectModeHelper::pushSelectType(const rfgeo::ShapeTypeSet &select_types)
{
    rfgeo::ShapeTypes selectflag = rfgeo::SelectUtils::getSelectFlag(select_types);
    selected_types_stk_.push(selectflag);
    GApp::instance().setViewerSelectFlag(selectflag);
}

void SelectModeHelper::popSelectType()
{
    if (!selected_types_stk_.empty())
    {
        selected_types_stk_.pop();
        auto next_select_types = selected_types_stk_.empty() ?
                                     rfgeo::SelectUtils::getUnSelectAllFlag() :
                                     selected_types_stk_.top();
        GApp::instance().setViewerSelectFlag(next_select_types);
    }
}

void SelectModeHelper::pushEnableType(const rfgeo::ShapeTypeSet &select_types)
{
    rfgeo::ShapeTypes selectflag = rfgeo::SelectUtils::getSelectFlag(select_types);
    types_stk_.push(selectflag);
    GApp::instance().setSelectEnabledFlag(selectflag);
}

void SelectModeHelper::popEnableType()
{
    if (!types_stk_.empty())
    {
        types_stk_.pop();
        auto next_enable_types =
            types_stk_.empty() ? rfgeo::SelectUtils::getUnSelectAllFlag() : types_stk_.top();
        GApp::instance().setSelectEnabledFlag(next_enable_types);
    }
}

void SelectModeHelper::popAllBothType()
{
    while (!types_stk_.empty())
    {
        types_stk_.pop();
    }
    while (!selected_types_stk_.empty())
    {
        selected_types_stk_.pop();
    }
    GApp::instance().setSelectEnabledFlag(previous_enabled_types_);
    GApp::instance().setViewerSelectFlag(previous_selected_types_);
}
} // namespace rfgui
