#include "rangebar.h"

#include <cmath>
#include <iostream>

#include <QGuiApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include <QScrollBar>
#include <QTimer>
#include <QVBoxLayout>

#include "rfbase/datautils.h"
#include "rfbase/ratiohelper.h"
#include "rflog/rfLog.h"

#include "frameutils/qtutils.h"
#include "ui_rangebar.h"

namespace rfgui
{
DrawArea::DrawArea(QWidget *parent)
    : QWidget(parent)

{
    this->setMinimumWidth(100);
    this->setFixedHeight(10);
    this->setContentsMargins(0, 0, 0, 0);
}

void DrawArea::setNewData(std::optional<double> left_bound, std::optional<double> right_bound,
                          const std::vector<double> &values)
{
    left_bound_line_loc_ = left_bound;
    right_bound_line_loc_ = right_bound;
    values_line_loc_ = values;
}

void DrawArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const int width = this->width();
    const int height = this->height();

    static const std::vector<QColor> valid_colors{Qt::GlobalColor::green,
                                                  Qt::GlobalColor::darkGreen, Qt::GlobalColor::blue,
                                                  Qt::GlobalColor::darkBlue};
    static const std::vector<QColor> invalid_colors{Qt::GlobalColor::red, Qt::GlobalColor::darkRed,
                                                    Qt::GlobalColor::yellow,
                                                    Qt::GlobalColor::darkYellow};

    QRectF total_rect(0, 0, width, height);
    painter.fillRect(total_rect, Qt::white);

    std::sort(values_line_loc_.begin(), values_line_loc_.end(), std::greater<double>());

    for (size_t i = 0; i < values_line_loc_.size(); ++i)
    {
        const double &value = values_line_loc_[i];
        assert(value >= 0 && value <= 1);

        bool in_range = true;
        in_range =
            in_range &&
            (left_bound_line_loc_.has_value() ? value >= left_bound_line_loc_.value() : true);
        in_range =
            in_range &&
            (right_bound_line_loc_.has_value() ? value <= right_bound_line_loc_.value() : true);

        QColor color = in_range ? valid_colors[i % valid_colors.size()] :
                                  invalid_colors[i % invalid_colors.size()];

        // set min as 0.1 to avoid width is too small to see
        int value_loc = int(width * std::max(value, 0.1));
        QRectF rect(0, 0, value_loc, height);
        painter.fillRect(rect, color);
    }

    // Draw the line at lowest and highest
    painter.setPen(Qt::black);
    if (left_bound_line_loc_.has_value())
    {
        int left_line = int(width * left_bound_line_loc_.value());
        painter.drawLine(left_line, 0, left_line, height);
    }
    if (right_bound_line_loc_.has_value())
    {
        int right_line = int(width * right_bound_line_loc_.value());
        painter.drawLine(right_line, 0, right_line, height);
    }
}

BarWidget::BarWidget(QWidget *parent)
    : QWidget(parent)
{
    ui_ = new Ui::DistBarWidget();
    ui_->setupUi(this);
    this->setWindowOpacity(0.6);
    this->setFixedWidth(300);
    this->setWindowFlag(Qt::FramelessWindowHint);

    draw_area_ = new DrawArea(this);
    QVBoxLayout *layout = new QVBoxLayout;
    ui_->widget_bar->setLayout(layout);

    // Add spacers to the top and bottom of the layout to center the draw_area_
    layout->addStretch();
    layout->addWidget(draw_area_);
    layout->addStretch();
}

BarWidget::~BarWidget()
{
    delete ui_;
    delete draw_area_;
}

void BarWidget::setLabelValue(std::optional<double> left_bound, std::optional<double> right_bound,
                              const std::vector<double> &)
{
    ui_->lbl_min_value->setText(
        left_bound.has_value() ? QString::number(left_bound.value(), 'f', 2) : "-");
    ui_->lbl_max_value->setText(
        right_bound.has_value() ? QString::number(right_bound.value(), 'f', 2) : "-");
    std::string value_text = "";
    ui_->lbl_value->setText(QString::fromStdString(value_text));
}

void BarWidget::setDrawValue(std::optional<double> left_bound_ratio,
                             std::optional<double> right_bound_ratio,
                             const std::vector<double> &values_ratio)
{
    draw_area_->setNewData(left_bound_ratio, right_bound_ratio, values_ratio);
}

void BarWidget::setLabel(const std::string &label)
{
    ui_->lbl_text->setText(QString::fromStdString(label));
}

void BarWidget::updateUI()
{
    draw_area_->update();
}

BarGroup::BarGroup(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowFlag(Qt::FramelessWindowHint);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSizeConstraint(QLayout::SetMinimumSize);
    layout->setContentsMargins(1, 1, 1, 1);
    this->setLayout(layout);
    bar_widget_ = new BarWidget(this);
    this->layout()->addWidget(bar_widget_);
}

BarGroup::~BarGroup()
{
    for (auto &l_data : data_)
    {
        delete l_data.ratio_helper_;
    }
}

void BarGroup::resizeData(size_t index)
{
    while (data_.size() <= index)
    {
        data_.emplace_back(Data());
        data_.back().ratio_helper_ = new rfbase::RatioHelper();
    }
}

void BarGroup::clear()
{
    for (auto &l_data : data_)
    {
        delete l_data.ratio_helper_;
    }
    data_.clear();
}

