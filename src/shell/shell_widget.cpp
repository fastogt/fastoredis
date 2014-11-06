#include "shell/shell_widget.h"

#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QPlainTextEdit>
#include <QToolBar>
#include <QAction>
#include <QProgressBar>
#include <QSplitter>
#include <QTextStream>
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>

#include "common/qt/convert_string.h"
#include "common/safe_sprintf.h"

#include "gui/gui_factory.h"
#include "gui/icon_label.h"

#include "shell/redis_shell.h"

#include "core/logger.h"

namespace
{
    const QString filterForScripts = QObject::tr("Text Files (*.txt);; All Files (*.*)");

    bool loadFromFileText(const QString &filePath, QString &text, QWidget* parent)
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
            static const uint16_t size_buff = 256;
            char buff[size_buff] = {0};
            common::strings::SafeSNPrintf(buff, size_buff, PROJECT_NAME" can't read from %1:\n%2.", convertToString(filePath).c_str(),
                            convertToString(file.errorString()).c_str());
            ErrorValueSPtr er(new ErrorValue(buff, Value::E_ERROR));
            fastoredis::LOG_ERROR(er);
            QMessageBox::critical(parent, QString("Error"),
                QObject::tr(PROJECT_NAME" can't read from %1:\n%2.")
                    .arg(filePath)
                    .arg(file.errorString()));
        }

        return result;
    }

    bool saveToFileText(QString filePath, const QString &text, QWidget* parent)
    {
        if (filePath.isEmpty())
            return false;

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
            QMessageBox::critical(parent, QString("Error"),
                QObject::tr(PROJECT_NAME" can't save to %1:\n%2.")
                    .arg(filePath)
                    .arg(file.errorString()));
        }

        return result;
    }
}

namespace fastoredis
{
    ShellWidget::ShellWidget(IServerPtr server, const QString &filePath, QWidget *parent)
        : QWidget(parent), _server(server), _filePath(filePath)
    {
        QVBoxLayout *mainlayout = new QVBoxLayout(this);
        QHBoxLayout *hlayout = new QHBoxLayout(this);
        hlayout->setContentsMargins(0, 0, 0, 0);

        QToolBar *conbar = new QToolBar;
        conbar->setMovable(true);
        connectAction_ = new QAction(GuiFactory::instance().connectIcon(), "Connect", conbar);
        VERIFY(connect(connectAction_, SIGNAL(triggered()), this, SLOT(connectToServer())));
        conbar->addAction(connectAction_);
        disConnectAction_ = new QAction(GuiFactory::instance().disConnectIcon(), "Disconnect", conbar);
        VERIFY(connect(disConnectAction_, SIGNAL(triggered()), this, SLOT(disconnectFromServer())));
        conbar->addAction(disConnectAction_);
        executeAction_ = new QAction(GuiFactory::instance().executeIcon(), "Execute", conbar);
        VERIFY(connect(executeAction_, SIGNAL(triggered()), this, SLOT(execute())));
        conbar->addAction(executeAction_);
        QAction *stopAction = new QAction(GuiFactory::instance().stopIcon(), "Stop", conbar);
        VERIFY(connect(stopAction, SIGNAL(triggered()), this, SLOT(stop())));
        conbar->addAction(stopAction);
        serverName_ = new IconLabel(GuiFactory::instance().serverIcon(), _server->address());
        conbar->addWidget(serverName_);

        VERIFY(connect(_server.get(), SIGNAL(startedConnect(const EventsInfo::ConnectInfoRequest &)), this, SLOT(startConnect(const EventsInfo::ConnectInfoRequest &))));
        VERIFY(connect(_server.get(), SIGNAL(finishedConnect(const EventsInfo::ConnectInfoResponce &)), this, SLOT(finishConnect(const EventsInfo::ConnectInfoResponce &))));
        VERIFY(connect(_server.get(), SIGNAL(startedDisconnect(const EventsInfo::DisonnectInfoRequest &)), this, SLOT(startDisconnect(const EventsInfo::DisonnectInfoRequest &))));
        VERIFY(connect(_server.get(), SIGNAL(finishedDisconnect(const EventsInfo::DisConnectInfoResponce &)), this, SLOT(finishDisconnect(const EventsInfo::DisConnectInfoResponce &))));
        VERIFY(connect(_server.get(), SIGNAL(startedExecute(const EventsInfo::ExecuteInfoRequest &)), this, SIGNAL(startedExecute(const EventsInfo::ExecuteInfoRequest &))));
        VERIFY(connect(_server.get(), SIGNAL(finishedExecute(const EventsInfo::ExecuteInfoResponce &)), this, SIGNAL(finishedExecute(const EventsInfo::ExecuteInfoResponce &))));
        VERIFY(connect(_server.get(), SIGNAL(progressChanged(const EventsInfo::ProgressInfoResponce &)), this, SLOT(progressChange(const EventsInfo::ProgressInfoResponce &))));

        syncConnectionActions();

        QToolBar *savebar = new QToolBar;
        savebar->setMovable(true);
        loadAction_ = new QAction(GuiFactory::instance().loadIcon(), "Load", savebar);
        VERIFY(connect(loadAction_, SIGNAL(triggered()), this, SLOT(loadFromFile())));
        savebar->addAction(loadAction_);
        saveAction_ = new QAction(GuiFactory::instance().saveIcon(), "Save", savebar);
        VERIFY(connect(saveAction_, SIGNAL(triggered()), this, SLOT(saveToFile())));
        savebar->addAction(saveAction_);
        saveAsAction_ = new QAction(GuiFactory::instance().saveAsIcon(), "Save as", savebar);
        VERIFY(connect(saveAsAction_, SIGNAL(triggered()), this, SLOT(saveToFileAs())));
        savebar->addAction(saveAsAction_);

        hlayout->addWidget(savebar);
        hlayout->addWidget(conbar);        

        QSplitter *splitter = new QSplitter;
        splitter->setOrientation(Qt::Horizontal);
        splitter->setHandleWidth(1);
        splitter->setContentsMargins(0, 0, 0, 0);
        hlayout->addWidget(splitter);

        workProgressBar_ = new QProgressBar;
        hlayout->addWidget(workProgressBar_);

        mainlayout->addLayout(hlayout);

        input_ = new RedisShell;
        input_->setContextMenuPolicy(Qt::CustomContextMenu);
        VERIFY(connect(input_, SIGNAL(executed()), this, SLOT(execute())));

        mainlayout->addWidget(input_);

        setLayout(mainlayout);
    }

