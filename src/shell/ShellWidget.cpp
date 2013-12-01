#include "shell/ShellWidget.h"

#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QPlainTextEdit>
#include <QToolBar>
#include <QAction>
#include <Qsci/qsciscintilla.h>

#include "common/macros.h"
#include "gui/GuiFactory.h"

namespace fastoredis
{
    ShellWidget::ShellWidget(const IServerPtr &server, QWidget *parent)
        : base_class(parent), _server(server)
    {

        QVBoxLayout *hlayout = new QVBoxLayout(this);
        hlayout->setContentsMargins(0, 0, 0, 0);

        QToolBar *bar = new QToolBar;

        _connectAction = new QAction(GuiFactory::instance().connectIcon(), "Connect", bar);
        VERIFY(connect(_connectAction, SIGNAL(triggered()), this, SLOT(connectToServer())));
        bar->addAction(_connectAction);
        _disConnectAction = new QAction(GuiFactory::instance().disConnectIcon(), "Disconnect", bar);
        VERIFY(connect(_disConnectAction, SIGNAL(triggered()), this, SLOT(disconnectFromServer())));
        bar->addAction(_disConnectAction);
        syncConnectionActions(true);

        VERIFY(connect(_server.get(), SIGNAL(startedConnect(const EventsInfo::ConnectInfoRequest &)), this, SLOT(startConnect(const EventsInfo::ConnectInfoRequest &))));
        VERIFY(connect(_server.get(), SIGNAL(finishedConnect(const EventsInfo::ConnectInfoResponce &)), this, SLOT(finishConnect(const EventsInfo::ConnectInfoResponce &))));
        VERIFY(connect(_server.get(), SIGNAL(startedDisconnect(const EventsInfo::DisonnectInfoRequest &)), this, SLOT(startDisconnect(const EventsInfo::DisonnectInfoRequest &))));
        VERIFY(connect(_server.get(), SIGNAL(finishedDisconnect(const EventsInfo::DisConnectInfoResponce &)), this, SLOT(finishDisconnect(const EventsInfo::DisConnectInfoResponce &))));

        QAction *executeAction = new QAction(GuiFactory::instance().executeIcon(), "Execute", bar);
        VERIFY(connect(executeAction, SIGNAL(triggered()), this, SLOT(execute())));
        bar->addAction(executeAction);

        QAction *stopAction = new QAction(GuiFactory::instance().stopIcon(), "Stop", bar);
        VERIFY(connect(stopAction, SIGNAL(triggered()), this, SLOT(stop())));
        bar->addAction(stopAction);

        hlayout->addWidget(bar);

        _input  = new QsciScintilla;
        _input->setContextMenuPolicy(Qt::CustomContextMenu);
        VERIFY(connect(_input,SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint &))));
        hlayout->addWidget(_input);
        _clear = new QAction("Clear All", this);
        VERIFY(connect(_clear, SIGNAL(triggered()),_input, SLOT(clear())));
        setLayout(hlayout);
    }

    void ShellWidget::showContextMenu(const QPoint &pt)
    {
        QMenu *menu = _input->createStandardContextMenu();
        menu->addAction(_clear);
        _clear->setEnabled(!text().isEmpty());

        menu->exec(_input->mapToGlobal(pt));
        delete menu;
    }

    void ShellWidget::execute()
    {
        _server->execute(text());
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

    void ShellWidget::setText(const QString &text)
    {
        _input->setText(text);
    }

    QString ShellWidget::text() const
    {
        return _input->text();
    }

    void ShellWidget::reload()
    {

    }

    ShellWidget *ShellWidget::duplicate(ShellWidget *src, const QString &text)
    {
        ShellWidget *result = new ShellWidget(src->_server, src->parentWidget());
        result->setText(text);
        return result;
    }

    void ShellWidget::syncConnectionActions(bool isConnectAct)
    {
        _connectAction->setVisible(isConnectAct);
        _disConnectAction->setVisible(!isConnectAct);
    }

    void ShellWidget::startConnect(const EventsInfo::ConnectInfoRequest &req)
    {
        syncConnectionActions(true);
    }

    void ShellWidget::finishConnect(const EventsInfo::ConnectInfoResponce &res)
    {
        Error::ErrorInfo er = res.errorInfo();
        if(!er.isError()){
            syncConnectionActions(false);
        }
    }

    void ShellWidget::startDisconnect(const EventsInfo::DisonnectInfoRequest &req)
    {
        syncConnectionActions(false);
    }

    void ShellWidget::finishDisconnect(const EventsInfo::DisConnectInfoResponce &res)
    {
        syncConnectionActions(true);
    }
}
