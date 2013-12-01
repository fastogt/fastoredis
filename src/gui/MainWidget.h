#pragma once

#include <QTabWidget>

#include "core/ConnectionSettings.h"

namespace fastoredis
{
    class ShellWidget;

    class MainWidget
            : public QTabWidget
    {
        Q_OBJECT
    public:
        typedef QTabWidget base_class;
        explicit MainWidget(QWidget *parent=0);
        void addWidgetBySetting(const IConnectionSettingsBasePtr &setting);
        ShellWidget *currentWidget() const;
        ShellWidget *widget(int index) const;
        void closeTab(int index);

    private Q_SLOTS:
        void createNewTab();
        void reloadeCurrentTab();
        void duplicateCurrentTab();
        void closeCurrentTab();
        void closedOtherTabs();

    private:
        void addWidgetToTab(QWidget *wid, const QString &title);
        void openNewTab(ShellWidget *src, const QString &title, const QString &text);
    };
}
