#include "cmd_dock.h"

#include <QScrollArea>
#include <QVBoxLayout>

namespace rfgui
{
CmdDock::CmdDock(QWidget *parent, QWidget *cmd_widget, const std::string &dock_name) :
    QDockWidget(parent)
{
    this->setWindowTitle(dock_name.c_str());

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(cmd_widget);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(scrollArea);

    QWidget *container = new QWidget;
    container->setLayout(layout);

    this->setWidget(container);
}

CmdDock::~CmdDock()
{
}

void CmdDock::setDockName(const std::string &str)
{
    this->setWindowTitle(str.c_str());
}

void CmdDock::hideTitleBar()
{
    this->setTitleBarWidget(new QWidget());
}
} // namespace rfgui