void BarGroup::updateUIByDataGroups()
{
    size_t left_most_diff = 0;
    size_t right_most_diff = 0;
    double left_diff = (double)INT_MAX;  // value- left_bound
    double right_diff = (double)INT_MAX; // right_bound - value
    for (size_t i = 0; i < data_.size(); ++i)
    {
        const auto &l_data = data_[i];
        std::optional<double> left_bound = l_data.ratio_helper_->getLeftBound();
        std::optional<double> right_bound = l_data.ratio_helper_->getRightBound();

        for (const auto &value : l_data.value_)
        {
            if (left_bound.has_value())
            {
                double diff = value - left_bound.value();
                if (diff < left_diff)
                {
                    left_diff = diff;
                    left_most_diff = i;
                }
            }
            if (right_bound.has_value())
            {
                double diff = right_bound.value() - value;
                if (diff < right_diff)
                {
                    right_diff = diff;
                    right_most_diff = i;
                }
            }
        }
    }
    const auto &left_most_data = data_[left_most_diff];
    const auto &right_most_data = data_[right_most_diff];
    if (left_most_data.value_.empty() || right_most_data.value_.empty())
    {
        return;
    }

    double left_val = *std::min_element(left_most_data.value_.begin(), left_most_data.value_.end());
    double right_val =
        *std::max_element(right_most_data.value_.begin(), right_most_data.value_.end());
    double min_val_ratio = left_most_data.ratio_helper_->getValueRatio(left_val);
    double max_val_ratio = right_most_data.ratio_helper_->getValueRatio(right_val);
    double left_bound_ratio = left_most_data.ratio_helper_->getLowestRatio();
    double right_bound_ratio = right_most_data.ratio_helper_->getHighestRatio();

    bar_widget_->setLabelValue(left_most_data.ratio_helper_->getLeftBound(),
                               right_most_data.ratio_helper_->getRightBound(),
                               {left_val, right_val});

    bar_widget_->setDrawValue(left_bound_ratio, right_bound_ratio, {min_val_ratio, max_val_ratio});
    bar_widget_->updateUI();
}

void BarGroup::setRange(size_t index, std::optional<double> lowest_value,
                        std::optional<double> highest_value)
{
    resizeData(index);
    data_[index].ratio_helper_->setRange(lowest_value, highest_value);
}

void BarGroup::setValue(size_t index, const std::vector<double> &value)
{
    resizeData(index);
    data_[index].value_ = value;
}

void BarGroup::setLabel(size_t index, const std::string &label)
{
    resizeData(index);
    data_[index].label = label;
}

void RangeGroup::updateLocation(int x, int y)
{
    this->move(x, y);
}

void RangeGroup::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QPoint startPos = QtUtils::getMouseEventGlobalPoint(event);
        off_pos_ = startPos - geometry().topLeft();
    }
    QDialog::mousePressEvent(event);
}

void RangeGroup::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        QPoint endPos = QtUtils::getMouseEventGlobalPoint(event);
        auto d = endPos - off_pos_;
        move(d.x(), d.y());
    }

    QDialog::mouseMoveEvent(event);
}

void RangeGroup::setInitPos(const QPoint &loc)
{
    off_pos_ = loc;
}

void RangeGroup::updateLoactionByInitPos(int x, int y)
{
    QPoint endPos(x, y);
    auto d = endPos - off_pos_;
    move(d.x(), d.y());
}

void RangeGroup::mouseReleaseEvent(QMouseEvent *event)
{
    QDialog::mouseReleaseEvent(event);
}

bool RangeGroup::hasDistBar(const std::string_view &widget_name) const
{
    return dist_bars_.find(widget_name) != dist_bars_.end();
}

BarGroup *RangeGroup::getDistBar(const std::string_view &widget_name)
{
    if (dist_bars_.find(widget_name) != dist_bars_.end())
    {
        return dist_bars_[widget_name];
    }

    BarGroup *dist_bar = new BarGroup(this);
    dist_bars_[widget_name] = dist_bar;
    dist_bar->setObjectName(widget_name.data());
    std::string bolder_qss = "#" + std::string(widget_name) + "{border: 1px solid black;}";
    std::string qss = bolder_qss;
    dist_bar->setStyleSheet(QString::fromStdString(qss));

    // Add to layout only if the scrollbar is visible
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(dist_bar);
    hLayout->addSpacing(10); // Add spacing to the right of the bar
    layout_->addLayout(hLayout);

    return dist_bar;
}

RangeGroup::RangeGroup(QWidget *parent)
    : QDialog(parent, Qt::Tool | Qt::WindowDoesNotAcceptFocus)
{
    setWindowFlag(Qt::FramelessWindowHint);
    setWindowOpacity(0.6);

    layout_ = new QVBoxLayout;
    layout_->setContentsMargins(0, 0, 0, 0);
    QWidget *content_widget = new QWidget;
    content_widget->setLayout(layout_);

    scroll_area_ = new QScrollArea(this);
    scroll_area_->setWidget(content_widget);
    scroll_area_->setWidgetResizable(true);
    scroll_area_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll_area_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll_area_->setMaximumHeight(400);

    this->setLayout(new QVBoxLayout(this));
    this->layout()->addWidget(scroll_area_);
    this->layout()->setContentsMargins(3, 3, 3, 3);
    layout_->setSizeConstraint(QLayout::SetNoConstraint);

    this->setStyleSheet("font-size: 7pt;");
}

void RangeGroup::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
}

RangeGroup::~RangeGroup()
{
    for (auto &it : dist_bars_)
    {
        delete it.second;
    }
}

void RangeGroup::updateUI()
{
    for (auto &[_, bar_widget] : dist_bars_)
    {
        bar_widget->updateUIByDataGroups();
    }
    QTimer::singleShot(0, this, &RangeGroup::adjustSize);
}

void RangeGroup::resetData()
{
    for (auto &it : dist_bars_)
    {
        it.second->clear();
    }
}
} // namespace rfgui
