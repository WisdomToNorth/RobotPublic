
#include "listwidget.h"

#include <QModelIndex>
#include <QStringList>
#include <QStringListModel>

#include "ui_listwidget.h"

namespace rfgui
{

ListWidget::ListWidget(int number, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog())
{
    ui->setupUi(this);
    QStringList num;
    for (int i = 0; i < number; ++i)
    {
        num << QString::number(i + 1) + '.';
    }
    model_ = new QStringListModel(num, ui->listView);
    ui->listView->setModel(model_);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connectSS();
}

ListWidget::ListWidget(const std::vector<std::string> &list, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog())
{
    ui->setupUi(this);
    QStringList num;
    int n = static_cast<int>(list.size());
    for (int i = 0; i < n; ++i)
    {
        num << QString::number(i + 1) + '.' + list.at(i).c_str();
    }
    model_ = new QStringListModel(num, ui->listView);
    ui->listView->setModel(model_);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connectSS();
}

ListWidget::ListWidget(const QStringList &list, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog())
{
    ui->setupUi(this);
    QStringList num;
    size_t n = list.size();
    for (size_t i = 0; i < n; ++i)
    {
        int index = static_cast<int>(i);
        num << QString::number(index + 1) + '.' + list.at(index);
    }
    model_ = new QStringListModel(num, ui->listView);
    ui->listView->setModel(model_);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connectSS();
}

ListWidget::~ListWidget()
{
    delete model_;
    delete ui;
}

void ListWidget::closeEvent(QCloseEvent *)
{
    exec_ = ListHandleState::cancel;
}

void ListWidget::connectSS()
{
    connect(ui->listView, SIGNAL(clicked(QModelIndex)), this, SLOT(slotItemClicked(QModelIndex)));
    connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)), this,
            SLOT(slotItemDoubleClicked(QModelIndex)));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(pbOKPressed()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(pbCancelPressed()));
}

void ListWidget::pbOKPressed()
{
    if (index_c1_ == -1)
    {
        // TODO: seems not good.
        index_c1_ = 0;
    }
    exec_ = ListHandleState::KAccept;
}

void ListWidget::pbCancelPressed()
{
    exec_ = ListHandleState::cancel;
}

void ListWidget::slotItemClicked(QModelIndex index)
{
    std::string selected_str = index.data().toString().toStdString();
    filename_ = selected_str.substr(selected_str.find('.'));
    index_c1_ = index.row() + 1;
    emit selectNumSig(index_c1_);
}

QString ListWidget::getFirst()
{
    QString first_str = model_->index(0).data().toString();
    return first_str.mid(first_str.indexOf('{'));
}

void ListWidget::slotItemDoubleClicked(QModelIndex index)
{
    std::string selected_str = index.data().toString().toStdString();
    filename_ = selected_str.substr(selected_str.find('.'));
    index_c1_ = index.row();
    exec_ = ListHandleState::KAccept;
    this->accept();
}
} // namespace rfgui
