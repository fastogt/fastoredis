#include "gui/MainWindow.h"

#include <QAction>
#include <QMenuBar>
#include <QToolButton>
#include <QStatusBar>
#include <QDockWidget>

#include "common/qt_helper/converter_patterns.h"
#include "gui/GuiFactory.h"
#include "gui/AppStyle.h"
#include "gui/LogWidget.h"
#include "gui/AboutDialog.h"
#include "gui/PreferencesDialog.h"

#include "core/SettingsManager.h"
#include "core/Logger.h"
#include "shell/ShellWidget.h"

namespace fastoredis
{
    MainWindow::MainWindow()
    : base_class()
    {
        using namespace common;
        unicode_string style = SettingsManager::instance().currentStyle();
        fastoredis::details::applyStyle(utils_qt::toQString(style));

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

        ShellWidget *shellW = new ShellWidget;
        setCentralWidget(shellW);

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
}

