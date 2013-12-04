#include "gui/MainWidget.h"

#include <QTabBar>

#include "core/ServersManager.h"
#include "gui/MainTabBar.h"
#include "gui/GuiFactory.h"
#include "common/macros.h"
#include "gui/QueryWidget.h"

namespace fastoredis
{
    MainWidget::MainWidget(QWidget *parent) :
        base_class(parent)
    {
        MainTabBar *tab = new MainTabBar(this);
        VERIFY(connect(tab, SIGNAL(createdNewTab()), this, SLOT(createNewTab())));
        VERIFY(connect(tab, SIGNAL(reloadedTab()), this, SLOT(reloadeCurrentTab())));
        VERIFY(connect(tab, SIGNAL(duplicatedTab()), this, SLOT(duplicateCurrentTab())));
        VERIFY(connect(tab, SIGNAL(closedOtherTabs()), this, SLOT(closedOtherTabs())));
        VERIFY(connect(tab, SIGNAL(closedTab()), this, SLOT(closeCurrentTab())));
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
            QueryWidget *queryWidget = new QueryWidget(server);
            addWidgetToTab(queryWidget, server->name());
        }
    }

    QueryWidget *MainWidget::currentWidget() const
    {
        return qobject_cast<QueryWidget *>(base_class::currentWidget());
    }

    QueryWidget *MainWidget::widget(int index) const
    {
        return qobject_cast<QueryWidget *>(base_class::widget(index));
    }

    void MainWidget::closeTab(int index)
    {
        QueryWidget * shw = widget(index);
        if(shw){
            removeTab(index);
            delete shw;
        }
    }

    void MainWidget::createNewTab()
    {
        int curIndex = currentIndex();
        QueryWidget * shw = widget(curIndex);
        if(shw){
            openNewTab(shw, tabText(curIndex), QString());
        }
    }

    void MainWidget::duplicateCurrentTab()
    {
        int curIndex = currentIndex();
        QueryWidget * shw = widget(curIndex);
        if(shw){
           openNewTab(shw, tabText(curIndex), shw->inputText());
        }
    }

    void MainWidget::reloadeCurrentTab()
    {
        int curIndex = currentIndex();
        QueryWidget * shw = widget(curIndex);
        if(shw){
            shw->reload();
        }
    }

    void MainWidget::closeCurrentTab()
    {
        int curIndex = currentIndex();
        closeTab(curIndex);
    }

    void MainWidget::closedOtherTabs()
    {
        int curIndex = currentIndex();
        tabBar()->moveTab(curIndex, 0);
        while (count() > 1) {
            closeTab(1);
        }
    }

    void MainWidget::addWidgetToTab(QWidget *wid, const QString &title)
    {
        addTab(wid, GuiFactory::instance().redisConnectionIcon(), title);
        setCurrentWidget(wid);
    }

    void MainWidget::openNewTab(QueryWidget *src, const QString &title, const QString &text)
    {
        QueryWidget *newWid = QueryWidget::duplicate(src, text);
        DCHECK(newWid);
        addWidgetToTab(newWid, title);
    }
}

