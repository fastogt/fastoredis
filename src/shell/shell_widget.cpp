#include "shell/shell_widget.h"

#include <QProgressBar>
#include <QSplitter>
#include <QAction>
#include <QToolBar>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QApplication>
#include <QTextStream>
#include <QFileDialog>

#include "common/qt/convert_string.h"
#include "common/sprintf.h"

#include "gui/shortcuts.h"
#include "gui/gui_factory.h"
#include "gui/icon_label.h"

#include "translations/global.h"

#include "shell/redis_shell.h"
#include "shell/memcached_shell.h"

#include "core/logger.h"

using namespace fastoredis::translations;

namespace
{
    bool loadFromFileText(const QString& filePath, QString& text, QWidget* parent)
    {
        using namespace common;

        bool result = false;
        QFile file(filePath);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream in(&file);
            QApplication::setOverrideCursor(Qt::WaitCursor);
            text = in.readAll();
            QApplication::restoreOverrideCursor();
            result = true;
        }
        else {
            char buff[256] = {0};
            SNPrintf(buff, sizeof(buff), PROJECT_NAME" can't read from %s:\n%s.", convertToString(filePath).c_str(),
                            convertToString(file.errorString()).c_str());
            ErrorValueSPtr er = common::make_error_value(buff, Value::E_ERROR);
            fastoredis::LOG_ERROR(er, true);
            QMessageBox::critical(parent, trError,
                QObject::tr(PROJECT_NAME" can't read from %1:\n%2.")
                    .arg(filePath)
                    .arg(file.errorString()));
        }

        return result;
    }

    bool saveToFileText(QString filePath, const QString& text, QWidget* parent)
    {
        if (filePath.isEmpty()){
            return false;
        }

#ifdef OS_LINUX
        if (QFileInfo(filePath).suffix().isEmpty()) {
            filePath += ".txt";
        }
#endif
        bool result = false;
        QFile file(filePath);
        if (file.open(QFile::WriteOnly | QFile::Text)) {
            QTextStream out(&file);
            QApplication::setOverrideCursor(Qt::WaitCursor);
            out << text;
            QApplication::restoreOverrideCursor();
            result = true;
        }
        else {
            QMessageBox::critical(parent, trError,
                QObject::tr(PROJECT_NAME" can't save to %1:\n%2.")
                    .arg(filePath)
                    .arg(file.errorString()));
        }

        return result;
    }

    const QSize iconSize = QSize(24, 24);
}

