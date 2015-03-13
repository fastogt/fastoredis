#pragma once

#include <vector>

#include <QWidget>

namespace fastoredis
{
    struct plot_settings
    {
        plot_settings(qreal min_x, qreal max_x, qreal min_y, qreal max_y, unsigned num_x_ticks = 0,unsigned num_y_ticks = 0);

        void scroll(int dx, int dy);
        void adjust();
        qreal span_x() const;
        qreal span_y() const;
        plot_settings create_child(qreal dx,qreal dy,QRect rect) const;

        qreal min_x_;
        qreal max_x_;
        qreal min_y_;
        qreal max_y_;

        unsigned num_x_ticks_;
        unsigned num_y_ticks_;        
    };

    class GraphWidget
            : public QWidget
    {
        Q_OBJECT
    public:
        enum
        {
            min_height = 480,
            min_width = 640
        };

        typedef std::vector<std::pair<qreal, qreal> > nodes_container_type;
        GraphWidget(QWidget* parent = 0);

        void setNodes(const nodes_container_type& nodes);

    public Q_SLOTS:
        void zoom_in();
        void zoom_out();

    protected:
        void paintEvent(QPaintEvent* event);
        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void mouseReleaseEvent(QMouseEvent* event);
        void keyPressEvent(QKeyEvent* event);
        void wheelEvent(QWheelEvent* event);

    private:
        void updateRubberBandRegion();
        void drawGrid(QPainter* painter);
        void drawCurves(QPainter* painter);
        QRect paintRect()const;

        enum
        {
            margin = 100
        };

        static const Qt::GlobalColor grid_color = Qt::black, rubber_color = Qt::blue, line_color = Qt::red;

        std::vector<plot_settings> zoomStack_;
        unsigned curZoom_;
        bool rubberBandIsShown_;
        QRect rubberBandRect_;
        nodes_container_type nodes_;
    };
}
