
#include "file_widget.h"

#include <QButtonGroup>
#include <QFileDialog>
#include <QTreeWidget>

#include <AIS_TextLabel.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <magic_enum/magic_enum.hpp>

#include "rfdb/dbutils/shapebuilder.h"
#include "rfgeo3d/geoutils.h"
#include "rfgeo3d/vmselectmode.h"
#include "rflog/rfLog.h"

#include "app_system.h"
#include "ui_file_widget.h"
#include "viewerwrapper.h"

namespace rfgui
{
using namespace rfgeo;

FileTreeWidget::FileTreeWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FilemgrWidget())
{
    ui->setupUi(this);

    ui->model_widget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->model_widget->setAutoScroll(true);
    ui->model_widget->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

    connect(ui->model_widget, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this,
            SLOT(itemStateChangedSlot(QTreeWidgetItem *, int)));
    connect(ui->model_widget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
            SLOT(itemClickedSlot(QTreeWidgetItem *, int)));
    connect(ui->model_widget, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
            this, SLOT(itemChangedSlot(QTreeWidgetItem *, QTreeWidgetItem *)));
}

FileTreeWidget::~FileTreeWidget()
{
    delete ui;
}

void FileTreeWidget::resizeEvent(QResizeEvent *)
{
    QHeaderView *header = ui->model_widget->header();
    header->setSectionResizeMode(QHeaderView::Interactive);
    int totalWidth = ui->model_widget->width();
    header->resizeSection(0, totalWidth * 7 / 10);
    header->resizeSection(1, totalWidth * 3 / 10);
}

void FileTreeWidget::createAuxObject(int viewer_index)
{
    world_abassica_ = rfdb::ShapeBuilder::buildAbassica(
        gp_Ax2(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1), gp_Dir(1, 0, 0)), true, 500.0);
    world_abassica_->getProperty()->setName("world abassica");
    world_abassica_->setTransparency(0.7);
    world_abassica_->setVisiable(true);
    GApp::instance().addDbVariant(rfdb::dbVariant(world_abassica_), UseType::Auxiliary,
                                  viewer_index);

    ref_abassica_ = rfdb::ShapeBuilder::buildAbassica(
        gp_Ax2(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1), gp_Dir(1, 0, 0)), false, 100.0);
    ref_abassica_->getProperty()->setName("ref abassica");
    ref_abassica_->setVisiable(false);
    GApp::instance().addDbVariant(rfdb::dbVariant(ref_abassica_), UseType::Auxiliary, viewer_index);

    temp_abassica_ = rfdb::ShapeBuilder::buildAbassica(
        gp_Ax2(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1), gp_Dir(1, 0, 0)), false, 30.0);
    temp_abassica_->getProperty()->setName("temp abassica");
    temp_abassica_->setVisiable(false);
    GApp::instance().addDbVariant(rfdb::dbVariant(temp_abassica_), UseType::Auxiliary,
                                  viewer_index);
}

void FileTreeWidget::itemClickedSlot(QTreeWidgetItem *current, int /*column*/)
{
    // Check if the item is selected
    UpdateHelper helper(this);
    if (current)
    {
        ViewNode *cur_treenode = item_map_.left.at(current);
        cur_treenode->is_selected_ = true;
        GApp::instance().updateViewTree(cur_treenode, rfapp::KMode::highlight);
    }
}

void FileTreeWidget::itemChangedSlot(QTreeWidgetItem * /*current*/, QTreeWidgetItem *previous)
{
    // Check if the item is selected
    UpdateHelper helper(this);

    if (previous)
    {
        ViewNode *pre_treenode = item_map_.left.at(previous);
        pre_treenode->is_selected_ = false;
        GApp::instance().updateViewTree(pre_treenode, rfapp::KMode::highlight);
    }
}

QTreeWidget *FileTreeWidget::getTreeWidget(QTreeWidgetItem *item) const
{
    return item->treeWidget();
}

