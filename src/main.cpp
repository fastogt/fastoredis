#include <QApplication>
#include <QDesktopWidget>

#include "gui/main_window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QRect screenGeometry = QApplication::desktop()->availableGeometry();
    QSize size(screenGeometry.width() - 450, screenGeometry.height() - 165);

    fastoredis::MainWindow win;
    win.resize(size);
    int x = (screenGeometry.width() - win.width()) / 2;
    int y = (screenGeometry.height() - win.height()) / 2;
    win.move(x, y);
    win.show();
    return app.exec();
}
