#include <QApplication>
#include <QDesktopWidget>

#include "gui/main_window.h"

namespace
{
    const QSize preferedSize = QSize(1024, 768);
}

int main(int argc, char *argv[])
{    
    QApplication app(argc, argv);
    app.setOrganizationName(PROJECT_COMPANYNAME);
    app.setApplicationName(PROJECT_NAME);

    fastoredis::MainWindow win;
#ifndef OS_ANDROID
    QRect screenGeometry = app.desktop()->availableGeometry();
    QSize screenSize(screenGeometry.width(), screenGeometry.height());
    QSize size(screenGeometry.width()/2, screenGeometry.height()/2);
    if(preferedSize.height() < screenSize.height() && preferedSize.width() < screenSize.width()){
        win.resize(preferedSize);
    }
    else{
        win.resize(size);
    }

    QPoint center = screenGeometry.center();
    win.move(center.x() - win.width() * 0.5, center.y() - win.height() * 0.5);
#endif

    win.show();
    return app.exec();
}
