
#pragma once
#ifndef VIEWER_WIDGET_H
#define VIEWER_WIDGET_H

#include <memory>
#include <vector>

#include <QWidget>

#include "rfapp/apptheme.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class ViewerWidgetUI;
}
QT_END_NAMESPACE

namespace rfgui
{
class ViewerWidget : public QWidget
{
    Q_OBJECT
public:
    ViewerWidget(QWidget *parent = Q_NULLPTR);
    ViewerWidget(const ViewerWidget &) = delete;
    ViewerWidget &operator=(const ViewerWidget &) = delete;
    ViewerWidget(ViewerWidget &&) = delete;
    ViewerWidget &operator=(ViewerWidget &&) = delete;

    void addNewContext();
    void removeContext();
    void addNewView();
    void removeView();
    bool setCurrentContext(int index);
    bool addChangeContextCommand(int index);
    void setThemeType(rfapp::ThemeType type);

    void hideMouseLocLabel();
    void showMouseLocLabel();
    void updateMouseLocLabel(double x, double y, double z);

private slots:
    void on_combo_view_currentIndexChanged(int index);
    void on_btn_PreviousView_clicked();
    void on_btn_NextView_clicked();
    void on_btn_CloseView_clicked();

private:
    Ui::ViewerWidgetUI *ui_ = nullptr;
};
} // namespace rfgui
#endif
