#include "rfLogListWidget.h"

#include <QApplication>
#include <QClipboard>
#include <QKeyEvent>
namespace rflog
{
RfLogListWidget::RfLogListWidget(QWidget *parent)
    : QListWidget(parent)
{
}

void RfLogListWidget::addLogEntry(LogEntryType type, const std::string &message)
{
    addLogEntry(type, QString::fromStdString(message));
    this->scrollToBottom();
}

void RfLogListWidget::addLogEntry(LogEntryType type, const QString &entry)
{
    auto item = new QListWidgetItem(entry);
    if (type == LogEntryType::Error)
    {
        item->setForeground(Qt::red);
    }
    else if (type == LogEntryType::Warning)
    {
        item->setForeground(Qt::darkYellow);
    }

    addItem(item);
}

void RfLogListWidget::keyPressEvent(QKeyEvent *event)
{
    // If the pressed key matches the Ctrl+C (Copy) shortcut
    if (event->matches(QKeySequence::Copy))
    {
        int itemsCount = count(); // Get the number of items in the list
        QStringList strings;      // Create a QStringList to store the text of the selected items

        // Iterate through all the items in the list
        for (int i = 0; i < itemsCount; ++i)
        {
            // If the item is selected
            if (item(i)->isSelected())
            {
                // Add the text of the item to the QStringList
                strings << item(i)->text();
            }
        }

        // Join all the text in the QStringList into a single string and copy it to the clipboard
        QApplication::clipboard()->setText(strings.join("\n"));
    }
    else
    {
        // If the pressed key is not Ctrl+C, call the base class's keyPressEvent method to handle
        // the event
        QListWidget::keyPressEvent(event);
    }
}
} // namespace rflog
