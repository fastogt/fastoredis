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

#include "gui/gui_factory.h"
#include "gui/icon_label.h"

#include "shell/redis_shell.h"

#include "core/logger.h"

namespace
{
    const QString filterForScripts = QObject::tr("JavaScript (*.js);; All Files (*.*)");

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
            char16 buff[256] = {0};
            string16_sprintf(buff, UTEXT(PROJECT_NAME" can't read from %1:\n%2."), convertToString16(filePath).c_str(),
                            convertToString16(file.errorString()).c_str());
            ErrorValue er(buff, Value::E_ERROR);
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

#ifdef Q_OS_LINUX
        if (QFileInfo(filePath).suffix().isEmpty()) {
            filePath += ".js";
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
        QVBoxLayout *mainlayout = new QVBoxLayout;
        QHBoxLayout *hlayout = new QHBoxLayout;
        hlayout->setContentsMargins(0, 0, 0, 0);

        QToolBar *conbar = new QToolBar;
        conbar->setMovable(true);
        _connectAction = new QAction(GuiFactory::instance().connectIcon(), "Connect", conbar);
        VERIFY(connect(_connectAction, SIGNAL(triggered()), this, SLOT(connectToServer())));
        conbar->addAction(_connectAction);
        _disConnectAction = new QAction(GuiFactory::instance().disConnectIcon(), "Disconnect", conbar);
        VERIFY(connect(_disConnectAction, SIGNAL(triggered()), this, SLOT(disconnectFromServer())));
        conbar->addAction(_disConnectAction);
        _executeAction = new QAction(GuiFactory::instance().executeIcon(), "Execute", conbar);
        VERIFY(connect(_executeAction, SIGNAL(triggered()), this, SLOT(execute())));
        conbar->addAction(_executeAction);
        QAction *stopAction = new QAction(GuiFactory::instance().stopIcon(), "Stop", conbar);
        VERIFY(connect(stopAction, SIGNAL(triggered()), this, SLOT(stop())));
        conbar->addAction(stopAction);
        _serverName = new IconLabel(GuiFactory::instance().serverIcon(), _server->address());
        conbar->addWidget(_serverName);

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
        _loadAction = new QAction(GuiFactory::instance().loadIcon(), "Load", savebar);
        VERIFY(connect(_loadAction, SIGNAL(triggered()), this, SLOT(loadFromFile())));
        savebar->addAction(_loadAction);
        _saveAction = new QAction(GuiFactory::instance().saveIcon(), "Save", savebar);
        VERIFY(connect(_saveAction, SIGNAL(triggered()), this, SLOT(saveToFile())));
        savebar->addAction(_saveAction);
        _saveAsAction = new QAction(GuiFactory::instance().saveAsIcon(), "Save as", savebar);
        VERIFY(connect(_saveAsAction, SIGNAL(triggered()), this, SLOT(saveToFileAs())));
        savebar->addAction(_saveAsAction);

        hlayout->addWidget(savebar);
        hlayout->addWidget(conbar);        

        QSplitter *splitter = new QSplitter;
        splitter->setOrientation(Qt::Horizontal);
        splitter->setHandleWidth(1);
        splitter->setContentsMargins(0, 0, 0, 0);
        hlayout->addWidget(splitter);

        _workProgressBar = new QProgressBar;
        hlayout->addWidget(_workProgressBar);

        mainlayout->addLayout(hlayout);

        _input = new RedisShell();
        _input->setContextMenuPolicy(Qt::CustomContextMenu);
        VERIFY(connect(_input, SIGNAL(executed()), this, SLOT(execute())));

        mainlayout->addWidget(_input);

        setLayout(mainlayout);
    }

    void ShellWidget::execute()
    {
        QString selected = _input->selectedText();
        if(selected.isEmpty()){
            selected = _input->text();
        }

        _server->execute(selected);
    }

    QString ShellWidget::text() const
    {
        return _input->text();
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
        _input->setText(text);
    }

    void ShellWidget::syncConnectionActions()
    {
        _connectAction->setVisible(!_server->isConnected());
        _disConnectAction->setVisible(_server->isConnected());
        _executeAction->setEnabled(_server->isConnected());
    }

    void ShellWidget::startConnect(const EventsInfo::ConnectInfoRequest &req)
    {
        syncConnectionActions();
    }

    void ShellWidget::finishConnect(const EventsInfo::ConnectInfoResponce &res)
    {
        _serverName->setText(_server->address());
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
        _workProgressBar->setValue(res._progress);
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
