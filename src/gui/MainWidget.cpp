#include "gui/MainWidget.h"

#include <QTabBar>
#include "core/ServersManager.h"
#include "shell/ShellWidget.h"

namespace fastoredis
{
    MainWidget::MainWidget(QWidget *parent) :
        base_class(parent)
    {
        QTabBar *tab = new QTabBar(this);

        setTabBar(tab);
        setTabsClosable(true);
        setElideMode(Qt::ElideRight);
        setMovable(true);
        setDocumentMode(true);
    }

    void MainWidget::addWidgetBySetting(const IConnectionSettingsBasePtr &setting)
    {
        IServerPtr server = ServersManager::instance().createServer(setting);
        if(server){
            ShellWidget *queryWidget = new ShellWidget(server,this);
            addTab(queryWidget, server->name());
            setCurrentWidget(queryWidget);
        }
    }
}

