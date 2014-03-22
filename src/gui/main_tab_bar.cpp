#include "gui/main_tab_bar.h"

#include <QMenu>
#include <QEvent>

#include "common/macros.h"

namespace fastoredis
{
    MainTabBar::MainTabBar(QWidget *parent)
        : QTabBar(parent)
    {
        _newShellAction = new QAction(this);
        _newShellAction->setShortcut(Qt::CTRL + Qt::Key_T);
        VERIFY(connect(_newShellAction, SIGNAL(triggered()), this , SIGNAL(createdNewTab())));

        _reloadShellAction = new QAction(this);
        _reloadShellAction->setShortcut(Qt::CTRL + Qt::Key_R);
        VERIFY(connect(_reloadShellAction, SIGNAL(triggered()), this , SIGNAL(reloadedTab())));

        _duplicateShellAction = new QAction(this);
        VERIFY(connect(_duplicateShellAction, SIGNAL(triggered()), this , SIGNAL(duplicatedTab())));

        _closeShellAction = new QAction(this);
        _closeShellAction->setShortcut(Qt::CTRL + Qt::Key_W);
        VERIFY(connect(_closeShellAction, SIGNAL(triggered()), this , SIGNAL(closedTab())));

        _closeOtherShellsAction = new QAction(this);
        VERIFY(connect(_closeOtherShellsAction, SIGNAL(triggered()), this , SIGNAL(closedOtherTabs())));

        setContextMenuPolicy(Qt::CustomContextMenu);
        VERIFY(connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint &))));

        retranslateUi();
    }

    void MainTabBar::showContextMenu(const QPoint &p)
    {
        QMenu menu(this);
        menu.addAction(_newShellAction);
        menu.addSeparator();
        menu.addAction(_reloadShellAction);
        menu.addAction(_duplicateShellAction);
        menu.addSeparator();
        menu.addAction(_closeShellAction);
        menu.addAction(_closeOtherShellsAction);
        menu.exec(mapToGlobal(p));
    }

    void MainTabBar::changeEvent(QEvent *e)
    {
        if(e->type() == QEvent::LanguageChange){
            retranslateUi();
        }
        QTabBar::changeEvent(e);
    }

    void MainTabBar::retranslateUi()
    {
        _newShellAction->setText(tr("&New Tab"));
        _reloadShellAction->setText(tr("&Reload"));
        _duplicateShellAction->setText(tr("&Duplicate"));
        _closeShellAction->setText(tr("&Close Tab"));
        _closeOtherShellsAction->setText(tr("Close &Other Tab"));
    }
}
