#pragma once

#include <QTabBar>

namespace fastoredis
{
    class MainTabBar : public QTabBar
    {
        Q_OBJECT

    public:
        explicit MainTabBar(QWidget* parent=0);

    Q_SIGNALS:
        void createdNewTab();
        void reloadedTab();
        void duplicatedTab();
        void closedTab();
        void closedOtherTabs();

    private Q_SLOTS:
        void showContextMenu(const QPoint &p);        

    private:
        QAction *_newShellAction;
        QAction *_reloadShellAction;
        QAction *_duplicateShellAction;
        QAction *_closeShellAction;
        QAction *_closeOtherShellsAction;
    };
}
