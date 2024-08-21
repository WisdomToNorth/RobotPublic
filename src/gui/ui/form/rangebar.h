
#ifndef DISTBAR_H
#define DISTBAR_H

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <QDialog>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>
#include <QScrollArea>
#include <QVBoxLayout>

namespace Ui
{
class DistBarWidget;
};
namespace rfbase
{
class RatioHelper;
};

namespace rfgui
{

class DrawArea : public QWidget
{
public:
    explicit DrawArea(QWidget *parent = nullptr);
    ~DrawArea() = default;
    // All input is double ratio, from 0 to 1
    void setNewData(std::optional<double> left_bound, std::optional<double> right_bound,
                    const std::vector<double> &values);

protected:
    void paintEvent(QPaintEvent *) override;

private:
    std::optional<double> left_bound_line_loc_;
    std::optional<double> right_bound_line_loc_;
    std::vector<double> values_line_loc_;
};

class BarWidget : public QWidget
{
public:
    BarWidget(QWidget *parent = nullptr);
    ~BarWidget() override;

    void setLabelValue(std::optional<double> left_bound, std::optional<double> right_bound,
                       const std::vector<double> &values);
    void setDrawValue(std::optional<double> left_bound_ratio,
                      std::optional<double> right_bound_ratio,
                      const std::vector<double> &values_ratio);

    void setLabel(const std::string &label);
    void updateUI();

private:
    Ui::DistBarWidget *ui_;
    DrawArea *draw_area_;
};

class BarGroup : public QWidget
{
public:
    void resizeData(size_t size);
    void setRange(size_t index, std::optional<double> lowest_value,
                  std::optional<double> highest_value);
    void setValue(size_t index, const std::vector<double> &value);
    void setLabel(size_t index, const std::string &label);
    void clear();

private:
    BarGroup(QWidget *parent = nullptr);
    ~BarGroup() override;
    BarGroup(const BarGroup &) = delete;
    BarGroup &operator=(const BarGroup &) = delete;

    void updateUIByDataGroups();

private:
    friend class RangeGroup;
    BarWidget *bar_widget_;

    struct Data
    {
        rfbase::RatioHelper *ratio_helper_;
        std::string label;
        std::vector<double> value_;
    };

    std::vector<Data> data_;
};

class RangeGroup : public QDialog
{
    std::unordered_map<std::string_view, BarGroup *> dist_bars_;
    QVBoxLayout *layout_;
    QScrollArea *scroll_area_;
    QPoint off_pos_;

public:
    RangeGroup(QWidget *parent = nullptr);
    ~RangeGroup() override;
    RangeGroup(const RangeGroup &) = delete;
    RangeGroup &operator=(const RangeGroup &) = delete;

    BarGroup *getDistBar(const std::string_view &widget_name);
    bool hasDistBar(const std::string_view &widget_name) const;

    void updateLocation(int x, int y);
    void updateUI();
    void resetData();

protected:
    void setInitPos(const QPoint &loc);
    void updateLoactionByInitPos(int x, int y);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};
} // namespace rfgui
#endif
