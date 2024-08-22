#include "app_system.h"

#include <QApplication>
#include <QMenu>
#include <QPainter>
#include <QTimer>

#include "generated/config.h"
#include "rfbase/datautils.h"
#include "rfgeo3d/vmselectmode.h"
#include "rfgeo3d/vmutils.h"
#include "rflog/rfLog.h"
#include "rflog/rfLogListWidget.h"

#include "command/cmd_base.h"
#include "command/cmdregister.h"
#include "ui/app_mode_dock.h"
#include "ui/cmd_dock.h"
#include "ui/file_explore_dock.h"
#include "ui/file_widget.h"
#include "ui/mainwindow.h"
#include "ui/viewerwidget.h"
#include "viewer_observer.h"
#include "viewnode_utils.h"
#include "viewrobot.h"

namespace rfgui
{
size_t GApp::s_viewer_created_count_ = 0;
GApp::GApp()
    : setting_(nullptr)
{
}

GApp::~GApp()
{
    for (auto &vt : view_trees_)
    {
        delete vt;
        vt = nullptr;
    }
    view_trees_.clear();

    if (setting_)
    {
        rfapp::RfSettings::deleteSetting(setting_);
    }

    if (main_window_)
    {
        delete main_window_;
        main_window_ = nullptr;
    }
}

bool GApp::loadSetting()
{
    setting_ = rfapp::RfSettings::createNewSetting();
    setting_->setAllUIRefreshEvent([this]() { this->refreshUISetting(); });
    setting_->setFontChangeEvent([this](const std::string &font_family, unsigned int font_size)
                                 { this->setAppFont(font_family, font_size); });

    bool setting_load =
        rfapp::RfSettings::loadSetting(*setting_, APP_CONFIG_PATH + "/rfsettings.json");

    if (!setting_load)
    {
        rflog::debug("Failed to load setting, create a new one");
        if (!setting_->save())
        {
            rflog::error("Failed to save setting");
        }
        setting_load = true;
    }

    assert(rfapp::RfSettings::isSettingValid(*setting_));
    setting_->apply();
    return setting_load;
}

void GApp::saveSetting() const
{
    if (!setting_->save())
    {
        rflog::error("Failed to save setting");
    }
}

rfapp::RfSettings *GApp::getSetting() const
{
    return setting_;
};

ViewerCore *GApp::createContextViewer(QWidget *parent)
{
    ViewerCore *viewer = new ViewerCore(parent);
    viewer->setName("Viewer " + std::to_string(++GApp::s_viewer_created_count_));
    viewer->setSelectFlag(viewer_select_flag_);

    viewer->viewerEventCb = [this](auto &&PH1)
    { return this->processViewerEvent(std::forward<decltype(PH1)>(PH1)); };

    viewer_select_mode_delegate_.attach(
        [viewer](int flag)
        {
            viewer->clearSelected();
            viewer->setSelectFlag(flag);
            viewer->updateViewer(ViewerCore::KUpdate::Update);
        });

    cad_views_.push_back(viewer);
    viewer_add_delegate_.notify(static_cast<int>(cad_views_.size() - 1));
    return viewer;
}

ViewerCore *GApp::setCurrentViewer(int index)
{
    cur_viewer_index_ = index;
    ViewerCore *viewer = getViewer(cur_viewer_index_);
    if (viewer)
    {
        viewer->show();
        viewer->clearSelected();
        viewer->setSelectFlag(viewer_select_flag_);
        viewer->updateViewer(ViewerCore::KUpdate::Refresh);
    }
    return viewer;
}

int GApp::curViewerIndex() const
{
    return cur_viewer_index_;
}

size_t GApp::getViewerCreatedCount() const
{
    return GApp::s_viewer_created_count_;
}

bool GApp::viewerChangeable() const
{
    return app_mode_ == AppMode::Leisure;
}

size_t GApp::getViewerCount() const
{
    return cad_views_.size();
}

void GApp::removeViewer(int index)

{
    for (int i = static_cast<int>(view_trees_.size() - 1); i >= 0; i--)
    {
        if (view_trees_[i]->index_in_viewer_ == index)
        {
            removeDbVariant(view_trees_[i]);
        }
    }

    if (index >= 0 && index < (int)cad_views_.size())
    {
        cad_views_.erase(cad_views_.begin() + index);
    }
    cur_viewer_index_ = -1;
}

ViewerCore *GApp::getCurrentViewer()
{
    return getViewer(cur_viewer_index_);
}

ViewerCore *GApp::getViewer(int index)
{
    if (index >= 0 && index < (int)cad_views_.size())
    {
        // return cad_views_[index];
        ViewerCore *viewer = static_cast<ViewerCore *>(cad_views_[index]);
        if (viewer)
        {
            return viewer;
        }
    }
    return nullptr;
}

void GApp::addDbVariant(const rfdb::dbVariant &db_variant, const UseType &type, int viewer_index)
{
    auto view_tree = ViewNodeUtils::buildTree(db_variant);
    ViewNodeUtils::setViewerIndex(view_tree, viewer_index);
    ViewNodeUtils::setUseType(view_tree, type);

    view_trees_.push_back(view_tree);
    db_variant_add_delegate_.notify(view_tree);
    subject_.notifyAll(view_tree, rfapp::KMode::add);

    if (db_variant.getType() == rfdb::dbVariant::Type::Robot)
    {
        ViewRobot *view_robot = new ViewRobot(db_variant.toRobot(), view_tree);
        view_robots_.push_back(view_robot);
    }
}

bool GApp::removeDbVariant(ViewNode *db_variant)
{
    for (auto it = view_trees_.begin(); it != view_trees_.end(); it++)
    {
        if (*it == db_variant)
        {
            auto view_tree = *it;
            db_variant_delete_delegate_.notify(view_tree);
            subject_.notifyAll(view_tree, rfapp::KMode::remove);

            view_trees_.erase(it);
            rflog::debug("Size of view_trees_ is {}", view_trees_.size());
            delete view_tree;
            view_tree = nullptr;
            return true;
        }
    }
    return false;
}

void GApp::attachViewTreeObserver(rfapp::Observer<ViewNode> *obs)
{
    subject_.attach(obs);
}

void GApp::updateViewTree(ViewNode *viewtreenode, rfapp::KMode mode)
{
    subject_.notifyAll(viewtreenode, mode);
}

void GApp::getAllViewTrees(std::vector<ViewNode *> &view_trees)
{
    view_trees = view_trees_;
}

ViewNode *GApp::getViewTreeByViewObj(Handle(AIS_InteractiveObject) view_obj)
{
    for (auto &vt : view_trees_)
    {
        if (vt->viewmodel_item_ == view_obj)
        {
            return vt;
        }
    }
    return nullptr;
}

void GApp::createGUI()
{
    createMainWindow();
    createAppModeDock();
    createLogDock();
    createFileExploreDock();
    main_window_->tabifyDockWidget(app_mode_dock_, file_explore_dock_);

    GApp::instance().attachViewTreeObserver(new ViewObserver());
    setAppMode(AppMode::Leisure);
    setSelectEnabledFlag(rfgeo::SelectUtils::getSelectAllFlag());

    main_window_->initProgressBar();
    main_window_->show();
}

void GApp::createMainWindow()
{
    main_window_ = new REMainWindow();
    main_window_->setWindowTitle("RFEditor");
    viewer_widget_ = new ViewerWidget(main_window_);
    main_window_->setCentralWidget(viewer_widget_);
}

void GApp::createAppModeDock()
{
    if (!main_window_)
    {
        return;
    }
    app_mode_dock_ = new AppModeDock(nullptr);
    // app_mode_dock_->setTitleBarWidget(new QWidget());

    main_window_->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, app_mode_dock_);
}

