#include <QApplication>
#include <QDesktopWidget>

#include "gui/main_window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QRect screenGeometry = app.desktop()->availableGeometry();
    QSize size(screenGeometry.width()/2, screenGeometry.height()/2);

    fastoredis::MainWindow win;
    win.resize(size);

    QPoint center = screenGeometry.center();
    win.move(center.x() - win.width() * 0.5, center.y() - win.height() * 0.5);

    win.show();
    return app.exec();
}
