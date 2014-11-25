#include "gui/widgets/query_widget.h"

#include <QHBoxLayout>
#include <QSplitter>

#include "shell/shell_widget.h"
#include "gui/widgets/output_widget.h"

namespace fastoredis
{
    QueryWidget::QueryWidget(IServerPtr server, QWidget* parent)
        : QWidget(parent)
    {
        shellWidget_ = new ShellWidget(server);
        outputWidget_ = new OutputWidget;
        VERIFY(connect(shellWidget_, SIGNAL(startedExecute(const EventsInfo::ExecuteInfoRequest &)), outputWidget_, SLOT(startExecute(const EventsInfo::ExecuteInfoRequest &))));
        VERIFY(connect(shellWidget_, SIGNAL(finishedExecute(const EventsInfo::ExecuteInfoResponce &)), outputWidget_, SLOT(finishExecute(const EventsInfo::ExecuteInfoResponce &))));

        QSplitter *splitter = new QSplitter;
        splitter->setOrientation(Qt::Vertical);
        splitter->setHandleWidth(1);
        splitter->setContentsMargins(0, 0, 0, 0);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        splitter->addWidget(shellWidget_);
        splitter->addWidget(outputWidget_);
        mainLayout->addWidget(splitter);

        setLayout(mainLayout);
    }

    QString QueryWidget::inputText() const
    {
        return shellWidget_->text();
    }

    void QueryWidget::reload()
    {

    }

    QueryWidget* QueryWidget::clone(const QString &text)
    {
        QueryWidget *result = new QueryWidget(shellWidget_->server(), parentWidget());
        result->shellWidget_->setText(text);
        return result;
    }
}
