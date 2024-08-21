
#pragma once
#ifndef LISTWIDGETR_H
#define LISTWIDGETR_H

#include <string>
#include <vector>

#include <QDialog>
#include <QStringListModel>
namespace Ui
{
class Dialog;
};

namespace rfgui
{

class ListWidget : public QDialog
{
    Q_OBJECT
public:
    ListWidget(const std::vector<std::string> &list, QWidget *parent = nullptr);
    ListWidget(const QStringList &list, QWidget *parent = nullptr);
    ListWidget(int number, QWidget *parent = nullptr);
    ~ListWidget() override;
    ListWidget(const ListWidget &) = delete;
    ListWidget &operator=(const ListWidget &) = delete;

    QString getFirst();
    enum class ListHandleState
    {
        unhandle,
        cancel,
        KAccept
    };

    // TODO: encapsulate this.
    std::string filename_;
    // NOTE: ui count from 1 for user, real index count from 0.
    int index_c1_ = -1;
    ListHandleState exec_ = ListHandleState::unhandle;

signals:
    void selectNumSig(int);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void slotItemClicked(QModelIndex index);
    void slotItemDoubleClicked(QModelIndex index);
    void pbOKPressed();
    void pbCancelPressed();

private:
    void connectSS();

private:
    Ui::Dialog *ui;
    QStringListModel *model_;
};
} // namespace rfgui
#endif
