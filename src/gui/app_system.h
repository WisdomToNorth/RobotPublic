#pragma once
#ifndef GAPP_H
#define GAPP_H

#include <functional>
#include <vector>

#include "rfapp/app_settings.h"
#include "rfapp/delegate.h"
#include "rfapp/observer.h"
#include "rfbase/rfdefine.h"
#include "rfdb/dbVariant.h"
#include "rfgraphics/rfviewernew.h"
#include "rfgraphics/rfviewevent.h"

#include "command/cmd_base.h"
#include "command/cmddefine.h"
#include "command/cmdmanager.h"
#include "selectmodehelper.h"
#include "ui/mainwindow.h"
#include "viewerwrapper.h"
#include "viewnode.h"

class QDockWidget;
class QMenu;

namespace rfgui
{
class AppModeDock;
class CmdDock;
class FileExpDock;
class FileTreeWidget;
class ViewerWidget;
class ViewRobot;

class GApp
{
public:
    enum class AppMode
    {
        Leisure,
        InCommand,
        InCalc,
        NoGui
    };

public:
    static GApp &instance()
    {
        // Lazy initialization
        static GApp *instance = new GApp();
        return *instance;
    }

    GApp(GApp &) = delete;

    bool loadSetting();
    void saveSetting() const;
    rfapp::RfSettings *getSetting() const;

    void setAppFont(const std::string &font_family, unsigned int font_size);
    void refreshUISetting();

    ViewerCore *createContextViewer(QWidget *parent);
    ViewerCore *getViewer(int index);
    ViewerCore *getCurrentViewer();
    ViewerCore *setCurrentViewer(int index);
    int curViewerIndex() const;
    void removeViewer(int index);
    size_t getViewerCount() const;
    size_t getViewerCreatedCount() const;
    bool viewerChangeable() const;

    void addDbVariant(const rfdb::dbVariant &db_variant, const UseType &type, int viewer_index);
    bool removeDbVariant(ViewNode *db_variant);

    void attachViewTreeObserver(rfapp::Observer<ViewNode> *obs);
    void updateViewTree(ViewNode *viewtreenode, rfapp::KMode mode);
    void getAllViewTrees(std::vector<ViewNode *> &view_trees);
    ViewNode *getViewTreeByViewObj(Handle(AIS_InteractiveObject) view_obj);

    void createGUI();
    REMainWindow *getMainWindow() const;
    ViewerWidget *getViewerWidget() const;
    void addToRMenu(const std::string &action_name);

    void processViewerEvent(const rfapp::ViewerEvent &view_event);

    [[nodiscard]] bool addToCmdList(ECmd cmdenum);
    [[nodiscard]] bool addToCmdList(ECmd cmdenum, const Parameter &parameter);
    [[nodiscard]] bool addToCmdList(std::unique_ptr<CmdBase> cmd);
    [[nodiscard]] bool executeLatestCmd();

    void addCmdDock(QWidget *widget, const std::string &name);
    void setCmdDone();
    void cleanUpCmdWidget();

    void showProgressBar() const;
    void hideProgressBar() const;
    void minimizeAllWidgets() const;
    void maximizeAllWidgets() const;
    void resizeAllWidgets() const;

    // first init, then set, then pop. Or you can always use set
    void setTempSelectMode(const rfgeo::ShapeTypeSet &select_types,
                           const rfgeo::ShapeTypeSet &enabled_types);
    void popTempSelectMode() const;
    void clearTempSelectMode();

    void setViewerSelectFlag(int flag);
    void setSelectEnabledFlag(int flag);

    typedef std::function<void(ViewNode *)> addEventFun;
    typedef std::function<void(ViewNode *)> removeEventFun;
    typedef std::function<void(ViewNode *)> changeEventFun;
    rfapp::Delegate<addEventFun> db_variant_add_delegate_;
    rfapp::Delegate<removeEventFun> db_variant_delete_delegate_;
    rfapp::Delegate<changeEventFun> db_variant_change_delegate_;

    typedef std::function<void(int)> viewerAddFun;
    rfapp::Delegate<viewerAddFun> viewer_add_delegate_;

    typedef std::function<void(int)> selectModeChangedFun;
    rfapp::Delegate<selectModeChangedFun> viewer_select_mode_delegate_;

    typedef std::function<void(int)> enableModeChangedFun;
    rfapp::Delegate<enableModeChangedFun> enable_mode_change_delegate_;

private:
    GApp();
    ~GApp();

    void createMainWindow();
    void createAppModeDock();
    void createLogDock();
    void createFileExploreDock();

    void setAppMode(AppMode mode);
    void onAppModeChanged(AppMode mode);
    AppMode getAppMode() const;

private:
    static size_t s_viewer_created_count_;

    std::vector<QWidget *> cad_views_;
    rfapp::RfSettings *setting_;

    std::vector<ViewNode *> view_trees_;
    std::vector<ViewRobot *> view_robots_; // TODO: refactory

    rfapp::Subject<ViewNode> subject_;
    int cur_viewer_index_ = -1;

    int viewer_select_flag_ = 0;
    int viewer_enabled_flag_ = 0;

    REMainWindow *main_window_ = nullptr;
    ViewerWidget *viewer_widget_ = nullptr;

    AppModeDock *app_mode_dock_ = nullptr;
    CmdDock *cmd_dock_ = nullptr;
    FileExpDock *file_explore_dock_ = nullptr;

    QMenu *right_button_menu_ = nullptr;
    SelectModeHelper *select_mode_helper_ = nullptr;

    AppMode app_mode_ = AppMode::Leisure;
};

} // namespace rfgui
#endif
