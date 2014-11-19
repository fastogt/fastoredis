#include "gui/base/graph_widget.h"

#include <math.h>

#include <QStylePainter>
#include <QDateTime>
#include <QWheelEvent>

namespace fastoredis
{
    plot_settings::plot_settings(qreal min_x, qreal max_x, qreal min_y, qreal max_y, unsigned num_x_ticks, unsigned num_y_ticks)
        :min_x_(min_x), max_x_(max_x),
        min_y_(min_y), max_y_(max_y),
        num_x_ticks_(num_x_ticks), num_y_ticks_(num_y_ticks)
    {
    }

    plot_settings plot_settings::create_child(qreal dx, qreal dy, QRect rect)const
    {
        plot_settings settings(*this);
        settings.min_x_ = min_x_ + dx * rect.left();
        settings.max_x_ = min_x_ + dx * rect.right();
        settings.min_y_ = max_y_ - dy * rect.bottom();
        settings.max_y_ = max_y_ - dy * rect.top();
        settings.adjust();

        return settings;
    }

    void plot_settings::scroll(int dx, int dy)
    {
        qreal stepX = span_x() / num_x_ticks_;
        qreal stepY = span_y() / num_y_ticks_;
        plot_settings tmp(min_x_ + dx * stepX, max_x_ + dx * stepX, min_y_ +dy * stepY, max_y_ + dy * stepY);
        if(is_valid_setting(tmp))
        {
            min_x_ += dx * stepX;
            max_x_ += dx * stepX;
            min_y_ += dy * stepY;
            max_y_ += dy * stepY;
        }
    }

    void plot_settings::adjust()
    {
        adjust_axis(min_x_, max_x_, num_x_ticks_);
        adjust_axis(min_y_, max_y_, num_y_ticks_);
    }

    bool plot_settings::is_valid_setting(const plot_settings& settings)
    {
        return settings.min_x_ >= 0 && settings.min_y_ >=0;
    }

    void plot_settings::adjust_axis(qreal &min, qreal &max, unsigned &numTicks)
    {
        static const unsigned MinTicks = 4;
        qreal grossStep = (max - min) / MinTicks;
        qreal step = pow(10.0, floor(log10(grossStep)));

        if (5 * step < grossStep){
            step *= 5;
        }
        else if (2 * step < grossStep){
            step *= 2;
        }

        numTicks = int(ceil(max / step) - floor(min / step));
        if (numTicks < MinTicks){
            numTicks = MinTicks;
        }
        min = floor(min / step) * step;
        max = ceil(max / step) * step;
    }

    GraphWidget::GraphWidget(QWidget *parent)
        : QWidget(parent),cur_zoom_(0),rubber_band_is_shown_(false), nodes_()
    {
        setBackgroundRole(QPalette::Light);
        setAutoFillBackground(true);
        setFocusPolicy(Qt::StrongFocus);
    }

    void GraphWidget::setNodes(const nodes_container_type& nodes)
    {
        nodes_ = nodes;
        zoomStack.clear();
        if(!nodes_.empty()){
            qreal min_x = nodes_[0].first;
            qreal max_x = nodes_[0].first;
            qreal min_y = nodes_[0].second;
            qreal max_y = nodes_[0].second;

            for(unsigned int i = 0; i < nodes_.size(); ++i){
                qreal curX = nodes_[i].first;
                qreal curY = nodes_[i].second;
                if(min_x > curX){
                    min_x = curX;
                }

                if(max_x < curX){
                    max_x = curX;
                }

                if(min_y > curY){
                    min_y = curY;
                }

                if(max_y < curY){
                    max_y = curY;
                }
            }

            zoomStack.push_back(plot_settings(min_x, max_x, min_y, max_y, 5, 5));
        }
        update();
    }

    void GraphWidget::zoom_out()
    {
        if (cur_zoom_ > 0){
            --cur_zoom_;
            update();
        }
    }

    void GraphWidget::zoom_in()
    {
        if (cur_zoom_ < zoomStack.size() - 1){
            ++cur_zoom_;
            update();
        }
    }

    void GraphWidget::draw_grid(QPainter *painter)
    {
        if(zoomStack.empty()){
            return;
        }

        QRect rect = get_paint_rect();
        if (!rect.isValid()){
            return;
        }

        plot_settings settings = zoomStack[cur_zoom_];
        painter->setPen(QColor(grid_color));
        for (unsigned i = 0; i <= settings.num_x_ticks_; ++i){
             int x = rect.left() + (i * (rect.width() - 1) / settings.num_x_ticks_);
             qreal label = settings.min_x_ + (i * settings.span_x() / settings.num_x_ticks_);
             painter->drawLine(x, rect.top(), x, rect.bottom());
             painter->drawLine(x, rect.bottom(), x, rect.bottom() + 5);
             QDateTime t = QDateTime::fromMSecsSinceEpoch((qint64)label);
             painter->drawText(x - 50, rect.bottom() + 5, 120, 20, Qt::AlignHCenter | Qt::AlignTop, t.toString(Qt::ISODate));
        }
        for (unsigned j = 0; j <= settings.num_y_ticks_; ++j){
            int y = rect.bottom() - (j * (rect.height() - 1) / settings.num_y_ticks_);
            qreal label = settings.min_y_ + (j * settings.span_y() / settings.num_y_ticks_);
            painter->drawLine(rect.left(), y, rect.right(), y);
            painter->drawLine(rect.left() - 5, y, rect.left(), y);
            painter->drawText(rect.left() - margin, y - 10, margin - 5, 20, Qt::AlignRight | Qt::AlignVCenter, QString::number(label));
        }
        painter->drawRect(rect.adjusted(0, 0, -1, -1));
    }

