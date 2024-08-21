#ifndef GUI_POSITIONS_H
#define GUI_POSITIONS_H

#include <QDialog>
#include <QString>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui
{
class PositionsWidgetUI;
}
QT_END_NAMESPACE

namespace rfgui
{
class PositionsWidget : public QDialog
{
    Q_OBJECT

public:
    explicit PositionsWidget(QWidget *parent = nullptr);
    ~PositionsWidget();

private:
    Ui::PositionsWidgetUI *ui;

    void mainloop();

    QString darkgrey = "background-color: rgba(65, 65, 65, 0);\ncolor: rgb(255, 255, 255);";
    QString green = "background-color: rgba(170, 255, 0, 0);\ncolor: rgb(0, 0, 0);";
    QString red = "background-color: rgba(242, 0, 0, 0);\ncolor: rgb(0, 0, 0);";
    QString orange = "background-color: rgba(255, 170, 0, 0);\ncolor: rgb(0, 0, 0);";
};
} // namespace rfgui
#endif // GUI_POSITIONS_H
