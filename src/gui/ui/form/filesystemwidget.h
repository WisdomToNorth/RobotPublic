#ifndef FILESYSTEM_WIDGET_H
#define FILESYSTEM_WIDGET_H

#include <QFileIconProvider>
#include <QFileInfo>
#include <QWidget>

class QTreeWidget;
class QTreeWidgetItem;

namespace rfgui
{

class WidgetFileSystem : public QWidget
{
    Q_OBJECT
public:
    WidgetFileSystem(QWidget *parent = nullptr);

    QFileInfo currentLocation() const;
    void setLocation(const std::string &fiLoc);
    void setLocation(const QFileInfo &fiLoc);

signals:
    void locationActivated(const QFileInfo &loc);

private:
    void onTreeItemActivated(QTreeWidgetItem *item, int column);
    void setCurrentFolder(const QDir &dir);
    void selectFileInCurrentFolder(const QString &fileName);

    QTreeWidget *tree_widget_ = nullptr;
    QFileInfo location_;
    QFileIconProvider file_icon_provider_;
};
} // namespace rfgui
#endif
