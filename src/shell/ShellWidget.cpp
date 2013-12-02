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
#include "shell/RedisShell.h"

namespace fastoredis
{
    ShellWidget::ShellWidget(const IServerPtr &server, QWidget *parent)
        : base_class(parent), _server(server)
    {
        QVBoxLayout *mainlayout = new QVBoxLayout(this);
        QHBoxLayout *hlayout = new QHBoxLayout(this);
        hlayout->setContentsMargins(0, 0, 0, 0);

        QToolBar *conbar = new QToolBar;
        conbar->setMovable(true);
        _connectAction = new QAction(GuiFactory::instance().connectIcon(), "Connect", conbar);
        VERIFY(connect(_connectAction, SIGNAL(triggered()), this, SLOT(connectToServer())));
        conbar->addAction(_connectAction);
        _disConnectAction = new QAction(GuiFactory::instance().disConnectIcon(), "Disconnect", conbar);
        VERIFY(connect(_disConnectAction, SIGNAL(triggered()), this, SLOT(disconnectFromServer())));
        conbar->addAction(_disConnectAction);
        syncConnectionActions();

        VERIFY(connect(_server.get(), SIGNAL(startedConnect(const EventsInfo::ConnectInfoRequest &)), this, SLOT(startConnect(const EventsInfo::ConnectInfoRequest &))));
        VERIFY(connect(_server.get(), SIGNAL(finishedConnect(const EventsInfo::ConnectInfoResponce &)), this, SLOT(finishConnect(const EventsInfo::ConnectInfoResponce &))));
        VERIFY(connect(_server.get(), SIGNAL(startedDisconnect(const EventsInfo::DisonnectInfoRequest &)), this, SLOT(startDisconnect(const EventsInfo::DisonnectInfoRequest &))));
        VERIFY(connect(_server.get(), SIGNAL(finishedDisconnect(const EventsInfo::DisConnectInfoResponce &)), this, SLOT(finishDisconnect(const EventsInfo::DisConnectInfoResponce &))));
        VERIFY(connect(_server.get(), SIGNAL(progressChanged(const EventsInfo::ProgressResponceInfo &)), this, SLOT(progressChange(const EventsInfo::ProgressResponceInfo &))));

        QToolBar *exeBar = new QToolBar;
        exeBar->setMovable(true);
        QAction *executeAction = new QAction(GuiFactory::instance().executeIcon(), "Execute", exeBar);
        VERIFY(connect(executeAction, SIGNAL(triggered()), this, SLOT(execute())));
        exeBar->addAction(executeAction);

        QAction *stopAction = new QAction(GuiFactory::instance().stopIcon(), "Stop", exeBar);
        VERIFY(connect(stopAction, SIGNAL(triggered()), this, SLOT(stop())));
        exeBar->addAction(stopAction);

        hlayout->addWidget(conbar);
        hlayout->addWidget(exeBar);

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
        VERIFY(connect(_input, SIGNAL(showAutocompletion(const QString &)), this, SLOT(showAutocompletion(const QString &))));

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

    void ShellWidget::reload()
    {

    }

    ShellWidget *ShellWidget::duplicate(ShellWidget *src, const QString &text)
    {
        ShellWidget *result = new ShellWidget(src->_server, src->parentWidget());
        result->_input->setText(text);
        return result;
    }

    void ShellWidget::syncConnectionActions()
    {
        _connectAction->setVisible(!_server->isConnected());
        _disConnectAction->setVisible(_server->isConnected());
    }

    void ShellWidget::startConnect(const EventsInfo::ConnectInfoRequest &req)
    {
        syncConnectionActions();
    }

    void ShellWidget::finishConnect(const EventsInfo::ConnectInfoResponce &res)
    {
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

    void ShellWidget::showAutocompletion(const QString &prefix)
    {
        //_input->refreshCompleteList(_server->getAutoComplete(prefix));
    }
}
