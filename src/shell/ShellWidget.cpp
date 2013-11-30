#include "shell/ShellWidget.h"

#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QToolBar>
#include <QAction>

#include "common/macros.h"
#include "gui/GuiFactory.h"

namespace fastoredis
{
    ShellWidget::ShellWidget(const IServerPtr &server, QWidget *parent)
        : base_class(parent), _server(server)
    {

        QVBoxLayout *hlayout = new QVBoxLayout(this);
        hlayout->setContentsMargins(0,0,0,0);

        QToolBar *bar = new QToolBar;
        QAction *connectAction = new QAction(GuiFactory::instance().connectIcon(), "Connect", bar);
        VERIFY(connect(connectAction, SIGNAL(triggered()), this, SLOT(connectToServer())));
        bar->addAction(connectAction);
        QAction *executeAction = new QAction(GuiFactory::instance().executeIcon(), "Execute", bar);
        VERIFY(connect(executeAction, SIGNAL(triggered()), this, SLOT(execute())));
        bar->addAction(executeAction);
        QAction *stopAction = new QAction(GuiFactory::instance().stopIcon(), "Stop", bar);
        VERIFY(connect(stopAction, SIGNAL(triggered()), this, SLOT(stop())));
        bar->addAction(stopAction);

        hlayout->addWidget(bar);

        _input  = new QTextEdit;
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
        _clear->setEnabled(!_input->toPlainText().isEmpty());

        menu->exec(_input->mapToGlobal(pt));
        delete menu;
    }

    void ShellWidget::execute()
    {
        QString text = _input->toPlainText();
        _server->execute(text);
    }

    void ShellWidget::stop()
    {

    }

    void ShellWidget::connectToServer()
    {
        _server->connect();
    }

    void ShellWidget::disconnectFromServer()
    {
        _server->disconnect();
    }
}
