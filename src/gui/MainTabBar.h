#pragma once

#include <QTabBar>
#include "global/global.h"

namespace fastoredis
{
    class MainTabBar : public QTabBar
    {
        Q_OBJECT

    public:
        typedef QTabBar base_class;
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
