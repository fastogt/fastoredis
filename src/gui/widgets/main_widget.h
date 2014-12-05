#pragma once

/**/

#include <QTabWidget>

#include "core/iserver.h"

namespace fastoredis
{
    class QueryWidget;

    class MainWidget
            : public QTabWidget
    {
        Q_OBJECT
    public:
        explicit MainWidget(QWidget *parent = 0);
        void addWidgetBySetting(const IConnectionSettingsBasePtr& setting);
        QueryWidget* currentWidget() const;
        QueryWidget* widget(int index) const;

        void nextTab();
        void previousTab();

    public Q_SLOTS:
        void openConsole(IServerPtr server);

    private Q_SLOTS:
        void createNewTab();
        void reloadeCurrentTab();
        void duplicateCurrentTab();
        void closeTab(int index);
        void closeCurrentTab();
        void closedOtherTabs();

    protected:
        virtual void keyPressEvent(QKeyEvent* e);

    private:
        void addWidgetToTab(QWidget* wid, const QString& title);
        void openNewTab(QueryWidget* src, const QString& title, const QString& text);
    };
}
