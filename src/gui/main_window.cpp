#include "gui/main_window.h"

#include <QAction>
#include <QMenuBar>
#include <QDockWidget>
#include <QMessageBox>
#include <QThread>

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
#include "translations/global.h"

namespace
{
    bool isNeededUpdate(const std::string& serverVersion)
    {
        if(serverVersion.empty()){
            return false;
        }

        std::string curVer;
        int pos = 0;
        uint16_t serMaj = 0;
        uint16_t serMin = 0;
        uint16_t serPatch = 0;

        for(int i = 0; i < serverVersion.length(); ++i){
            char ch = serverVersion[i];
            if(ch == '.'){
                if(pos == 0){
                    serMaj = common::convertFromString<uint16_t>(curVer);
                }
                else if(pos == 1){
                    serMin = common::convertFromString<uint16_t>(curVer);
                }
                else if(pos == 2){
                    serPatch = common::convertFromString<uint16_t>(curVer);
                }

                ++pos;
                curVer.clear();
            }
            else{
                curVer += ch;
            }
        }

        if(pos != 3){
            return false;
        }

        if(PROJECT_VERSION_MAJOR < serMaj){
            return true;
        }

        if(PROJECT_VERSION_MINOR < serMin){
            return true;
        }

        return PROJECT_VERSION_PATCH < serPatch;
    }

    const QString checkVersion = QObject::tr("Check version");
    const QString connectionErrorText = QObject::tr("Connection error!");
}

namespace fastoredis
{

    UpdateChecker::UpdateChecker(QObject *parent)
        : QObject(parent)
    {

    }

    void UpdateChecker::routine()
    {
        common::net::SocketTcp s(SITE_URL, SERV_PORT);
        bool res = s.connect();
        if(!res){
            emit versionAvailibled(res, QString());
            return;
        }

        res = s.write(common::convertFromString<common::buffer_type>(GET_VERSION));
        if(!res){
            emit versionAvailibled(res, QString());
            s.close();
            return;
        }

        common::buffer_type version;
        res = s.read(version, 128);

        QString vers = common::convertFromString<QString>(common::convertToString(version));
        emit versionAvailibled(res, vers);

        s.close();
        return;
    }

    MainWindow::MainWindow()
        : QMainWindow(), isCheckedInSession_(false)
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

    void MainWindow::showEvent(QShowEvent* e)
    {
        QMainWindow::showEvent(e);
        bool isA = SettingsManager::instance().autoCheckUpdates();
        if(isA && !isCheckedInSession_){
            isCheckedInSession_ = true;
            checkUpdate();
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
        using namespace translations;
        openAction_->setText(trOpen);
        saveAction_->setText(trSave);
        saveAsAction_->setText(trSaveAs);
        exitAction_->setText(trExit);
        fileAction_->setText(trFile);
        preferencesAction_->setText(trPreferences);
        checkUpdateAction_->setText(trCheckUpdate);
        viewAction_->setText(trView);
        optionsAction_->setText(trOptions);
        aboutAction_->setText(tr("About %1...").arg(PROJECT_NAME));
        helpAction_->setText(trHelp);
        explorerAction_->setText(trExpTree);
        logsAction_->setText(trLogs);
        expDock_->setWindowTitle(trExpTree);
        logDock_->setWindowTitle(trLogs);
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
        QThread* th = new QThread;
        UpdateChecker* cheker = new UpdateChecker;
        cheker->moveToThread(th);
        VERIFY(connect(th, SIGNAL(started()), cheker, SLOT(routine())));
        VERIFY(connect(cheker, SIGNAL(versionAvailibled(bool, const QString&)), this, SLOT(versionAvailible(bool, const QString&))));
        VERIFY(connect(cheker, SIGNAL(versionAvailibled(bool, const QString&)), th, SLOT(quit())));
        VERIFY(connect(th, SIGNAL(finished()), cheker, SLOT(deleteLater())));
        VERIFY(connect(th, SIGNAL(finished()), th, SLOT(deleteLater())));
        th->start();
    }

    void MainWindow::versionAvailible(bool succesResult, const QString& version)
    {
        if(!succesResult){
            QMessageBox::information(this, checkVersion, connectionErrorText);
            checkUpdateAction_->setEnabled(true);
        }
        else{
            bool isn = isNeededUpdate(common::convertToString(version));
            if(isn){
                QMessageBox::information(this, checkVersion,
                    QObject::tr("Availible new version: %1")
                        .arg(version));
            }
            else{
                QMessageBox::information(this, checkVersion,
                    QObject::tr("<h3>You're' up-to-date!</h3>"
                                PROJECT_NAME" %1 is currently the newest version available.")
                        .arg(version));
            }

            checkUpdateAction_->setEnabled(isn);
        }
    }

    MainWidget *const MainWindow::mainWidget() const
    {
        MainWidget *wid = qobject_cast<MainWidget*>(centralWidget());
        VERIFY(wid);
        return wid;
    }
}

