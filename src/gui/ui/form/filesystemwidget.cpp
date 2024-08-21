#include "filesystemwidget.h"

#include <QBoxLayout>
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QTreeWidget>

namespace rfgui
{

namespace Internal
{

static QString absolutePath(const QFileInfo &fi)
{
    return fi.isDir() ? fi.absoluteFilePath() : fi.absolutePath();
}

static QString bytesText(uint64_t sizeBytes, const QLocale &locale)
{
    constexpr int oneMB = 1024 * 1024;
    const quint64 qSizeBytes = sizeBytes; // Avoids ambiguous calls of QLocale::toString()
    if (sizeBytes < 1024)
        return QStringLiteral("%1%2").arg(locale.toString(qSizeBytes), QStringLiteral("B"));
    else if (sizeBytes < oneMB)
        return QStringLiteral("%1%2").arg(locale.toString(qSizeBytes / 1024), QStringLiteral("KB"));
    else if (sizeBytes < (10 * oneMB))
        return QStringLiteral("%1%2").arg(
            locale.toString(static_cast<double>(qSizeBytes) / oneMB, 'f', 2), QStringLiteral("MB"));
    else if (sizeBytes < (100 * oneMB))
        return QStringLiteral("%1%2").arg(
            locale.toString(static_cast<double>(qSizeBytes) / oneMB, 'f', 1), QStringLiteral("MB"));
    else
        return QStringLiteral("%1%2").arg(locale.toString(qSizeBytes / oneMB),
                                          QStringLiteral("MB"));
}

} // namespace Internal

WidgetFileSystem::WidgetFileSystem(QWidget *parent)
    : QWidget(parent)
    , tree_widget_(new QTreeWidget(this))
{
    auto layout = new QVBoxLayout(this);
    layout->addWidget(tree_widget_);
    // layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);

    tree_widget_->setSelectionMode(QAbstractItemView::SingleSelection);
    tree_widget_->setColumnCount(1);
    tree_widget_->setIndentation(0);

    QObject::connect(tree_widget_, &QTreeWidget::itemActivated, this,
                     &WidgetFileSystem::onTreeItemActivated);
}

QFileInfo WidgetFileSystem::currentLocation() const
{
    return location_;
}

void WidgetFileSystem::setLocation(const std::string &fiLoc)
{
    setLocation(QFileInfo(QString::fromStdString(fiLoc)));
}

void WidgetFileSystem::setLocation(const QFileInfo &fiLoc)
{
    const QString cur_path_loc = Internal::absolutePath(location_);
    const QString path_loc = Internal::absolutePath(fiLoc);
    if (cur_path_loc == path_loc && fiLoc.isFile())
    {
        this->selectFileInCurrentFolder(fiLoc.fileName());
    }
    else
    {
        this->setCurrentFolder(path_loc);
        this->selectFileInCurrentFolder(fiLoc.fileName());
    }

    location_ = fiLoc;
}

void WidgetFileSystem::onTreeItemActivated(QTreeWidgetItem *item, int column)
{
    if (item != nullptr && column == 0)
    {
        if (item->text(0) == QLatin1String(".."))
        {
            QDir dir = Internal::absolutePath(location_);
            dir.cdUp();
            this->setCurrentFolder(dir);
            location_ = QFileInfo(dir.absolutePath());
        }
        else
        {
            const QDir dir(Internal::absolutePath(location_));
            const QFileInfo fi(dir, item->text(0));
            if (fi.isDir())
            {
                this->setLocation(fi);
            }
            else
            {
                emit locationActivated(fi);
                location_ = fi;
            }
        }
    }
}

void WidgetFileSystem::setCurrentFolder(const QDir &dir)
{
    tree_widget_->clear();
    QList<QTreeWidgetItem *> listItem;
    if (dir.exists())
    {
        const QFileInfoList listEntryFileInfo =
            dir.entryInfoList(QDir::Files | QDir::AllDirs | QDir::NoDot, QDir::DirsFirst);
        for (const QFileInfo &fi : listEntryFileInfo)
        {
            auto item = new QTreeWidgetItem;
            if (fi.fileName() != QLatin1String("."))
            {
                item->setText(0, fi.fileName());
                item->setIcon(0, file_icon_provider_.icon(fi));
                const QString itemTooltip =
                    tr("%1\nSize: %2\nLast modified: %3")
                        .arg(QDir::toNativeSeparators(fi.absoluteFilePath()))
                        .arg(Internal::bytesText(fi.size(), QLocale("RGB(%1, %2 %3)")))
                        .arg(QLocale::system().toString(fi.lastModified(), QLocale::LongFormat));
                item->setToolTip(0, itemTooltip);
            }

            listItem.push_back(item);
        }
    }

    tree_widget_->addTopLevelItems(listItem);
    tree_widget_->headerItem()->setText(0, dir.dirName());
}

void WidgetFileSystem::selectFileInCurrentFolder(const QString &fileName)
{
    for (int i = 0; i < tree_widget_->topLevelItemCount(); ++i)
    {
        if (tree_widget_->topLevelItem(i)->text(0) == fileName)
        {
            tree_widget_->clearSelection();
            tree_widget_->topLevelItem(i)->setSelected(true);
            return;
        }
    }
}

} // namespace rfgui
