#include "gui/MainWindow.h"

#include <QAction>
#include <QMenuBar>
#include <QToolButton>
#include <QStatusBar>
#include <QDockWidget>

#include "common/qt/converter_patterns.h"
#include "gui/GuiFactory.h"
#include "gui/AppStyle.h"
#include "gui/widgets/LogTabWidget.h"
#include "gui/dialogs/AboutDialog.h"
#include "gui/dialogs/PreferencesDialog.h"
#include "gui/dialogs/ConnectionsDialog.h"
#include "gui/widgets/MainWidget.h"
#include "gui/explorer/ExplorerTreeView.h"

#include "core/ServersManager.h"
#include "core/SettingsManager.h"
#include "core/Logger.h"

#include "translations/Translations.h"

namespace fastoredis
{
    MainWindow::MainWindow()
        : QMainWindow()
    {
        using namespace common;
        unicode_string lang = SettingsManager::instance().currentLanguage();
        QString newLang = fastoredis::translations::applyLanguage(utils_qt::toQString(lang));
        SettingsManager::instance().setCurrentLanguage(common::utils_qt::toStdString(newLang));

        unicode_string style = SettingsManager::instance().currentStyle();
        fastoredis::applyStyle(utils_qt::toQString(style));

        setWindowTitle(PROJECT_NAME_TITLE" "PROJECT_VERSION);
        setWindowIcon(GuiFactory::instance().mainWindowIcon());

        _openAction = new QAction(this);
        _openAction->setIcon(GuiFactory::instance().openIcon());
        _openAction->setShortcut(QKeySequence::Open);
        VERIFY(connect(_openAction, SIGNAL(triggered()), this, SLOT(open())));

        _saveAction = new QAction(this);
        _saveAction->setShortcuts(QKeySequence::Save);
        VERIFY(connect(_saveAction, SIGNAL(triggered()), this, SLOT(save())));

        _saveAsAction = new QAction(this);
        _saveAsAction->setShortcuts(QKeySequence::SaveAs);
        VERIFY(connect(_saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs())));

        // Exit action
        _exitAction = new QAction(this);
        _exitAction->setShortcut(QKeySequence::Quit);
        VERIFY(connect(_exitAction, SIGNAL(triggered()), this, SLOT(close())));

        // File menu
        QMenu *fileMenu = new QMenu(this);
        _fileAction = menuBar()->addMenu(fileMenu);
        fileMenu->addAction(_openAction);
        fileMenu->addAction(_saveAction);
        fileMenu->addAction(_saveAsAction);
        fileMenu->addSeparator();
        fileMenu->addAction(_exitAction);

        _preferencesAction = new QAction(this);
        VERIFY(connect(_preferencesAction, SIGNAL(triggered()), this, SLOT(openPreferences())));

        QMenu *viewMenu = new QMenu(this);
        _viewAction = menuBar()->addMenu(viewMenu);

        QMenu *optionsMenu = new QMenu(this);
        _optionsAction = menuBar()->addMenu(optionsMenu);
        optionsMenu->addAction(_preferencesAction);

        _aboutAction = new QAction(this);
        VERIFY(connect(_aboutAction, SIGNAL(triggered()), this, SLOT(about())));

        QMenu *helpMenu = new QMenu(this);
        _helpAction = menuBar()->addMenu(helpMenu);
        helpMenu->addAction(_aboutAction);

        MainWidget *mainW = new MainWidget;
        setCentralWidget(mainW);

        _exp = new ExplorerTreeView(this);
        VERIFY(connect(_exp, SIGNAL(openedConsole(const IServerPtr &)), mainW, SLOT(openConsole(const IServerPtr &))));
        _expDock = new QDockWidget(this);
        _explorerAction = _expDock->toggleViewAction();
        _explorerAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
        _explorerAction->setChecked(true);
        viewMenu->addAction(_explorerAction);

        _expDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
        _expDock->setWidget(_exp);
        _expDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
        _expDock->setVisible(true);
        addDockWidget(Qt::LeftDockWidgetArea, _expDock);

        LogTabWidget *log = new LogTabWidget(this);
        VERIFY(connect(&Logger::instance(), SIGNAL(printed(const QString&, common::logging::LEVEL_LOG)), log, SLOT(addLogMessage(const QString&, common::logging::LEVEL_LOG))));
        _logDock = new QDockWidget(this);
        _logsAction = _logDock->toggleViewAction();
        _logsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
        _logsAction->setChecked(false);
        viewMenu->addAction(_logsAction);

        _logDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
        _logDock->setWidget(log);
        _logDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
        _logDock->setVisible(false);
        addDockWidget(Qt::BottomDockWidgetArea, _logDock);

        createStatusBar();
        retranslateUi();
    }

    void MainWindow::createStatusBar()
    {
    }

    void MainWindow::open()
    {
        ConnectionsDialog dlg(this);
        int result = dlg.exec();
        if(result == QDialog::Accepted){
            IServerPtr server = ServersManager::instance().createServer(dlg.selectedConnection());
            _exp->addServer(server);
        }
    }

    void MainWindow::changeEvent(QEvent *e)
    {
        if(e->type() == QEvent::LanguageChange){
            retranslateUi();
        }
        QMainWindow::changeEvent(e);
    }

    void MainWindow::retranslateUi()
    {
        _openAction->setText(tr("&Open..."));
        _saveAction->setText(tr("Save"));
        _saveAsAction->setText(tr("Save &As..."));
        _exitAction->setText(tr("&Exit"));
        _fileAction->setText(tr("File"));
        _preferencesAction->setText(tr("Preferences"));
        _viewAction->setText(tr("View"));
        _optionsAction->setText(tr("Options"));
        _aboutAction->setText(tr("&About %1...").arg(PROJECT_NAME));
        _helpAction->setText(tr("Help"));
        const QString &exp = tr("Explorer tree");
        const QString &log = tr("Logs");
        _explorerAction->setText(exp);
        _logsAction->setText(log);
        _expDock->setWindowTitle(exp);
        _logDock->setWindowTitle(log);
    }

    void MainWindow::save()
    {
    }

    void MainWindow::saveAs()
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

