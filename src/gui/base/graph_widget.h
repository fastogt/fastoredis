#pragma once

#include <vector>

#include <QWidget>

namespace fastoredis
{
    class plot_settings
    {
    public:
        plot_settings(qreal min_x, qreal max_x, qreal min_y, qreal max_y, unsigned num_x_ticks = 0,unsigned num_y_ticks = 0);

        void scroll(int dx, int dy);
        void adjust();
        qreal span_x() const { return max_x_ - min_x_; }
        qreal span_y() const { return max_y_ - min_y_; }
        qreal min_x_;
        qreal max_x_;
        qreal min_y_;
        qreal max_y_;

        unsigned num_x_ticks_;
        unsigned num_y_ticks_;

        plot_settings create_child(qreal dx,qreal dy,QRect rect)const;
        static bool is_valid_setting(const plot_settings &);
    private:
        static void adjust_axis(qreal &min, qreal &max, unsigned &numTicks);
    };

    class GraphWidget
            : public QWidget
    {
        Q_OBJECT
    public:
        typedef std::vector<std::pair<qreal, qreal> > nodes_container_type;
        GraphWidget(QWidget *parent = 0);
        QSize minimumSizeHint() const;
        QSize sizeHint() const;

        void setNodes(const nodes_container_type& nodes);

    public Q_SLOTS:
        void zoom_in();
        void zoom_out();

    protected:
        void paintEvent(QPaintEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void keyPressEvent(QKeyEvent *event);
        void wheelEvent(QWheelEvent *event);

    private:
        void update_rubber_band_region();
        void draw_grid(QPainter *painter);
        void draw_curves(QPainter *painter);
        QRect get_paint_rect()const;
        enum { margin = 50 };
        static const Qt::GlobalColor grid_color = Qt::black, rubber_color = Qt::blue, line_color = Qt::red;

        std::vector<plot_settings> zoomStack;
        unsigned cur_zoom_;
        bool rubber_band_is_shown_;
        QRect rubber_band_rect_;
        nodes_container_type nodes_;
    };
}
