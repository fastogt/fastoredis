#pragma once

#include <QPixmap>
#include <QWidget>
#include <vector>

namespace fastoredis
{
    class plot_settings
    {
    public:
        plot_settings(float min_x, float max_x, float min_y, float max_y, unsigned num_x_ticks = 0,unsigned num_y_ticks = 0);

        void scroll(int dx, int dy);
        void adjust();
        float span_x() const { return max_x_ - min_x_; }
        float span_y() const { return max_y_ - min_y_; }
        float min_x_;
        float max_x_;
        float min_y_;
        float max_y_;

        unsigned num_x_ticks_;
        unsigned num_y_ticks_;

        plot_settings create_child(float dx,float dy,QRect rect)const;
        static bool is_valid_setting(const plot_settings &);
    private:
        static void adjust_axis(float &min, float &max, unsigned &numTicks);
    };

    class GraphWidget
            : public QWidget
    {
        Q_OBJECT
    public:
        typedef std::vector<std::pair<long, long> > nodes_container_type;
        GraphWidget(const nodes_container_type& nodes, QWidget *parent = 0);
        QSize minimumSizeHint() const;
        QSize sizeHint() const;

        void setNodes(const nodes_container_type& nodes);

    public Q_SLOTS:
        void zoom_in();
        void zoom_out();

    protected:
        void paintEvent(QPaintEvent *event);
        void resizeEvent(QResizeEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void keyPressEvent(QKeyEvent *event);
        void wheelEvent(QWheelEvent *event);

    private:
        void update_rubber_band_region();
        void refresh_pixmap();
        void draw_grid(QPainter *painter);
        void draw_curves(QPainter *painter);
        QRect get_paint_rect()const;
        enum { margin = 50 };
        static const Qt::GlobalColor grid_color = Qt::black, rubber_color = Qt::red, line_color = Qt::red;

        std::vector<plot_settings> zoomStack;
        unsigned cur_zoom_;
        bool rubber_band_is_shown_;
        QRect rubber_band_rect_;
        QPixmap pixmap;
        nodes_container_type nodes_;
    };

}