void GApp::createLogDock()
{
    if (!main_window_)
    {
        return;
    }
    QDockWidget *log_dock = new QDockWidget();
    log_dock->setWindowTitle("Log");
    main_window_->addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, log_dock);

    rflog::FRLog &log = rflog::FRLog::instance();
    log.registerLogger(APP_LOG_PATH, "RFEditor");
    QListWidget *console_widget = log.registerLogListWidget();
    log_dock->setWidget(console_widget);

    rflog::debug("Build at {}", BUILD_TIME);
    rflog::debug("Hello! Welcome to RobotFramework V{}!", PROGRAM_VERSION);
}

void GApp::createFileExploreDock()
{
    if (!main_window_)
    {
        return;
    }

    file_explore_dock_ = new FileExpDock(main_window_, "File Explore");
    main_window_->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, file_explore_dock_);
}

void GApp::addCmdDock(QWidget *widget, const std::string &name)
{
    if (!main_window_ || (app_mode_ != AppMode::InCommand))
    {
        rflog::debug("Cannot add command widget in current mode, check you cmd type!");
        return;
    }

    cmd_dock_ = new CmdDock(nullptr, widget, name);
    main_window_->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, cmd_dock_);
    main_window_->tabifyDockWidget(app_mode_dock_, cmd_dock_);
    // cmd_dock_->setTitleBarWidget(new QWidget());

    // Use a single-shot timer to delay the call to raise()
    QTimer::singleShot(0, cmd_dock_, &QDockWidget::raise);
}

REMainWindow *GApp::getMainWindow() const
{
    return main_window_;
}

ViewerWidget *GApp::getViewerWidget() const
{
    return viewer_widget_;
}

