#include "gui/widgets/query_widget.h"

#include <QHBoxLayout>
#include <QMenu>

#include "common/macros.h"
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

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addWidget(shellWidget_);
        mainLayout->addWidget(outputWidget_);

        setLayout(mainLayout);
    }

    QString QueryWidget::inputText() const
    {
        return shellWidget_->text();
    }

    void QueryWidget::reload()
    {

    }

    QueryWidget *QueryWidget::duplicate(QueryWidget *src, const QString &text)
    {
        QueryWidget *result = new QueryWidget(src->shellWidget_->server(), src->parentWidget());
        result->shellWidget_->setText(text);
        return result;
    }
}
