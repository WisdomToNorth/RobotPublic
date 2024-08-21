#include "app_mode_dock.h"

#include "app_system.h"
#include "file_widget.h"
#include "gui_positions.h"
#include "select_mode_widget.h"
#include "ui_app_mode_dock.h"

namespace rfgui
{
AppModeDock::AppModeDock(QWidget *parent)
    : QDockWidget(parent)
    , ui_(new Ui::AppModeWidget)
{
    this->setWindowTitle("App Mode");

    tab_widget_ = new QWidget(this);
    ui_->setupUi(tab_widget_);

    this->setWidget(tab_widget_);

    db_mgr_widget_ = new FileTreeWidget();
    select_mode_widget_ = new SelectModeWidget();
    positions_widget_ = new PositionsWidget();

    ui_->tab_widget_->addTab(db_mgr_widget_, "File Manager");
    ui_->tab_widget_->addTab(select_mode_widget_, "Select Mode");
    ui_->tab_widget_->addTab(positions_widget_, "Positions");

    ui_->tab_widget_->setCurrentIndex(0);
    ui_->tab_widget_->tabBar()->setElideMode(Qt::ElideNone);
    ui_->tab_widget_->tabBar()->setExpanding(true);
    ui_->tab_widget_->setTabPosition(QTabWidget::TabPosition::West);

    // Bind delegate
    GApp::instance().attachViewTreeObserver(db_mgr_widget_);
    GApp::instance().viewer_add_delegate_.attach([this](int index)
                                                 { db_mgr_widget_->createAuxObject(index); });
    GApp::instance().viewer_select_mode_delegate_.attach(
        [this](int flag) { select_mode_widget_->setCurSelectFlag(flag); });
    GApp::instance().enable_mode_change_delegate_.attach(
        [this](int flag) { select_mode_widget_->setEnabledFlag(flag); });
}

AppModeDock::~AppModeDock()
{
}

void AppModeDock::userSetStyleSheet(const QString &stylesheet)
{
    this->setStyleSheet(stylesheet);
    if (db_mgr_widget_->isVisible())
    {
        QResizeEvent event_2(db_mgr_widget_->size(), db_mgr_widget_->size());
        QApplication::sendEvent(db_mgr_widget_, &event_2);
    }
}

void AppModeDock::userResize()
{
    db_mgr_widget_->setMinimumWidth(GApp::instance().getMainWindow()->width() / 5);
    select_mode_widget_->setMinimumWidth(GApp::instance().getMainWindow()->width() / 5);
    positions_widget_->setMinimumWidth(GApp::instance().getMainWindow()->width() / 5);
}
} // namespace rfgui
