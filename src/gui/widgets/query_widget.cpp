#include "gui/widgets/query_widget.h"

#include <QHBoxLayout>
#include <QSplitter>

#include "shell/redis_shell_widget.h"
#include "gui/widgets/output_widget.h"

namespace fastoredis
{
    QueryWidget::QueryWidget(IServerPtr server, QWidget* parent)
        : QWidget(parent)
    {
        RedisShellWidget_ = new RedisShellWidget(server);
        const QString delemitr = server->outputDelemitr();

        outputWidget_ = new OutputWidget(delemitr);
        VERIFY(connect(RedisShellWidget_, SIGNAL(rootCreated(const EventsInfo::CommandRootCreatedInfo&)), outputWidget_, SLOT(rootCreate(const EventsInfo::CommandRootCreatedInfo&))));
        VERIFY(connect(RedisShellWidget_, SIGNAL(rootCompleated(const EventsInfo::CommandRootCompleatedInfo& )), outputWidget_, SLOT(rootCompleate(const EventsInfo::CommandRootCompleatedInfo&))));

        VERIFY(connect(RedisShellWidget_, SIGNAL(addedChild(FastoObject *)), outputWidget_, SLOT(addChild(FastoObject *))));
        VERIFY(connect(RedisShellWidget_, SIGNAL(itemUpdated(FastoObject*, const QString&)), outputWidget_, SLOT(itemUpdate(FastoObject*, const QString&))));

        QSplitter *splitter = new QSplitter;
        splitter->setOrientation(Qt::Vertical);
        splitter->setHandleWidth(1);
        splitter->setContentsMargins(0, 0, 0, 0);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        splitter->addWidget(RedisShellWidget_);
        splitter->addWidget(outputWidget_);
        mainLayout->addWidget(splitter);

        setLayout(mainLayout);
    }

    QString QueryWidget::inputText() const
    {
        return RedisShellWidget_->text();
    }

    void QueryWidget::reload()
    {

    }

    QueryWidget* QueryWidget::clone(const QString &text)
    {
        QueryWidget *result = new QueryWidget(RedisShellWidget_->server(), parentWidget());
        result->RedisShellWidget_->setText(text);
        return result;
    }
}
