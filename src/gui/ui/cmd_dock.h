
#pragma once
#ifndef CMD_DOCK_H
#define CMD_DOCK_H

#include <QDockWidget>
#include <QString>
#include <QWidget>

namespace rfgui
{
class FileTreeWidget;

class CmdDock : public QDockWidget
{
    Q_OBJECT
public:
    CmdDock(QWidget *parent, QWidget *cmd_widget, const std::string &dock_name);
    ~CmdDock();
    CmdDock(const CmdDock &) = delete;
    CmdDock &operator=(const CmdDock &) = delete;
    CmdDock(CmdDock &&) = delete;
    CmdDock &operator=(CmdDock &&) = delete;

    void setDockName(const std::string &str);
    void hideTitleBar();

private:
    QWidget *widget_ = nullptr;
};
} // namespace rfgui
#endif
