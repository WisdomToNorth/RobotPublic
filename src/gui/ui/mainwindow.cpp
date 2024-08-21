
#include "mainwindow.h"

#include <thread>

#include <QDockWidget>
#include <QIcon>
#include <QPainter>
#include <QProgressBar>
#include <QScreen>
#include <QTabWidget>

#include "rfbase/datautils.h"
#include "rfbase/rfTimer.h"
#include "rflog/rfLog.h"

#include "app_system.h"
#include "command/cmddefine.h"
#include "command/cmdicon.h"
#include "command/cmdregister.h"
#include "file_widget.h"
#include "ui_mainwindow.h"
#include "viewerwrapper.h"

namespace rfgui
{
REMainWindow::REMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::REMainWindowClass)
{
    ui->setupUi(this);

    initUIsize();
    initProgressBar();
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::North);
    initCmdActions();
}

REMainWindow::~REMainWindow()
{
    delete ui;
};

void REMainWindow::initUIsize()
{
    ui->mainToolBar->setMovable(false);
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);

    const QScreen *main_screen = QGuiApplication::primaryScreen();
    auto width = main_screen->geometry().width();
    auto height = main_screen->availableGeometry().height();
    const QRect screen_rect{width * 1 / 12, height * 1 / 20, width * 5 / 6, height * 5 / 6};
    this->setGeometry(screen_rect);
    this->updateGeometry();

    // Change the corner where the left and bottom dock areas meet
    this->setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
}

void REMainWindow::initProgressBar()
{
    if (progress_bar_ != nullptr)
    {
        return;
    }
    progress_bar_ = new QProgressBar(this);

    progress_bar_->setRange(0, 0);
    progress_bar_->setOrientation(Qt::Horizontal);
    progress_bar_->setAlignment(Qt::AlignVCenter);
    progress_bar_->setTextVisible(false);
    progress_bar_->setVisible(false);
    ui->statusBar->addPermanentWidget(progress_bar_);
    updateProgressBarLoc();
}

void REMainWindow::updateProgressBarLoc()
{
    if (progress_bar_ && progress_bar_->isVisible())
    {
        int lw = ui->statusBar->width() / 4;
        int lh = ui->statusBar->height() * 2 / 3;
        progress_bar_->setFixedSize(lw, lh);
    }
}

void REMainWindow::updateAllWidgets()
{
    auto widgets = QApplication::allWidgets();
    for (auto widget : widgets)
    {
        widget->update();
    }
}

void REMainWindow::setStatusBarText(const std::string &text)
{
    ui->statusBar->showMessage(text.c_str());
}

void REMainWindow::showProgressBar()
{
    rflog::debug("Begin caculate!");
    if (progress_bar_)
    {
        progress_bar_->setVisible(true);
    }
}

void REMainWindow::hideProgressBar()
{
    if (progress_bar_ && progress_bar_->isVisible())
    {
        progress_bar_->setVisible(false);
    }
}

void REMainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
}

void REMainWindow::initCmdActions()
{
    std::vector<std::pair<std::string, cmd::ECmd>> cmds;
    CmdRegister::getUserCmds(cmds);

    std::unordered_map<std::string, QMenu *> menu_map;
    for (const auto &[group, cmdenum] : cmds)
    {
        // Create action
        const std::string &cmd_name = cmd::CmdUtil::getCmdName(cmdenum);
        QAction *action = new QAction(this);
        action->setText(cmd_name.c_str());
        std::string icon_path = CmdIcon::getIconPath(cmdenum);
        // fmt::print("icon path: {}\n", icon_path);
        action->setIcon(QIcon(QString::fromStdString(icon_path)));
        action->setObjectName(cmd_name.c_str());
        action->setShortcut(
            QKeySequence::fromString(cmd::CmdUtil::getCmdShortCut(cmdenum).c_str()));
        connect(action, &QAction::triggered, this,
                [cmd_name]()
                {
                    if (GApp::instance().addToCmdList(cmd_name))
                    {
                        std::ignore = GApp::instance().executeLatestCmd();
                    }
                });
        cmd_actions_.push_back(action);

        // Add to menu and toolbar
        if (menu_map.find(group) == menu_map.end())
        {
            menu_map[group] = ui->menuBar->addMenu(group.c_str());
            ui->mainToolBar->addSeparator();
        }
        menu_map[group]->addAction(action);
        ui->mainToolBar->addAction(action);
    }
}

void REMainWindow::keyPressEvent(QKeyEvent *event)
{
    QMainWindow::keyPressEvent(event);
    if (event->key() == Qt::Key_Escape)
    {
        GApp::instance().setCmdDone();
    }
}

void REMainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    GApp::instance().resizeAllWidgets();
}

void REMainWindow::moveEvent(QMoveEvent *event)
{
    QMainWindow::moveEvent(event);
}

void REMainWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
}

void REMainWindow::changeEvent(QEvent *event)
{
    QMainWindow::changeEvent(event);
    if (this->windowState() == Qt::WindowState::WindowMinimized)
    {
        GApp::instance().minimizeAllWidgets();
    }
    else if (this->windowState() == Qt::WindowState::WindowMaximized)
    {
        GApp::instance().maximizeAllWidgets();
    }
}

void REMainWindow::updateActionIcon()
{
    for (auto &action : cmd_actions_)
    {
        std::string icon_path = CmdIcon::getIconPath(action->objectName().toStdString());
        action->setIcon(QIcon(QString::fromStdString(icon_path)));
    }
}
} // namespace rfgui
