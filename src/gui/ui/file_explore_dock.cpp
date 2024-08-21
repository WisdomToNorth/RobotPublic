#include "file_explore_dock.h"

#include <QVBoxLayout>

#include "generated/config.h"

#include "ui/form/filesystemwidget.h"

namespace rfgui
{
FileExpDock::FileExpDock(QWidget *parent, const std::string &dock_name)
{
    this->setParent(parent);
    this->setWindowTitle(dock_name.c_str());

    WidgetFileSystem *file_explore_widget_ = new WidgetFileSystem(this);
    file_explore_widget_->setLocation(APP_RESOURCE_PATH);
    this->setWidget(file_explore_widget_);
}

FileExpDock::~FileExpDock()
{
}

void FileExpDock::setDockName(const std::string &str)
{
    this->setWindowTitle(str.c_str());
}

void FileExpDock::hideTitleBar()
{
    this->setTitleBarWidget(new QWidget());
}
} // namespace rfgui
