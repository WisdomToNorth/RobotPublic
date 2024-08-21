
#pragma once
#ifndef CONFIG_MAINWINDOW_H
#define CONFIG_MAINWINDOW_H

#include <vector>

#include <QAction>
#include <QMainWindow>

#include <AIS_ColoredShape.hxx>

class QProgressDialog;
class QProgressBar;

QT_BEGIN_NAMESPACE
namespace Ui
{
class REMainWindowClass;
}
QT_END_NAMESPACE

namespace rfgui
{
class RfViewer;
class AuxOptionWidget;

class REMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    REMainWindow(QWidget *parent = Q_NULLPTR);
    ~REMainWindow() override;
    REMainWindow(const REMainWindow &) = delete;
    REMainWindow &operator=(const REMainWindow &) = delete;
    REMainWindow(REMainWindow &&) = delete;
    REMainWindow &operator=(REMainWindow &&) = delete;

    void updateActionIcon();
    void initProgressBar();

public:
    void setStatusBarText(const std::string &text);
    void showProgressBar();
    void hideProgressBar();
    void updateProgressBarLoc();
    void updateAllWidgets();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void changeEvent(QEvent *event) override; // Minimized window
    void keyPressEvent(QKeyEvent *event) override;
    void moveEvent(QMoveEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    void initUIsize();

    void initCmdActions();

private:
    Ui::REMainWindowClass *ui;

    QProgressBar *progress_bar_ = nullptr;
    std::vector<QAction *> cmd_actions_;

private:
    // for test
    friend class TestMainWindow;
};
} // namespace rfgui
#endif