void GApp::addToRMenu(const std::string &action_name)
{
    if (!right_button_menu_)
    {
        return;
    }
    right_button_menu_->addAction(action_name.c_str(), [action_name]
                                  { CmdMngr::getInstance().processRMenuAction(action_name); });
}

void GApp::processViewerEvent(const rfapp::ViewerEvent &view_event)
{
    viewer_widget_->updateMouseLocLabel(view_event.view_pos_x, view_event.view_pos_y,
                                        view_event.view_pos_z);
    if (view_event.type_flags == rfapp::VType::RButtonDown)
    {
        ViewerCore *viewer = getCurrentViewer();
        right_button_menu_ = new QMenu(viewer);
        right_button_menu_->addAction("Done", []() { GApp::instance().setCmdDone(); });
        right_button_menu_->addAction("Cancel", []() { GApp::instance().setCmdDone(); });
        if (app_mode_ == AppMode::InCommand)
        {
            CmdMngr::getInstance().getCmdRMenu();
        }
        const QPoint &globalPos =
            viewer->mapToGlobal(QPoint(view_event.mouse_pos_x, view_event.mouse_pos_y));
        right_button_menu_->exec(globalPos);
    }
    else if (view_event.type_flags == rfapp::VType::LButtonDown)
    {
        if (app_mode_ == AppMode::InCommand) // TODO: not good here, refactory
        {
            CmdMngr::getInstance().process(view_event);
        }
    }
    else if (view_event.type_flags == rfapp::VType::LRButtonDown)
    {
        std::ignore = addToCmdList(ECmd::RotViewer);
        CmdMngr::getInstance().process(view_event);
    }
    else if (view_event.type_flags == rfapp::VType::MouseMove)
    {
        CmdMngr::getInstance().process(view_event);
    }
    else if (view_event.type_flags == rfapp::VType::MouseWheel)
    {
        std::ignore = addToCmdList(ECmd::ZoomViewer);
        CmdMngr::getInstance().process(view_event);
    }
    else if (view_event.type_flags == rfapp::VType::MButtonDown)
    {
        std::ignore = addToCmdList(ECmd::PanViewer);
        CmdMngr::getInstance().process(view_event);
    }
    else if (view_event.type_flags == rfapp::VType::MButtonUp)
    {
        CmdMngr::getInstance().process(view_event);
    }
    else if (view_event.type_flags == rfapp::VType::LButtonUp ||
             view_event.type_flags == rfapp::VType::RButtonUp)
    {
        CmdMngr::getInstance().process(view_event);
    }
    else
    {
        rflog::debug("Unknown event type: {}", view_event.type_flags);
    }
}

void GApp::refreshUISetting()
{
    if (!setting_)
    {
        return;
    }
    const std::string &theme_name = setting_->getThemeFileName();
    const QString stylesheet = QString::fromStdString(
        rfbase::DataUtils::loadFile(APP_THEME_PATH + setting_->getThemeFileName()));
    rfapp::ThemeType type = rfapp::ThemeUtils::getThemeType(theme_name);

    main_window_->setStyleSheet(stylesheet);
    main_window_->updateActionIcon();
    app_mode_dock_->setStyleSheet(stylesheet);
    viewer_widget_->setThemeType(type);
    resizeAllWidgets();
}

void GApp::setAppFont(const std::string &font_family, unsigned int font_size)
{
    QApplication::setFont(QFont(font_family.c_str(), font_size));
}

bool GApp::addToCmdList(ECmd cmdenum)
{
    return addToCmdList(CmdRegister::createCommand(cmdenum));
}

bool GApp::addToCmdList(ECmd cmdenum, const Parameter &parameter)
{
    std::unique_ptr<CmdBase> cmd_ptr(CmdRegister::createCommand(cmdenum));
    if (!cmd_ptr)
    {
        return false;
    }
    cmd_ptr->setParameter(parameter);
    return addToCmdList(std::move(cmd_ptr));
}

bool GApp::addToCmdList(std::unique_ptr<CmdBase> cmd)
{
    if (!cmd)
    {
        return false;
    }

    if (app_mode_ == AppMode::NoGui)
    {
        if (cmd->getType() == CmdType::Interactive)
        {
            return false;
        }
        else
        {
            CmdMngr::getInstance().addToCmdList(std::move(cmd));
            return true;
        }
    }

    switch (app_mode_)
    {
    case AppMode::Leisure:
    {
        switch (cmd->getType())
        {
        case CmdType::TimeSession:
        {
            setAppMode(AppMode::InCalc);
            CmdMngr::getInstance().addToCmdList(std::move(cmd));
            return true;
        }
        case CmdType::Interactive:
        {
            setAppMode(AppMode::InCommand);
            CmdMngr::getInstance().addToCmdList(std::move(cmd));
            return true;
        }
        case CmdType::Immediate:
        case CmdType::Nestable:
        case CmdType::Appmode:
        {
            CmdMngr::getInstance().addToCmdList(std::move(cmd));
            return true;
        }
        case CmdType::Unknown:
        {
            return false;
        }
        }
        return false;
    }
    case AppMode::InCommand:
    case AppMode::InCalc:
    {
        if (cmd->getType() == CmdType::Nestable)
        {
            CmdMngr::getInstance().addToCmdList(std::move(cmd));
            return true;
        }
        else
        {
            return false;
        }
    }
    case AppMode::NoGui:
    {
        return false;
    }
    }
    return false;
}