    void ShellWidget::execute()
    {
        QString selected = input_->selectedText();
        if(selected.isEmpty()){
            selected = input_->text();
        }

        _server->execute(selected);
    }

    QString ShellWidget::text() const
    {
        return input_->text();
    }

    void ShellWidget::stop()
    {
        _server->stopCurrentEvent();
    }

    void ShellWidget::connectToServer()
    {
        _server->connect();
    }

    void ShellWidget::disconnectFromServer()
    {
        _server->disconnect();
    }

    IServerPtr ShellWidget::server() const
    {
        return _server;
    }

    void ShellWidget::setText(const QString& text)
    {
        input_->setText(text);
    }

    void ShellWidget::syncConnectionActions()
    {
        connectAction_->setVisible(!_server->isConnected());
        disConnectAction_->setVisible(_server->isConnected());
        executeAction_->setEnabled(_server->isConnected());
    }

    void ShellWidget::startConnect(const EventsInfo::ConnectInfoRequest &req)
    {
        syncConnectionActions();
    }

    void ShellWidget::finishConnect(const EventsInfo::ConnectInfoResponce &res)
    {
        serverName_->setText(_server->address());
        syncConnectionActions();
    }

    void ShellWidget::startDisconnect(const EventsInfo::DisonnectInfoRequest &req)
    {
        syncConnectionActions();
    }

    void ShellWidget::finishDisconnect(const EventsInfo::DisConnectInfoResponce &res)
    {
        syncConnectionActions();
    }

    void ShellWidget::progressChange(const EventsInfo::ProgressInfoResponce &res)
    {
        workProgressBar_->setValue(res._progress);
    }

    void ShellWidget::loadFromFile()
    {
        QString out;
        bool res = loadFromFile(_filePath);
        if(res){
            setText(out);
        }
    }

    bool ShellWidget::loadFromFile(const QString &path)
    {
        bool res = false;
        QString filepath = QFileDialog::getOpenFileName(this, path, QString(), filterForScripts);
        if (!filepath.isEmpty()) {
            QString out;
            if (loadFromFileText(filepath, out, this)) {
                setText(out);
                _filePath = filepath;
                res = true;
            }
        }
        return res;
    }

    void ShellWidget::saveToFileAs()
    {
        QString filepath = QFileDialog::getSaveFileName(this,
            QObject::tr("Save As"), _filePath, filterForScripts);

        if (saveToFileText(filepath,text(), this)) {
            _filePath = filepath;
        }
    }

    void ShellWidget::saveToFile()
    {
        if(_filePath.isEmpty()){
            saveToFileAs();
        } else {
            saveToFileText(_filePath, text(), this);
        }
    }
}
