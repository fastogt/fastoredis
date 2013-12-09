#include "shell/ShellWidget.h"

#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QPlainTextEdit>
#include <QToolBar>
#include <QAction>
#include <QProgressBar>
#include <QSplitter>

#include "common/qt_helper/converter_patterns.h"
#include "gui/GuiFactory.h"
#include "gui/IconLabel.h"
#include "shell/RedisShell.h"

namespace fastoredis
{
    ShellWidget::ShellWidget(const IServerPtr &server, QWidget *parent)
        : base_class(parent), _server(server)
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
        VERIFY(connect(_server.get(), SIGNAL(progressChanged(const EventsInfo::ProgressResponceInfo &)), this, SLOT(progressChange(const EventsInfo::ProgressResponceInfo &))));

        syncConnectionActions();

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

    const IServerPtr &ShellWidget::server() const
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

    void ShellWidget::progressChange(const EventsInfo::ProgressResponceInfo &res)
    {
        _workProgressBar->setValue(res._progress);
    }
}
