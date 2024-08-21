#include "viewerwidget.h"

#include <cassert>

#include "app_system.h"
#include "ui_viewerwidget.h"

namespace rfgui
{
ViewerWidget::ViewerWidget(QWidget *parent)
    : QWidget(parent)
{
    ui_ = new Ui::ViewerWidgetUI();
    ui_->setupUi(this);
    hideMouseLocLabel();
}

void ViewerWidget::addNewContext()
{
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget); // Create a layout for the widget
    ViewerCore *viewer = GApp::instance().createContextViewer(widget);
    layout->addWidget(viewer);
    ui_->view_stack->addWidget(widget);

    ui_->combo_view->blockSignals(true);
    ui_->combo_view->addItem(viewer->getName().c_str());
    ui_->combo_view->blockSignals(false);

    setCurrentContext(ui_->view_stack->count() - 1);
}

bool ViewerWidget::setCurrentContext(int index)
{
    [[maybe_unused]] int vsc = ui_->view_stack->count();
    [[maybe_unused]] int gvc = static_cast<int>(GApp::instance().getViewerCount());
    assert(vsc == gvc);

    // NOTE: We should first set the current index of the view stack, then set the current index of
    // the combo box, because the refresh of viewer only happens when it is visible.
    ui_->view_stack->setCurrentIndex(index);
    std::ignore = GApp::instance().setCurrentViewer(index);
    ui_->combo_view->blockSignals(true);
    ui_->combo_view->setCurrentIndex(index);
    ui_->combo_view->blockSignals(false);

    showMouseLocLabel();
    return true;
}

void ViewerWidget::on_combo_view_currentIndexChanged(int index)
{
    if (!addChangeContextCommand(index))
    {
        ui_->combo_view->setCurrentIndex(GApp::instance().curViewerIndex());
    }
}

void ViewerWidget::removeContext()
{
    [[maybe_unused]] int vsc = ui_->view_stack->count();
    [[maybe_unused]] int gvc = static_cast<int>(GApp::instance().getViewerCount());
    assert(vsc == gvc);

    int index = ui_->view_stack->currentIndex();
    GApp::instance().removeViewer(index);
    ui_->combo_view->blockSignals(true);
    ui_->combo_view->removeItem(index);
    ui_->combo_view->blockSignals(false);
    QWidget *widget = ui_->view_stack->widget(index);
    ui_->view_stack->removeWidget(widget);
    delete widget;

    int next_index = std::min(index, ui_->view_stack->count() - 1);
    setCurrentContext(next_index);
}

void ViewerWidget::addNewView()
{
}

void ViewerWidget::removeView()
{
}

void ViewerWidget::hideMouseLocLabel()
{
    ui_->label_ValuePosX->setVisible(false);
    ui_->label_ValuePosY->setVisible(false);
    ui_->label_ValuePosZ->setVisible(false);
    ui_->label_PosX->setVisible(false);
    ui_->label_PosY->setVisible(false);
    ui_->label_PosZ->setVisible(false);
}

void ViewerWidget::showMouseLocLabel()
{
    ui_->label_ValuePosX->setVisible(true);
    ui_->label_ValuePosY->setVisible(true);
    ui_->label_ValuePosZ->setVisible(true);
    ui_->label_PosX->setVisible(true);
    ui_->label_PosY->setVisible(true);
    ui_->label_PosZ->setVisible(true);
}

void ViewerWidget::updateMouseLocLabel(double x, double y, double z)
{
    ui_->label_ValuePosX->setText(QString::number(x, 'f', 2));
    ui_->label_ValuePosY->setText(QString::number(y, 'f', 2));
    ui_->label_ValuePosZ->setText(QString::number(z, 'f', 2));
}

bool ViewerWidget::addChangeContextCommand(int index)
{
    if (GApp::instance().addToCmdList(cmd::kChangeContext, {.int_value = index}))
    {
        return GApp::instance().executeLatestCmd();
    }
    return false;
}

void ViewerWidget::on_btn_PreviousView_clicked()
{
    int index = std::max(0, ui_->view_stack->currentIndex() - 1);
    if (index == ui_->view_stack->currentIndex())
    {
        return;
    }
    addChangeContextCommand(index);
}

void ViewerWidget::on_btn_NextView_clicked()
{
    int index = std::min(ui_->view_stack->count() - 1, ui_->view_stack->currentIndex() + 1);
    if (index == ui_->view_stack->currentIndex())
    {
        return;
    }
    addChangeContextCommand(index);
}

void ViewerWidget::on_btn_CloseView_clicked()
{
    if (GApp::instance().addToCmdList(cmd::kRemoveContext))
    {
        std::ignore = GApp::instance().executeLatestCmd();
    }
}

void ViewerWidget::setThemeType(rfapp::ThemeType type)
{
    switch (type)
    {
    case rfapp::ThemeType::kLight:
    {
        ui_->btn_PreviousView->setIcon(QIcon(":/REMainWindow/icons/light/back.svg"));
        ui_->btn_NextView->setIcon(QIcon(":/REMainWindow/icons/light/next.svg"));
        ui_->btn_CloseView->setIcon(QIcon(":/REMainWindow/icons/light/cross.svg"));
        break;
    }
    case rfapp::ThemeType::kDark:
    {
        ui_->btn_PreviousView->setIcon(QIcon(":/REMainWindow/icons/dark/back.svg"));
        ui_->btn_NextView->setIcon(QIcon(":/REMainWindow/icons/dark/next.svg"));
        ui_->btn_CloseView->setIcon(QIcon(":/REMainWindow/icons/dark/cross.svg"));
        break;
    }
    }
}

} // namespace rfgui