namespace fastoredis
{
    BaseShellWidget::BaseShellWidget(IServerSPtr server, const QString& filePath, QWidget* parent)
        : QWidget(parent), server_(server), filePath_(filePath)
    {
        VERIFY(connect(server_.get(), SIGNAL(startedConnect(const EventsInfo::ConnectInfoRequest &)), this, SLOT(startConnect(const EventsInfo::ConnectInfoRequest &))));
        VERIFY(connect(server_.get(), SIGNAL(finishedConnect(const EventsInfo::ConnectInfoResponce &)), this, SLOT(finishConnect(const EventsInfo::ConnectInfoResponce &))));
        VERIFY(connect(server_.get(), SIGNAL(startedDisconnect(const EventsInfo::DisonnectInfoRequest &)), this, SLOT(startDisconnect(const EventsInfo::DisonnectInfoRequest &))));
        VERIFY(connect(server_.get(), SIGNAL(finishedDisconnect(const EventsInfo::DisConnectInfoResponce &)), this, SLOT(finishDisconnect(const EventsInfo::DisConnectInfoResponce &))));
        VERIFY(connect(server_.get(), SIGNAL(startedExecute(const EventsInfo::ExecuteInfoRequest &)), this, SIGNAL(startedExecute(const EventsInfo::ExecuteInfoRequest &))));
        VERIFY(connect(server_.get(), SIGNAL(progressChanged(const EventsInfo::ProgressInfoResponce &)), this, SLOT(progressChange(const EventsInfo::ProgressInfoResponce &))));

        VERIFY(connect(server_.get(), SIGNAL(enteredMode(const EventsInfo::EnterModeInfo&)), this, SLOT(enterMode(const EventsInfo::EnterModeInfo&))));
        VERIFY(connect(server_.get(), SIGNAL(leavedMode(const EventsInfo::LeaveModeInfo&)), this, SLOT(leaveMode(const EventsInfo::LeaveModeInfo&))));

        VERIFY(connect(server_.get(), SIGNAL(rootCreated(const EventsInfo::CommandRootCreatedInfo&)), this, SIGNAL(rootCreated(const EventsInfo::CommandRootCreatedInfo&))));
        VERIFY(connect(server_.get(), SIGNAL(rootCompleated(const EventsInfo::CommandRootCompleatedInfo& )), this, SIGNAL(rootCompleated(const EventsInfo::CommandRootCompleatedInfo&))));

        VERIFY(connect(server_.get(), SIGNAL(addedChild(FastoObject *)), this, SIGNAL(addedChild(FastoObject *))));
        VERIFY(connect(server_.get(), SIGNAL(itemUpdated(FastoObject*, const QString&)), this, SIGNAL(itemUpdated(FastoObject*, const QString&)), Qt::UniqueConnection));

        QVBoxLayout* mainlayout = new QVBoxLayout;
        QHBoxLayout* hlayout = new QHBoxLayout;
        hlayout->setContentsMargins(0, 0, 0, 0);
        hlayout->setSpacing(0);

        QToolBar *savebar = new QToolBar;
        savebar->setStyleSheet("QToolBar { border: 0px; }");

        loadAction_ = new QAction(GuiFactory::instance().loadIcon(), trLoad, savebar);
        VERIFY(connect(loadAction_, SIGNAL(triggered()), this, SLOT(loadFromFile())));
        savebar->addAction(loadAction_);

        saveAction_ = new QAction(GuiFactory::instance().saveIcon(), trSave, savebar);
        VERIFY(connect(saveAction_, SIGNAL(triggered()), this, SLOT(saveToFile())));
        savebar->addAction(saveAction_);

        saveAsAction_ = new QAction(GuiFactory::instance().saveAsIcon(), trSaveAs, savebar);
        VERIFY(connect(saveAsAction_, SIGNAL(triggered()), this, SLOT(saveToFileAs())));
        savebar->addAction(saveAsAction_);

        connectAction_ = new QAction(GuiFactory::instance().connectIcon(), trConnect, savebar);
        VERIFY(connect(connectAction_, SIGNAL(triggered()), this, SLOT(connectToServer())));
        savebar->addAction(connectAction_);

        disConnectAction_ = new QAction(GuiFactory::instance().disConnectIcon(), trDisconnect, savebar);
        VERIFY(connect(disConnectAction_, SIGNAL(triggered()), this, SLOT(disconnectFromServer())));
        savebar->addAction(disConnectAction_);

        executeAction_ = new QAction(GuiFactory::instance().executeIcon(), trExecute, savebar);
        executeAction_->setShortcut(executeKey);
        VERIFY(connect(executeAction_, SIGNAL(triggered()), this, SLOT(execute())));
        savebar->addAction(executeAction_);

        QAction *stopAction = new QAction(GuiFactory::instance().stopIcon(), trStop, savebar);
        VERIFY(connect(stopAction, SIGNAL(triggered()), this, SLOT(stop())));
        savebar->addAction(stopAction);

        const ConnectionMode mode = IntaractiveMode;
        connectionMode_ = new IconLabel(GuiFactory::instance().modeIcon(mode), common::convertFromString<QString>(common::convertToString(mode)), iconSize);

        hlayout->addWidget(savebar);

        QSplitter *splitter = new QSplitter;
        splitter->setOrientation(Qt::Horizontal);
        splitter->setHandleWidth(1);
        splitter->setContentsMargins(0, 0, 0, 0);
        hlayout->addWidget(splitter);

        hlayout->addWidget(connectionMode_);
        workProgressBar_ = new QProgressBar;
        hlayout->addWidget(workProgressBar_);

        connectionTypes type = server->type();
        if(type == REDIS){
            input_ = new RedisShell;
            setToolTip(tr("Based on redis-cli version: %1").arg(input_->version()));
        }
        else if(type == MEMCACHED){
            input_ = new MemcachedShell;
            setToolTip(tr("Based on libmemcached version: %1").arg(input_->version()));
        }
        else{
            NOTREACHED();
        }

        input_->setContextMenuPolicy(Qt::CustomContextMenu);

        mainlayout->addLayout(hlayout);
        mainlayout->addWidget(input_);

        setLayout(mainlayout);

        syncConnectionActions();
    }

