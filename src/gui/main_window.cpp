#include "gui/main_window.h"

#include <QAction>
#include <QMenuBar>
#include <QDockWidget>
#include <QMessageBox>
#include <QThread>
#include <QApplication>

#ifdef OS_ANDROID
//http://www.math.spbu.ru/user/gran/Atom21/AtomBook_21_lab6.pdf
//http://www.linux.org.ru/forum/development/11000751
//http://doc.qt.io/qt-5/qtouchevent.html
//http://www.math.spbu.ru/user/gran/AtomMG2/lec_04.pdf
//http://qt-project.org/forums/viewthread/7253
//http://blog.sbw.so/Article/index/title/QT%20for%20Android%20%E6%8D%95%E8%8E%B7%E5%A4%9A%E7%82%B9%E8%A7%A6%E6%8E%A7%E7%9A%84%E7%A4%BA%E4%BE%8B.html
//http://developer.nokia.com/community/wiki/Enabling_pinch_zooming_in_QGraphicsWebView_with_QPinchGesture
//http://habrahabr.ru/post/134289/
#include <QGestureEvent>
#endif

#include "common/net/socket_tcp.h"
#include "common/qt/convert_string.h"

#include "gui/shortcuts.h"
#include "gui/gui_factory.h"
#include "gui/app_style.h"
#include "gui/dialogs/about_dialog.h"
#include "gui/dialogs/preferences_dialog.h"
#include "gui/dialogs/connections_dialog.h"
#include "gui/widgets/log_tab_widget.h"
#include "gui/widgets/main_widget.h"
#include "gui/explorer/explorer_tree_view.h"
#include "gui/dialogs/python_console_dialog.h"
#include "gui/dialogs/lua_console_dialog.h"
#include "gui/dialogs/encode_decode_dialog.h"

#include "core/servers_manager.h"
#include "core/settings_manager.h"
#include "core/logger.h"
#include "core/command_logger.h"

#include "server_config_daemon/server_config.h"

#include "translations/translations.h"
#include "translations/global.h"

