
#pragma once
#ifndef SELECT_MODE_WIDGET_H
#define SELECT_MODE_WIDGET_H

#include <unordered_map>

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
class SelectModeWidgetUI;
};
QT_END_NAMESPACE

namespace rfgui
{
class SelectModeWidget : public QWidget
{
    Q_OBJECT
public:
    SelectModeWidget(QWidget *parent = nullptr);
    ~SelectModeWidget();

    void updateViewerDynamicMode();

    int getCurSelectFlag();

    // Will ignore disable type
    void setCurSelectFlag(int select_flag);
    void setEnabledFlag(int enable_flag);

private slots:
    void on_cb_solid_toggled(bool);
    void on_cb_shell_toggled(bool);
    void on_cb_face_toggled(bool);
    void on_cb_wire_toggled(bool);
    void on_cb_edge_toggled(bool);
    void on_cb_vertax_toggled(bool);
    void on_cb_shape_toggled(bool);
    void on_pb_selectall_clicked();
    void on_pb_unselectall_clicked();

private:
    Ui::SelectModeWidgetUI *ui_;

    // If update is called from this widget, it will ignore the update signal from the model.
    // We may find a better way to handle this.
    bool about_to_update_ = false;

    class UpdateHelper
    {
        SelectModeWidget *parent_;

    public:
        UpdateHelper(SelectModeWidget *parent) : parent_(parent)
        {
            parent_->about_to_update_ = true;
        }
        ~UpdateHelper()
        {
            parent_->about_to_update_ = false;
        }
    };
};
} // namespace rfgui
#endif
