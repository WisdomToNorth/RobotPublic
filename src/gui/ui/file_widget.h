
#pragma once
#ifndef FILE_WIDGET_H
#define FILE_WIDGET_H

#include <unordered_map>

#include <QTreeWidgetItem>
#include <QWidget>

#include <boost/bimap/bimap.hpp>

#include "rfapp/observer.h"
#include "rfdb/dbVariant.h"

#include "viewnode.h"

class QTreeWidget;
class Bnd_OBB;
class QButtonGroup;

QT_BEGIN_NAMESPACE
namespace Ui
{
class FilemgrWidget;
}
QT_END_NAMESPACE

namespace rfgeo
{
class Abassica;
};
namespace rfgui
{
class FileTreeWidget : public QWidget, public rfapp::Observer<ViewNode>
{
    Q_OBJECT
public:
    FileTreeWidget(QWidget *parent = nullptr);
    ~FileTreeWidget() override;
    FileTreeWidget(const FileTreeWidget &) = delete;
    FileTreeWidget &operator=(const FileTreeWidget &) = delete;

    template <typename T>
    void buildTreeWidgetItems(ViewNode *viewtree, T *parent);

    void addModel(ViewNode *viewtree);
    bool removeModel(ViewNode *viewtree);
    void createAuxObject(int viewer_index);

    ViewNode *getSelectedModel();

    void updateEvent(ViewNode *sub, rfapp::KMode mode) override;

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void itemClickedSlot(QTreeWidgetItem *current, int column);
    void itemChangedSlot(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void itemStateChangedSlot(QTreeWidgetItem *, int column);

private:
    void changeChildrenState(QTreeWidgetItem *item);
    void changeParentState(QTreeWidgetItem *item);

    QTreeWidget *getTreeWidget(QTreeWidgetItem *item) const;
    QTreeWidgetItem *getRootNode(QTreeWidgetItem *item);
    void deleteLeaves(QTreeWidgetItem *item);

private:
    Ui::FilemgrWidget *ui;

    boost::bimaps::bimap<QTreeWidgetItem *, ViewNode *> item_map_; // TODO: remove this

    rfdb::dbPart *world_abassica_ = nullptr;
    rfdb::dbPart *ref_abassica_ = nullptr;
    rfdb::dbPart *temp_abassica_ = nullptr;

    bool show_bbox_ = false;

    // If update is called from this widget, it will ignore the update signal from the model.
    // We may find a better way to handle this.
    bool about_to_update_ = false;

    class UpdateHelper
    {
        FileTreeWidget *parent_;

    public:
        UpdateHelper(FileTreeWidget *parent)
            : parent_(parent)
        {
            parent_->about_to_update_ = true;
        }
        ~UpdateHelper()
        {
            parent_->about_to_update_ = false;
        }
    };
};
} // namespace rfgui
#endif
