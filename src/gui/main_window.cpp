#include "gui/main_window.h"

#include <QAction>
#include <QMenuBar>
#include <QDockWidget>
#include <QMessageBox>

#include "common/net/socket_tcp.h"
#include "common/qt/convert_string.h"

#include "gui/gui_factory.h"
#include "gui/app_style.h"
#include "gui/dialogs/about_dialog.h"
#include "gui/dialogs/preferences_dialog.h"
#include "gui/dialogs/connections_dialog.h"
#include "gui/widgets/log_tab_widget.h"
#include "gui/widgets/main_widget.h"
#include "gui/explorer/explorer_tree_view.h"

#include "core/servers_manager.h"
#include "core/settings_manager.h"
#include "core/logger.h"
#include "core/command_logger.h"

#include "server_config_daemon/server_config.h"

#include "translations/translations.h"

namespace fastoredis
{
    MainWindow::MainWindow()
        : QMainWindow()
    {
        using namespace common;
        std::string lang = SettingsManager::instance().currentLanguage();
        QString newLang = fastoredis::translations::applyLanguage(convertFromString<QString>(lang));
        SettingsManager::instance().setCurrentLanguage(convertToString(newLang));

        std::string style = SettingsManager::instance().currentStyle();
        fastoredis::applyStyle(convertFromString<QString>(style));

        setWindowTitle(PROJECT_NAME_TITLE" "PROJECT_VERSION);
        setWindowIcon(GuiFactory::instance().mainWindowIcon());

        openAction_ = new QAction(this);
        openAction_->setIcon(GuiFactory::instance().openIcon());
        openAction_->setShortcut(QKeySequence::Open);
        VERIFY(connect(openAction_, SIGNAL(triggered()), this, SLOT(open())));

        saveAction_ = new QAction(this);
        saveAction_->setShortcuts(QKeySequence::Save);
        VERIFY(connect(saveAction_, SIGNAL(triggered()), this, SLOT(save())));

        saveAsAction_ = new QAction(this);
        saveAsAction_->setShortcuts(QKeySequence::SaveAs);
        VERIFY(connect(saveAsAction_, SIGNAL(triggered()), this, SLOT(saveAs())));

        // Exit action
        exitAction_ = new QAction(this);
        exitAction_->setShortcut(QKeySequence::Quit);
        VERIFY(connect(exitAction_, SIGNAL(triggered()), this, SLOT(close())));

        // File menu
        QMenu *fileMenu = new QMenu(this);
        fileAction_ = menuBar()->addMenu(fileMenu);
        fileMenu->addAction(openAction_);
        fileMenu->addAction(saveAction_);
        fileMenu->addAction(saveAsAction_);
        fileMenu->addSeparator();
        fileMenu->addAction(exitAction_);

        preferencesAction_ = new QAction(this);
        VERIFY(connect(preferencesAction_, SIGNAL(triggered()), this, SLOT(openPreferences())));

        QMenu *viewMenu = new QMenu(this);
        viewAction_ = menuBar()->addMenu(viewMenu);

        QMenu *optionsMenu = new QMenu(this);
        optionsAction_ = menuBar()->addMenu(optionsMenu);

        checkUpdateAction_ = new QAction(this);
        VERIFY(connect(checkUpdateAction_, SIGNAL(triggered()), this, SLOT(checkUpdate())));

        optionsMenu->addAction(checkUpdateAction_);
        optionsMenu->addAction(preferencesAction_);

        aboutAction_ = new QAction(this);
        VERIFY(connect(aboutAction_, SIGNAL(triggered()), this, SLOT(about())));

        QMenu *helpMenu = new QMenu(this);
        helpAction_ = menuBar()->addMenu(helpMenu);
        helpMenu->addAction(aboutAction_);

        MainWidget *mainW = new MainWidget;
        setCentralWidget(mainW);

        exp_ = new ExplorerTreeView(this);
        VERIFY(connect(exp_, SIGNAL(openedConsole(IServerPtr)), mainW, SLOT(openConsole(IServerPtr))));
        expDock_ = new QDockWidget(this);
        explorerAction_ = expDock_->toggleViewAction();
        explorerAction_->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
        explorerAction_->setChecked(true);
        viewMenu->addAction(explorerAction_);

        expDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
        expDock_->setWidget(exp_);
        expDock_->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
        expDock_->setVisible(true);
        addDockWidget(Qt::LeftDockWidgetArea, expDock_);

        LogTabWidget *log = new LogTabWidget(this);
        VERIFY(connect(&Logger::instance(), SIGNAL(printed(const QString&, common::logging::LEVEL_LOG)), log, SLOT(addLogMessage(const QString&, common::logging::LEVEL_LOG))));
        VERIFY(connect(&CommandLogger::instance(), SIGNAL(printed(const Command&)), log, SLOT(addCommand(const Command&))));
        logDock_ = new QDockWidget(this);
        logsAction_ = logDock_->toggleViewAction();
        logsAction_->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
        logsAction_->setChecked(false);
        viewMenu->addAction(logsAction_);

        logDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
        logDock_->setWidget(log);
        logDock_->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
        logDock_->setVisible(false);
        addDockWidget(Qt::BottomDockWidgetArea, logDock_);

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
            exp_->addServer(server);
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
        openAction_->setText(tr("&Open..."));
        saveAction_->setText(tr("Save"));
        saveAsAction_->setText(tr("Save &As..."));
        exitAction_->setText(tr("&Exit"));
        fileAction_->setText(tr("File"));
        preferencesAction_->setText(tr("Preferences"));
        checkUpdateAction_->setText(tr("Check updates"));
        viewAction_->setText(tr("View"));
        optionsAction_->setText(tr("Options"));
        aboutAction_->setText(tr("&About %1...").arg(PROJECT_NAME));
        helpAction_->setText(tr("Help"));
        const QString &exp = tr("Explorer tree");
        const QString &log = tr("Logs");
        explorerAction_->setText(exp);
        logsAction_->setText(log);
        expDock_->setWindowTitle(exp);
        logDock_->setWindowTitle(log);
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

    void MainWindow::checkUpdate()
    {
        common::net::SocketTcp s(SITE_URL, SERV_PORT);
        bool res = s.connect();
        if(!res){
            return;
        }

        res = s.write(common::convertFromString<common::buffer_type>(GET_VERSION));
        if(!res){
            s.close();
            return;
        }

        common::buffer_type version;
        res = s.read(version, 128);
        if(res){
            std::string sversion = common::convertToString(version);
            QMessageBox::information(this, QString("version"),
                QObject::tr(PROJECT_NAME" version: %1.")
                    .arg(common::convertFromString<QString>(sversion)));
        }

        s.close();
        return;
    }

    MainWidget *const MainWindow::mainWidget() const
    {
        MainWidget *wid = qobject_cast<MainWidget*>(centralWidget());
        VERIFY(wid);
        return wid;
    }
}