    void BaseShellWidget::enterMode(const EventsInfo::EnterModeInfo& res)
    {
        ConnectionMode mode = res.mode_;
        connectionMode_->setIcon(GuiFactory::instance().modeIcon(mode), iconSize);
        std::string modeText = common::convertToString(mode);
        connectionMode_->setText(common::convertFromString<QString>(modeText));
    }

    void BaseShellWidget::leaveMode(const EventsInfo::LeaveModeInfo& res)
    {

    }

    void BaseShellWidget::execute()
    {
        QString selected = input_->selectedText();
        if(selected.isEmpty()){
            selected = input_->text();
        }

        server_->execute(selected);
    }

    QString BaseShellWidget::text() const
    {
        return input_->text();
    }

    void BaseShellWidget::stop()
    {
        server_->stopCurrentEvent();
    }

    void BaseShellWidget::connectToServer()
    {
        server_->connect();
    }

    void BaseShellWidget::disconnectFromServer()
    {
        server_->disconnect();
    }

    IServerSPtr BaseShellWidget::server() const
    {
        return server_;
    }

    void BaseShellWidget::setText(const QString& text)
    {
        input_->setText(text);
    }

    void BaseShellWidget::execute(const QString& text)
    {
        input_->setText(text);
        execute();
    }

    void BaseShellWidget::syncConnectionActions()
    {
        connectAction_->setVisible(!server_->isConnected());
        disConnectAction_->setVisible(server_->isConnected());
        executeAction_->setEnabled(server_->isConnected());
    }

    void BaseShellWidget::startConnect(const EventsInfo::ConnectInfoRequest& req)
    {
        syncConnectionActions();
    }

    void BaseShellWidget::finishConnect(const EventsInfo::ConnectInfoResponce& res)
    {
        syncConnectionActions();
    }

    void BaseShellWidget::startDisconnect(const EventsInfo::DisonnectInfoRequest& req)
    {
        syncConnectionActions();
    }

    void BaseShellWidget::finishDisconnect(const EventsInfo::DisConnectInfoResponce& res)
    {
        syncConnectionActions();
    }

    void BaseShellWidget::progressChange(const EventsInfo::ProgressInfoResponce& res)
    {
        workProgressBar_->setValue(res.progress_);
    }

    void BaseShellWidget::loadFromFile()
    {
        loadFromFile(filePath_);
    }

    bool BaseShellWidget::loadFromFile(const QString& path)
    {
        bool res = false;
        QString filepath = QFileDialog::getOpenFileName(this, path, QString(), trfilterForScripts);
        if (!filepath.isEmpty()) {
            QString out;
            if (loadFromFileText(filepath, out, this)) {
                setText(out);
                filePath_ = filepath;
                res = true;
            }
        }
        return res;
    }

    void BaseShellWidget::saveToFileAs()
    {
        QString filepath = QFileDialog::getSaveFileName(this,
            trSaveAs, filePath_, trfilterForScripts);

        if (saveToFileText(filepath,text(), this)) {
            filePath_ = filepath;
        }
    }

    void BaseShellWidget::saveToFile()
    {
        if(filePath_.isEmpty()){
            saveToFileAs();
        }
        else {
            saveToFileText(filePath_, text(), this);
        }
    }
}
