
#pragma once
#ifndef FILE_EXPLORE_DOCK_H
#define FILE_EXPLORE_DOCK_H

#include <QDockWidget>
#include <QString>
#include <QWidget>

namespace rfgui
{
class WidgetFileSystem;

class FileExpDock : public QDockWidget
{
    Q_OBJECT
public:
    FileExpDock(QWidget *parent, const std::string &dock_name);
    ~FileExpDock();
    FileExpDock(const FileExpDock &) = delete;
    FileExpDock &operator=(const FileExpDock &) = delete;
    FileExpDock(FileExpDock &&) = delete;
    FileExpDock &operator=(FileExpDock &&) = delete;

    void setDockName(const std::string &str);
    void hideTitleBar();
};
} // namespace rfgui
#endif
