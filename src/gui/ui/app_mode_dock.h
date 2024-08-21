
#pragma once
#ifndef APP_MODE_DOCK_H
#define APP_MODE_DOCK_H

#include <QDockWidget>
#include <QString>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
class AppModeWidget;
}
QT_END_NAMESPACE

namespace rfgui
{
class FileTreeWidget;
class SelectModeWidget;
class PositionsWidget;

class AppModeDock : public QDockWidget
{
    Q_OBJECT
public:
    AppModeDock(QWidget *parent);
    ~AppModeDock();
    AppModeDock(const AppModeDock &) = delete;
    AppModeDock &operator=(const AppModeDock &) = delete;
    AppModeDock(AppModeDock &&) = delete;
    AppModeDock &operator=(AppModeDock &&) = delete;

    void userSetStyleSheet(const QString &stylesheet);
    void userResize();

private:
    Ui::AppModeWidget *ui_ = nullptr;
    QWidget *tab_widget_ = nullptr;

    FileTreeWidget *db_mgr_widget_ = nullptr;
    SelectModeWidget *select_mode_widget_ = nullptr;
    PositionsWidget *positions_widget_ = nullptr;
};
} // namespace rfgui
#endif
