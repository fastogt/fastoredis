#include "gui/main_tab_bar.h"

#include <QMenu>
#include <QEvent>

#include "common/macros.h"

#include "translations/global.h"

namespace fastoredis
{
    MainTabBar::MainTabBar(QWidget* parent)
        : QTabBar(parent)
    {
        newShellAction_ = new QAction(this);
        newShellAction_->setShortcut(Qt::CTRL + Qt::Key_T);
        VERIFY(connect(newShellAction_, SIGNAL(triggered()), this , SIGNAL(createdNewTab())));

        reloadShellAction_ = new QAction(this);
        reloadShellAction_->setShortcut(Qt::CTRL + Qt::Key_R);
        VERIFY(connect(reloadShellAction_, SIGNAL(triggered()), this , SIGNAL(reloadedTab())));

        duplicateShellAction_ = new QAction(this);
        VERIFY(connect(duplicateShellAction_, SIGNAL(triggered()), this , SIGNAL(duplicatedTab())));

        closeShellAction_ = new QAction(this);
        closeShellAction_->setShortcut(Qt::CTRL + Qt::Key_W);
        VERIFY(connect(closeShellAction_, SIGNAL(triggered()), this , SIGNAL(closedTab())));

        closeOtherShellsAction_ = new QAction(this);
        VERIFY(connect(closeOtherShellsAction_, SIGNAL(triggered()), this , SIGNAL(closedOtherTabs())));

        setContextMenuPolicy(Qt::CustomContextMenu);
        VERIFY(connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint &))));

        retranslateUi();
    }

    void MainTabBar::showContextMenu(const QPoint& p)
    {
        QMenu menu(this);
        menu.addAction(newShellAction_);
        menu.addSeparator();
        menu.addAction(reloadShellAction_);
        menu.addAction(duplicateShellAction_);
        menu.addSeparator();
        menu.addAction(closeShellAction_);
        menu.addAction(closeOtherShellsAction_);
        menu.exec(mapToGlobal(p));
    }

    void MainTabBar::changeEvent(QEvent* e)
    {
        if(e->type() == QEvent::LanguageChange){
            retranslateUi();
        }

        QTabBar::changeEvent(e);
    }

    void MainTabBar::retranslateUi()
    {
        using namespace translations;
        newShellAction_->setText(trNewTab);
        reloadShellAction_->setText(trReload);
        duplicateShellAction_->setText(trDuplicate);
        closeShellAction_->setText(trCloseTab);
        closeOtherShellsAction_->setText(trCloseOtherTab);
    }
}
