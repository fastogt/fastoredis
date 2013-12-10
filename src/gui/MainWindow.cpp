#include "gui/MainWindow.h"

#include <QAction>
#include <QMenuBar>
#include <QToolButton>
#include <QStatusBar>
#include <QDockWidget>

#include "common/qt_helper/converter_patterns.h"
#include "gui/GuiFactory.h"
#include "gui/AppStyle.h"
#include "gui/widgets/LogWidget.h"
#include "gui/dialogs/AboutDialog.h"
#include "gui/dialogs/PreferencesDialog.h"
#include "gui/dialogs/ConnectionsDialog.h"
#include "gui/widgets/MainWidget.h"
#include "gui/explorer/ExplorerWidget.h"

#include "core/SettingsManager.h"
#include "core/Logger.h"

namespace fastoredis
{
    MainWindow::MainWindow()
    : base_class()
    {
        using namespace common;
        unicode_string style = SettingsManager::instance().currentStyle();
        fastoredis::detail::applyStyle(utils_qt::toQString(style));

        setWindowTitle(PROJECT_NAME_TITLE" "PROJECT_VERSION);
        setWindowIcon(GuiFactory::instance().mainWindowIcon());

        _openAction = new QAction(GuiFactory::instance().openIcon(), tr("&Open..."), this);
        _openAction->setShortcut(QKeySequence::Open);
        VERIFY(connect(_openAction, SIGNAL(triggered()), this, SLOT(open())));

        _saveAction = new QAction(tr("Save"), this);
        _saveAction->setShortcuts(QKeySequence::Save);
        VERIFY(connect(_saveAction, SIGNAL(triggered()), this, SLOT(save())));

        _saveAsAction = new QAction(tr("Save &As..."), this);
        _saveAsAction->setShortcuts(QKeySequence::SaveAs);
        VERIFY(connect(_saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs())));

        // Exit action
        QAction *exitAction = new QAction("&Exit", this);
        exitAction->setShortcut(QKeySequence::Quit);
        VERIFY(connect(exitAction, SIGNAL(triggered()), this, SLOT(close())));

        // File menu
        QMenu *fileMenu = menuBar()->addMenu("File");
        fileMenu->addAction(_openAction);
        fileMenu->addAction(_saveAction);
        fileMenu->addAction(_saveAsAction);
        fileMenu->addSeparator();
        fileMenu->addAction(exitAction);

        QAction *preferencesAction = new QAction("Preferences",this);
        VERIFY(connect(preferencesAction, SIGNAL(triggered()), this, SLOT(openPreferences())));

        QMenu *viewMenu = menuBar()->addMenu("View");

        QMenu *optionsMenu = menuBar()->addMenu("Options");
        optionsMenu->addAction(preferencesAction);

        QAction *aboutAction = new QAction("&About "PROJECT_NAME"...", this);
        VERIFY(connect(aboutAction, SIGNAL(triggered()), this, SLOT(about())));

        QMenu *helpMenu = menuBar()->addMenu("Help");
        helpMenu->addAction(aboutAction);

        MainWidget *mainW = new MainWidget;
        setCentralWidget(mainW);

        ExplorerWidget *exp = new ExplorerWidget(this);
        QDockWidget *expDock = new QDockWidget(tr("Explorer tree"));
        QAction *ac = expDock->toggleViewAction();
        ac->setText(QString("&Explorer tree"));
        ac->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
        ac->setChecked(false);
        viewMenu->addAction(ac);

        expDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
        expDock->setWidget(exp);
        expDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
        expDock->setVisible(false);
        addDockWidget(Qt::LeftDockWidgetArea, expDock);

        LogWidget *log = new LogWidget(this);
        VERIFY(connect(&Logger::instance(), SIGNAL(printed(const QString&, common::logging::LEVEL_LOG)), log, SLOT(addMessage(const QString&, common::logging::LEVEL_LOG))));
        QDockWidget *logDock = new QDockWidget(tr("Logs"));
        QAction *action = logDock->toggleViewAction();
        action->setText(QString("&Logs"));
        action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
        action->setChecked(false);
        viewMenu->addAction(action);

        logDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
        logDock->setWidget(log);
        logDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
        logDock->setVisible(false);
        addDockWidget(Qt::BottomDockWidgetArea, logDock);

        createStatusBar();
    }

    void MainWindow::createStatusBar()
    {
    }

    void MainWindow::open()
    {
        ConnectionsDialog dlg(this);
        int result = dlg.exec();
        if(result == QDialog::Accepted){
            MainWidget *wid = mainWidget();
            if(wid){
                wid->addWidgetBySetting(dlg.selectedConnection());
            }
        }
    }

    void MainWindow::save()
    {
    }

    void MainWindow::saveAs()
    {
    }

    void MainWindow::exit()
    {
    }

    void MainWindow::about()
    {
        AboutDialog dlg(this);
        dlg.exec();
    }

    void MainWindow::openPreferences()
    {
        PreferencesDialog dlg(this);
        dlg.exec();
    }

    MainWidget *const MainWindow::mainWidget() const
    {
        MainWidget *wid = qobject_cast<MainWidget*>(centralWidget());
        VERIFY(wid);
        return wid;
    }
}