void FileTreeWidget::itemStateChangedSlot(QTreeWidgetItem *item, int column)
{
    QTreeWidget *treewidget = item->treeWidget();
    treewidget->blockSignals(true);
    changeChildrenState(item);
    changeParentState(item);
    treewidget->blockSignals(false);

    ViewNode *treenode = item_map_.left.at(item);
    treenode->is_visible_ = item->checkState(column);

    UpdateHelper helper(this);
    GApp::instance().updateViewTree(treenode, rfapp::KMode::hide);
    QWidget::update();
}

ViewNode *FileTreeWidget::getSelectedModel()
{
    const QList<QTreeWidgetItem *> &selected = ui->model_widget->selectedItems();
    if (selected.empty())
    {
        return nullptr;
    }
    else
    {
        ViewNode *treenode = item_map_.left.at(selected.front());
        return treenode;
    }
}

void FileTreeWidget::changeParentState(QTreeWidgetItem *item)
{
    auto parent = item->parent();
    if (!parent)
    {
        return;
    }
    Qt::CheckState current_state = item->checkState(0);

    bool level_same = true;
    for (int i = 0; i < parent->childCount(); ++i)
    {
        if (parent->child(i)->checkState(0) != current_state)
        {
            level_same = false;
            break;
        }
    }

    if (level_same)
    {
        parent->setCheckState(0, current_state);
    }
    else
    {
        parent->setCheckState(0, Qt::CheckState::PartiallyChecked);
    }
    changeParentState(parent);
}

void FileTreeWidget::changeChildrenState(QTreeWidgetItem *item)
{
    int childcnt = item->childCount();
    for (int i = 0; i < childcnt; ++i)
    {
        QTreeWidgetItem *childItem = item->child(i);
        childItem->setCheckState(0, item->checkState(0));
        changeChildrenState(childItem);
    }
}

void FileTreeWidget::updateEvent(ViewNode *sub, rfapp::KMode mode)
{
    if (about_to_update_)
    {
        return;
    }

    switch (mode)
    {
    case rfapp::KMode::add:
    {
        addModel(sub);
        break;
    }
    case rfapp::KMode::remove:
    {
        removeModel(sub);
        break;
    }
    case rfapp::KMode::hide:
    case rfapp::KMode::update:
    case rfapp::KMode::highlight:
    default: break;
    }
}

void FileTreeWidget::addModel(ViewNode *viewtree)
{
    const auto &use_type = viewtree->type_;

    if (use_type == UseType::Database || use_type == UseType::Auxiliary)
    {
        ui->model_widget->blockSignals(true);
        buildTreeWidgetItems(viewtree, ui->model_widget);
        ui->model_widget->blockSignals(false);
        ui->model_widget->setCurrentItem(ui->model_widget->topLevelItem(0));
    }
    else
    {
        rflog::debug("addModel: unknown use type");
    }
}

bool FileTreeWidget::removeModel(ViewNode *viewtree)
{
    const auto &usetype = viewtree->type_;

    if (usetype == UseType::Auxiliary || usetype == UseType::Database)
    {
        ui->model_widget->blockSignals(true);
        auto item = item_map_.right.at(viewtree);
        deleteLeaves(item);
        ui->model_widget->blockSignals(false);
    }
    return true;
}

template <typename T>
void FileTreeWidget::buildTreeWidgetItems(ViewNode *viewtree, T *parent)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, viewtree->name_.c_str());
    item->setText(1, QString::number(viewtree->index_in_viewer_));
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
    Qt::CheckState state = viewtree->is_visible_ ? Qt::Checked : Qt::Unchecked;
    item->setCheckState(0, state);

    item_map_.insert(
        boost::bimaps::bimap<QTreeWidgetItem *, ViewNode *>::value_type(item, viewtree));

    for (auto &node : viewtree->children_)
    {
        buildTreeWidgetItems(node, item);
    }
}

QTreeWidgetItem *FileTreeWidget::getRootNode(QTreeWidgetItem *item)
{
    while (item->parent())
    {
        item = item->parent();
    }
    return item;
}

void FileTreeWidget::deleteLeaves(QTreeWidgetItem *item)
{
    for (int i = 0; i < item->childCount(); ++i)
    {
        auto child = item->child(i);
        deleteLeaves(child);
    }
    delete item;
    item = nullptr;
}
} // namespace rfgui
