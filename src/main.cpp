#include <QApplication>
#include <QDesktopWidget>
#include <QTranslator>
#include "gui/MainWindow.h"
#include "common/events/static_events_bus.hpp"

void foo(){}
void foo1(int){}
void foo2(int,int){}
int main(int argc, char *argv[])
{
    typedef boost::mpl::vector<void(*)(),void(*)(int)> fus_t;
    common::events::static_event_bus<fus_t> b;
    common::events::IEvent *ev;
    common::events::IReceiver *rec;
    b.registerReciver(&foo,rec);
    //b.registerReciver(&foo2,rec);
    b.broadcast(&foo,ev);
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