    void GraphWidget::draw_curves(QPainter *painter)
    {
        if(zoomStack.empty()){
            return;
        }

        QRect rect = get_paint_rect();

        if (!rect.isValid()){
            return;
        }

        painter->setClipRect(rect.adjusted(+1, +1, -1, -1));

        QPolygonF polyline;
        plot_settings settings = zoomStack[cur_zoom_];
        for(unsigned int i = 0; i < nodes_.size(); ++i){
            qreal dx = nodes_[i].first - settings.min_x_;
            qreal dy = nodes_[i].second - settings.min_y_;
            qreal x = rect.left() + (dx * (rect.width() - 1)
                                         / settings.span_x());
            qreal y = rect.bottom() - (dy * (rect.height() - 1)
                                           / settings.span_y());

            polyline.push_back(QPointF(x,y));
        }
        painter->setPen(QColor(line_color));
        painter->drawPolyline(polyline);
    }

    void GraphWidget::update_rubber_band_region()
    {
        QRect rect = rubber_band_rect_.normalized();
        update(rect.left(), rect.top(), rect.width(), 1);
        update(rect.left(), rect.top(), 1, rect.height());
        update(rect.left(), rect.bottom(), rect.width(), 1);
        update(rect.right(), rect.top(), 1, rect.height());
    }

    void GraphWidget::wheelEvent(QWheelEvent *event)
    {
        int numDegrees = event->delta() / 8;
        int numTicks = numDegrees / 15;
        if (event->orientation() == Qt::Horizontal){
            zoomStack[cur_zoom_].scroll(numTicks, 0);
        }
        else{
            zoomStack[cur_zoom_].scroll(0, numTicks);
        }
        update();
    }

    void GraphWidget::keyPressEvent(QKeyEvent *event)
    {
        switch (event->key())
        {
        case Qt::Key_Plus:
            zoom_in();
            break;
        case Qt::Key_Minus:
            zoom_out();
            break;
        case Qt::Key_Left:
            zoomStack[cur_zoom_].scroll(-1, 0);
            update();
            break;
        case Qt::Key_Right:
            zoomStack[cur_zoom_].scroll(+1, 0);
            update();
            break;
        case Qt::Key_Down:
            zoomStack[cur_zoom_].scroll(0, -1);
            update();
            break;
        case Qt::Key_Up:
            zoomStack[cur_zoom_].scroll(0, +1);
            update();
            break;
        default:
            break;
        }

        QWidget::keyPressEvent(event);
    }

    void GraphWidget::mouseReleaseEvent(QMouseEvent *event)
    {
        if(zoomStack.empty()){
            return;
        }

        if ((event->button() == Qt::LeftButton) && rubber_band_is_shown_){
            rubber_band_is_shown_ = false;
            update_rubber_band_region();
            unsetCursor();

            QRect rect = rubber_band_rect_.normalized();
            if (rect.width() > 4 && rect.height() > 4){
                rect.translate(-margin, -margin);

                plot_settings prevSettings = zoomStack[cur_zoom_];
                qreal dx = prevSettings.span_x() / qreal(width() - 2 * margin);
                qreal dy = prevSettings.span_y() / qreal(height() - 2 * margin);
                plot_settings settings = prevSettings.create_child(dx,dy,rect);
                if(plot_settings::is_valid_setting(settings)){
                    zoomStack.push_back(settings);
                    zoom_in();
                    update();
                }
            }
        }
    }

    void GraphWidget::mouseMoveEvent(QMouseEvent *event)
    {
        if (rubber_band_is_shown_){
            update_rubber_band_region();
            rubber_band_rect_.setBottomRight(event->pos());
            update_rubber_band_region();
        }

        //QWidget::mouseMoveEvent(event);
    }

    void GraphWidget::mousePressEvent(QMouseEvent *event)
    {
        if(zoomStack.empty()){
            return;
        }

        if (event->button() == Qt::LeftButton){
            if (get_paint_rect().contains(event->pos())){
                rubber_band_is_shown_ = true;
                rubber_band_rect_.setTopLeft(event->pos());
                rubber_band_rect_.setBottomRight(event->pos());
                update_rubber_band_region();
                setCursor(Qt::CrossCursor);
            }
        }
    }

    void GraphWidget::paintEvent(QPaintEvent* event)
    {
        if (rubber_band_is_shown_){
            QStylePainter spainter(this);
            spainter.setPen(QColor(rubber_color));
            spainter.drawRect(rubber_band_rect_.normalized().adjusted(0, 0, -1, -1));
        }

        QPainter painter(this);
        draw_grid(&painter);
        draw_curves(&painter);
    }

    QRect GraphWidget::get_paint_rect()const
    {
        return QRect(margin, margin, width() - 2 * margin, height() - 2 * margin);
    }

    QSize GraphWidget::sizeHint() const
    {
        return QSize(12 * margin, 8 * margin);
    }

    QSize GraphWidget::minimumSizeHint() const
    {
        return QSize(6 * margin, 4 * margin);
    }
}