bool GApp::executeLatestCmd()
{
    return CmdMngr::getInstance().executeLastCmd();
}

void GApp::setCmdDone()
{
    CmdMngr::getInstance().doneCmd();

    if (CmdMngr::getInstance().isCmdListEmpty())
    {
        clearTempSelectMode();
        setAppMode(AppMode::Leisure);
        cleanUpCmdWidget();
    }
}

void GApp::cleanUpCmdWidget()
{
    if (cmd_dock_)
    {
        delete cmd_dock_;
        cmd_dock_ = nullptr;
    }
}

void GApp::showProgressBar() const
{
    main_window_->showProgressBar();
}

void GApp::hideProgressBar() const
{
    main_window_->hideProgressBar();
}

void GApp::minimizeAllWidgets() const
{
    if (main_window_)
    {
        main_window_->showMinimized();
    }
}

void GApp::maximizeAllWidgets() const
{
    if (main_window_)
    {
        main_window_->showMaximized();
    }
}

void GApp::resizeAllWidgets() const
{
    if (!main_window_)
    {
        return;
    }
    main_window_->updateGeometry();
    main_window_->updateProgressBarLoc();

    if (app_mode_dock_->isVisible())
    {
        main_window_->resizeDocks({app_mode_dock_}, {main_window_->width() / 4},
                                  Qt::Orientation::Horizontal);
        app_mode_dock_->userResize();
        QResizeEvent event_3(app_mode_dock_->size(), app_mode_dock_->size());
        QApplication::sendEvent(app_mode_dock_, &event_3);
    }
    if (cmd_dock_ && cmd_dock_->isVisible())
    {
        QResizeEvent event_4(cmd_dock_->size(), cmd_dock_->size());
        QApplication::sendEvent(cmd_dock_, &event_4);
    }
}

void GApp::setAppMode(AppMode mode)
{
    app_mode_ = mode;
    onAppModeChanged(mode);
}

void GApp::onAppModeChanged(AppMode mode)
{
    if (main_window_ == nullptr)
    {
        return;
    }
    switch (mode)
    {
    case AppMode::Leisure:
    {
        hideProgressBar();
        main_window_->setStatusBarText("Leisure");
        break;
    }
    case AppMode::InCommand:
    {
        main_window_->setStatusBarText(CmdMngr::getInstance().getCurCmdName());
        break;
    }
    case AppMode::InCalc:
    {
        main_window_->setStatusBarText("Caculating...");
        showProgressBar();
        break;
    }
    case AppMode::NoGui:
    {
        main_window_->setStatusBarText("ERROR");
        break;
    }
    }
}

GApp::AppMode GApp::getAppMode() const
{
    return app_mode_;
}

void GApp::setTempSelectMode(const rfgeo::ShapeTypeSet &select_types,
                             const rfgeo::ShapeTypeSet &enabled_types)
{
    if (select_mode_helper_ == nullptr)
    {
        select_mode_helper_ =
            new SelectModeHelper(viewer_select_flag_, rfgeo::SelectUtils::getSelectAllFlag());
    }

    // NOTE: set enable type first, then set select type
    select_mode_helper_->pushEnableType(enabled_types);
    select_mode_helper_->pushSelectType(select_types);
}

void GApp::popTempSelectMode() const
{
    if (select_mode_helper_ != nullptr)
    {
        select_mode_helper_->popEnableType();
        select_mode_helper_->popSelectType();
    }
}

void GApp::clearTempSelectMode()
{
    if (select_mode_helper_ != nullptr)
    {
        select_mode_helper_->popAllBothType();
        delete select_mode_helper_;
        select_mode_helper_ = nullptr;
    }
}

void GApp::setViewerSelectFlag(int flag)
{
    if (viewer_select_flag_ == flag)
    {
        return;
    }
    // filter select type by enable type
    viewer_select_flag_ = flag & viewer_enabled_flag_;
    viewer_select_mode_delegate_.notify(viewer_select_flag_);
}

void GApp::setSelectEnabledFlag(int flag)
{
    viewer_enabled_flag_ = flag;
    enable_mode_change_delegate_.notify(viewer_enabled_flag_);
}
} // namespace rfgui
