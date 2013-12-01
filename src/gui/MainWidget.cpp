#include "gui/MainWidget.h"

#include <QTabBar>

#include "core/ServersManager.h"
#include "shell/ShellWidget.h"
#include "gui/MainTabBar.h"
#include "gui/GuiFactory.h"
#include "common/macros.h"

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
            ShellWidget *queryWidget = new ShellWidget(server,this);
            addWidgetToTab(queryWidget, server->name());
        }        
    }

    ShellWidget *MainWidget::currentWidget() const
    {
        return qobject_cast<ShellWidget *>(base_class::currentWidget());
    }

    ShellWidget *MainWidget::widget(int index) const
    {
        return qobject_cast<ShellWidget *>(base_class::widget(index));
    }

    void MainWidget::closeTab(int index)
    {
        ShellWidget * shw = widget(index);
        if(shw){
            removeTab(index);
            delete shw;
        }
    }

    void MainWidget::createNewTab()
    {
        int curIndex = currentIndex();
        ShellWidget * shw = widget(curIndex);
        if(shw){
            openNewTab(shw, tabText(curIndex), QString());
        }
    }

    void MainWidget::duplicateCurrentTab()
    {
        int curIndex = currentIndex();
        ShellWidget * shw = widget(curIndex);
        if(shw){
            openNewTab(shw, tabText(curIndex), shw->text());
        }
    }

    void MainWidget::reloadeCurrentTab()
    {
        int curIndex = currentIndex();
        ShellWidget * shw = widget(curIndex);
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

    void MainWidget::openNewTab(ShellWidget *src, const QString &title, const QString &text)
    {
        ShellWidget *newWid = ShellWidget::duplicate(src, text);
        DCHECK(newWid);
        addWidgetToTab(newWid, title);
    }
}