namespace
{
    bool isNeededUpdate(const QString& serverVersion)
    {
        if(serverVersion.isEmpty()){
            return false;
        }

        QString curVer;
        int pos = 0;
        uint serMaj = 0;
        uint serMin = 0;
        uint serPatch = 0;

        for(int i = 0; i < serverVersion.length(); ++i){
            QChar ch = serverVersion[i];
            if(ch == '.'){
                if(pos == 0){
                    serMaj = curVer.toUInt();
                }
                else if(pos == 1){
                    serMin = curVer.toUInt();
                }
                else if(pos == 2){
                    serPatch = curVer.toUInt();
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

    const QKeySequence logsKeySequence = Qt::CTRL + Qt::Key_L;
    const QKeySequence explorerKeySequence = Qt::CTRL + Qt::Key_T;
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
#ifdef OS_ANDROID
        setAttribute(Qt::WA_AcceptTouchEvents);
        //setAttribute(Qt::WA_StaticContents);

        //grabGesture(Qt::TapGesture); //click
        grabGesture(Qt::TapAndHoldGesture); //long tap

        //grabGesture(Qt::SwipeGesture); //swipe
        //grabGesture(Qt::PanGesture); // drag and drop
        //grabGesture(Qt::PinchGesture); //zoom
#endif
        using namespace common;
        QString lang = SettingsManager::instance().currentLanguage();
        QString newLang = fastoredis::translations::applyLanguage(lang);
        SettingsManager::instance().setCurrentLanguage(newLang);

        QString style = SettingsManager::instance().currentStyle();
        fastoredis::applyStyle(style);

        setWindowTitle(PROJECT_NAME_TITLE " " PROJECT_VERSION);
        setWindowIcon(GuiFactory::instance().mainWindowIcon());

        openAction_ = new QAction(this);
        openAction_->setIcon(GuiFactory::instance().openIcon());
        openAction_->setShortcut(openKey);
        VERIFY(connect(openAction_, &QAction::triggered, this, &MainWindow::open));

        // Exit action
        exitAction_ = new QAction(this);
        exitAction_->setShortcut(quitKey);
        VERIFY(connect(exitAction_, SIGNAL(triggered()), this, SLOT(close())));

        // File menu
        QMenu *fileMenu = new QMenu(this);
        fileAction_ = menuBar()->addMenu(fileMenu);
        fileMenu->addAction(openAction_);
        fileMenu->addSeparator();
        fileMenu->addAction(exitAction_);

        preferencesAction_ = new QAction(this);
        preferencesAction_->setIcon(GuiFactory::instance().preferencesIcon());
        VERIFY(connect(preferencesAction_, SIGNAL(triggered()), this, SLOT(openPreferences())));

        QMenu *optionsMenu = new QMenu(this);
        optionsAction_ = menuBar()->addMenu(optionsMenu);

        checkUpdateAction_ = new QAction(this);
        VERIFY(connect(checkUpdateAction_, SIGNAL(triggered()), this, SLOT(checkUpdate())));

        optionsMenu->addAction(checkUpdateAction_);
        optionsMenu->addAction(preferencesAction_);

        //tools menu
        QMenu *tools = new QMenu(this);
        toolsAction_ = menuBar()->addMenu(tools);
        pythonConsoleAction_ = new QAction(this);
        pythonConsoleAction_->setIcon(GuiFactory::instance().pythonIcon());
        VERIFY(connect(pythonConsoleAction_, SIGNAL(triggered()), this, SLOT(openPythonConsole())));
        tools->addAction(pythonConsoleAction_);

        luaConsoleAction_ = new QAction(this);
        luaConsoleAction_->setIcon(GuiFactory::instance().luaIcon());
        VERIFY(connect(luaConsoleAction_, SIGNAL(triggered()), this, SLOT(openLuaConsole())));
        tools->addAction(luaConsoleAction_);

        encodeDecodeDialogAction_ = new QAction(this);
        encodeDecodeDialogAction_->setIcon(GuiFactory::instance().encodeDecodeIcon());
        VERIFY(connect(encodeDecodeDialogAction_, SIGNAL(triggered()), this, SLOT(openEncodeDecodeDialog())));
        tools->addAction(encodeDecodeDialogAction_);

        //window menu
        QMenu *window = new QMenu(this);
        windowAction_ = menuBar()->addMenu(window);
        fullScreanAction_ = new QAction(this);
        fullScreanAction_->setShortcut(fullScreenKey);
        VERIFY(connect(fullScreanAction_, SIGNAL(triggered()), this, SLOT(enterLeaveFullScreen())));
        window->addAction(fullScreanAction_);

        QMenu *views = new QMenu(translations::trViews, this);
        window->addMenu(views);

        QMenu *helpMenu = new QMenu(this);
        aboutAction_ = new QAction(this);
        VERIFY(connect(aboutAction_, SIGNAL(triggered()), this, SLOT(about())));

        helpAction_ = menuBar()->addMenu(helpMenu);
        helpMenu->addAction(aboutAction_);

        MainWidget *mainW = new MainWidget;
        setCentralWidget(mainW);

        exp_ = new ExplorerTreeView(this);
        VERIFY(connect(exp_, SIGNAL(openedConsole(IServerSPtr, const QString&)), mainW, SLOT(openConsole(IServerSPtr, const QString&))));
        VERIFY(connect(exp_, SIGNAL(executeText(IServerSPtr, const QString&)), mainW, SLOT(executeText(IServerSPtr, const QString&))));
        VERIFY(connect(exp_, SIGNAL(closeServer(IServerSPtr)), &ServersManager::instance(), SLOT(closeServer(IServerSPtr))));
        expDock_ = new QDockWidget(this);
        explorerAction_ = expDock_->toggleViewAction();
        explorerAction_->setShortcut(explorerKeySequence);
        explorerAction_->setChecked(true);
        views->addAction(explorerAction_);

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
        logsAction_->setShortcut(logsKeySequence);
        logsAction_->setChecked(false);
        views->addAction(logsAction_);

        logDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
        logDock_->setWidget(log);
        logDock_->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
        logDock_->setVisible(false);
        addDockWidget(Qt::BottomDockWidgetArea, logDock_);

        setMinimumSize(QSize(min_width, min_height));
        createStatusBar();
        retranslateUi();
    }

    MainWindow::~MainWindow()
    {
    }

    void MainWindow::createStatusBar()
    {
    }

    void MainWindow::open()
    {
        ConnectionsDialog dlg(this);
        int result = dlg.exec();
        if(result == QDialog::Accepted){
            IServerSPtr server = ServersManager::instance().createServer(dlg.selectedConnection());
            exp_->addServer(server);
        }
    }

    void MainWindow::showEvent(QShowEvent* ev)
    {
        QMainWindow::showEvent(ev);
        bool isA = SettingsManager::instance().autoCheckUpdates();
        if(isA && !isCheckedInSession_){
            isCheckedInSession_ = true;
            checkUpdate();
        }
    }

#ifdef OS_ANDROID
    bool MainWindow::event(QEvent *event)
    {
        if (event->type() == QEvent::Gesture){
            QGestureEvent *gest = static_cast<QGestureEvent*>(event);
            if(gest){
                return gestureEvent(gest);
            }
        }
        return QMainWindow::event(event);
    }

    bool MainWindow::gestureEvent(QGestureEvent *event)
    {

        /*if (QGesture *qpan = event->gesture(Qt::PanGesture)){
            QPanGesture* pan = static_cast<QPanGesture*>(qpan);
        }
        if (QGesture *qpinch = event->gesture(Qt::PinchGesture)){
            QPinchGesture* pinch = static_cast<QPinchGesture*>(qpinch);
        }
        if (QGesture *qtap = event->gesture(Qt::TapGesture)){
            QTapGesture* tap = static_cast<QTapGesture*>(qtap);
        }*/

        if (QGesture *qswipe = event->gesture(Qt::SwipeGesture)){
            QSwipeGesture* swipe = static_cast<QSwipeGesture*>(qswipe);
            swipeTriggered(swipe);
        }
        else if (QGesture *qtapandhold = event->gesture(Qt::TapAndHoldGesture)){
            QTapAndHoldGesture* tapandhold = static_cast<QTapAndHoldGesture*>(qtapandhold);
            tapAndHoldTriggered(tapandhold);
            event->accept();
        }

        return true;
    }

    void MainWindow::swipeTriggered(QSwipeGesture* swipeEvent)
    {

    }

    void MainWindow::tapAndHoldTriggered(QTapAndHoldGesture* tapEvent)
    {
        QPoint pos = tapEvent->position().toPoint();
        QContextMenuEvent* cont = new QContextMenuEvent(QContextMenuEvent::Mouse, pos, mapToGlobal(pos));
        QWidget* rec = childAt(pos);
        QApplication::postEvent(rec, cont);
    }
#endif

    void MainWindow::changeEvent(QEvent *ev)
    {
        if(ev->type() == QEvent::LanguageChange){
            retranslateUi();
        }

        return QMainWindow::changeEvent(ev);
    }

    void MainWindow::retranslateUi()
    {
        using namespace translations;
        openAction_->setText(trOpen);
        exitAction_->setText(trExit);
        fileAction_->setText(trFile);
        toolsAction_->setText(trTools);
        pythonConsoleAction_->setText(trPythonConsole);
        luaConsoleAction_->setText(trLuaConsole);
        encodeDecodeDialogAction_->setText(trEncodeDecodeDialog);
        preferencesAction_->setText(trPreferences);
        checkUpdateAction_->setText(trCheckUpdate);
        optionsAction_->setText(trOptions);
        windowAction_->setText(trWindow);
        fullScreanAction_->setText(trEnterFullScreen);
        aboutAction_->setText(tr("About %1...").arg(PROJECT_NAME));
        helpAction_->setText(trHelp);
        explorerAction_->setText(trExpTree);
        logsAction_->setText(trLogs);
        expDock_->setWindowTitle(trExpTree);
        logDock_->setWindowTitle(trLogs);        
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

    void MainWindow::openPythonConsole()
    {
        PythonConsoleDialog dlg(QString(), this);
        dlg.exec();
    }

    void MainWindow::openLuaConsole()
    {
        LuaConsoleDialog dlg(QString(), this);
        dlg.exec();
    }

    void MainWindow::openEncodeDecodeDialog()
    {
        EncodeDecodeDialog dlg(this);
        dlg.exec();
    }

    void MainWindow::enterLeaveFullScreen()
    {
        using namespace translations;
        if(isFullScreen()){
            showNormal();
            fullScreanAction_->setText(trEnterFullScreen);
        }
        else{
            showFullScreen();
            fullScreanAction_->setText(trExitFullScreen);
        }
    }

    void MainWindow::versionAvailible(bool succesResult, const QString& version)
    {
        using namespace translations;
        if(!succesResult){
            QMessageBox::information(this, trCheckVersion, trConnectionErrorText);
            checkUpdateAction_->setEnabled(true);
        }
        else{
            bool isn = isNeededUpdate(version);
            if(isn){
                QMessageBox::information(this, trCheckVersion,
                    QObject::tr("Availible new version: %1")
                        .arg(version));
            }
            else{
                QMessageBox::information(this, trCheckVersion,
                    QObject::tr("<h3>You're' up-to-date!</h3>" PROJECT_NAME_TITLE " %1 is currently the newest version available.")
                        .arg(version));
            }

            checkUpdateAction_->setEnabled(isn);
        }
    }
}

