#pragma once

#include <QTabWidget>

#include "core/ConnectionSettings.h"

namespace fastoredis
{
    class QueryWidget;

    class MainWidget
            : public QTabWidget
    {
        Q_OBJECT
    public:
        typedef QTabWidget base_class;
        explicit MainWidget(QWidget *parent=0);
        void addWidgetBySetting(const IConnectionSettingsBasePtr &setting);
        QueryWidget *currentWidget() const;
        QueryWidget *widget(int index) const;
        void closeTab(int index);        

    private Q_SLOTS:
        void createNewTab();
        void reloadeCurrentTab();
        void duplicateCurrentTab();
        void closeCurrentTab();
        void closedOtherTabs();

    private:
        void addWidgetToTab(QWidget *wid, const QString &title);
        void openNewTab(QueryWidget *src, const QString &title, const QString &text);
    };
}
