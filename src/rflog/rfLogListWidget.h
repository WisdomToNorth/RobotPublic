#pragma once
#ifndef CUSTOM_LISTVIEW_H
#define CUSTOM_LISTVIEW_H

#include <QListWidget>

#include "rfLogEntryType.h"

namespace rflog
{
class RfLogListWidget : public QListWidget
{
public:
    RfLogListWidget(QWidget *parent = nullptr);
    void addLogEntry(LogEntryType type, const std::string &message);

private:
    void addLogEntry(LogEntryType type, const QString &entry);

protected:
    void keyPressEvent(QKeyEvent *event) override;
};
} // namespace rflog
#endif
