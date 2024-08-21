#include "select_mode_widget.h"

#include <unordered_set>

#include <QButtonGroup>
#include <QTreeWidget>

#include <BRepPrimAPI_MakeSphere.hxx>
#include <magic_enum/magic_enum.hpp>

#include "rfdb/dbutils/shapebuilder.h"
#include "rfgeo3d/vmselectmode.h"
#include "rflog/rfLog.h"

#include "app_system.h"
#include "ui_select_mode_widget.h"
#include "viewer_utils.h"
#include "viewerwrapper.h"

namespace rfgui
{
SelectModeWidget::SelectModeWidget(QWidget *parent)
    : QWidget(parent)
    , ui_(new Ui::SelectModeWidgetUI())
{
    ui_->setupUi(this);
}

SelectModeWidget::~SelectModeWidget()
{
}

void SelectModeWidget::on_cb_face_toggled(bool)
{
    updateViewerDynamicMode();
}
void SelectModeWidget::on_cb_edge_toggled(bool)
{
    updateViewerDynamicMode();
}
void SelectModeWidget::on_cb_vertax_toggled(bool)
{
    updateViewerDynamicMode();
}
void SelectModeWidget::on_cb_solid_toggled(bool)
{
    updateViewerDynamicMode();
}
void SelectModeWidget::on_cb_shape_toggled(bool)
{
    updateViewerDynamicMode();
}
void SelectModeWidget::on_cb_wire_toggled(bool)
{
    updateViewerDynamicMode();
}
void SelectModeWidget::on_cb_shell_toggled(bool)
{
    updateViewerDynamicMode();
}

void SelectModeWidget::on_pb_selectall_clicked()
{
    GApp::instance().setViewerSelectFlag(rfgeo::SelectUtils::getSelectAllFlag());
}

void SelectModeWidget::on_pb_unselectall_clicked()
{
    GApp::instance().setViewerSelectFlag(rfgeo::SelectUtils::getUnSelectAllFlag());
}

int SelectModeWidget::getCurSelectFlag()
{
    std::unordered_set<TopAbs_ShapeEnum> shape_types;

    if (ui_->cb_solid->isChecked())
        shape_types.insert(TopAbs_ShapeEnum::TopAbs_SOLID);
    if (ui_->cb_shell->isChecked())
        shape_types.insert(TopAbs_ShapeEnum::TopAbs_SHELL);
    if (ui_->cb_face->isChecked())
        shape_types.insert(TopAbs_ShapeEnum::TopAbs_FACE);
    if (ui_->cb_wire->isChecked())
        shape_types.insert(TopAbs_ShapeEnum::TopAbs_WIRE);
    if (ui_->cb_edge->isChecked())
        shape_types.insert(TopAbs_ShapeEnum::TopAbs_EDGE);
    if (ui_->cb_vertax->isChecked())
        shape_types.insert(TopAbs_ShapeEnum::TopAbs_VERTEX);
    if (ui_->cb_shape->isChecked())
        shape_types.insert(TopAbs_ShapeEnum::TopAbs_SHAPE);

    int flag = rfgeo::SelectUtils::getSelectFlag(shape_types);
    return flag;
}

void SelectModeWidget::setCurSelectFlag(int select_flag)
{
    if (this->getCurSelectFlag() == select_flag)
    {
        return;
    }

    std::unordered_set<TopAbs_ShapeEnum> shape_types;
    rfgeo::SelectUtils::getSelectTypes(select_flag, shape_types);
    bool select_solid = shape_types.contains(TopAbs_ShapeEnum::TopAbs_SOLID);
    bool select_shell = shape_types.contains(TopAbs_ShapeEnum::TopAbs_SHELL);
    bool select_face = shape_types.contains(TopAbs_ShapeEnum::TopAbs_FACE);
    bool select_wire = shape_types.contains(TopAbs_ShapeEnum::TopAbs_WIRE);
    bool select_edge = shape_types.contains(TopAbs_ShapeEnum::TopAbs_EDGE);
    bool select_vertax = shape_types.contains(TopAbs_ShapeEnum::TopAbs_VERTEX);
    bool select_shape = shape_types.contains(TopAbs_ShapeEnum::TopAbs_SHAPE);

    ui_->cb_solid->blockSignals(true);
    ui_->cb_shell->blockSignals(true);
    ui_->cb_face->blockSignals(true);
    ui_->cb_wire->blockSignals(true);
    ui_->cb_edge->blockSignals(true);
    ui_->cb_vertax->blockSignals(true);
    ui_->cb_shape->blockSignals(true);

    ui_->cb_solid->setChecked(select_solid);
    ui_->cb_shell->setChecked(select_shell);
    ui_->cb_face->setChecked(select_face);
    ui_->cb_wire->setChecked(select_wire);
    ui_->cb_edge->setChecked(select_edge);
    ui_->cb_vertax->setChecked(select_vertax);
    ui_->cb_shape->setChecked(select_shape);

    ui_->cb_solid->blockSignals(false);
    ui_->cb_shell->blockSignals(false);
    ui_->cb_face->blockSignals(false);
    ui_->cb_wire->blockSignals(false);
    ui_->cb_edge->blockSignals(false);
    ui_->cb_vertax->blockSignals(false);
    ui_->cb_shape->blockSignals(false);

    [[maybe_unused]] auto new_flag = getCurSelectFlag();
    assert(new_flag == select_flag);
}

void SelectModeWidget::setEnabledFlag(int select_flag)
{
    std::unordered_set<TopAbs_ShapeEnum> shape_types;
    rfgeo::SelectUtils::getSelectTypes(select_flag, shape_types);
    bool select_solid = shape_types.contains(TopAbs_ShapeEnum::TopAbs_SOLID);
    bool select_shell = shape_types.contains(TopAbs_ShapeEnum::TopAbs_SHELL);
    bool select_face = shape_types.contains(TopAbs_ShapeEnum::TopAbs_FACE);
    bool select_wire = shape_types.contains(TopAbs_ShapeEnum::TopAbs_WIRE);
    bool select_edge = shape_types.contains(TopAbs_ShapeEnum::TopAbs_EDGE);
    bool select_vertax = shape_types.contains(TopAbs_ShapeEnum::TopAbs_VERTEX);
    bool select_shape = shape_types.contains(TopAbs_ShapeEnum::TopAbs_SHAPE);

    ui_->cb_solid->setEnabled(select_solid);
    ui_->cb_shell->setEnabled(select_shell);
    ui_->cb_face->setEnabled(select_face);
    ui_->cb_wire->setEnabled(select_wire);
    ui_->cb_edge->setEnabled(select_edge);
    ui_->cb_vertax->setEnabled(select_vertax);
    ui_->cb_shape->setEnabled(select_shape);
}

void SelectModeWidget::updateViewerDynamicMode()
{
    if (about_to_update_)
    {
        assert(false);
        return;
    }
    int select_flag = getCurSelectFlag();
    GApp::instance().setViewerSelectFlag(select_flag);
}

} // namespace rfgui
