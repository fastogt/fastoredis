#include "gui/QueryWidget.h"

#include <QHBoxLayout>
#include <QMenu>

#include "common/macros.h"
#include "shell/ShellWidget.h"
#include "gui/OutputWidget.h"

namespace fastoredis
{
    QueryWidget::QueryWidget(const IServerPtr &server, QWidget* parent)
        : base_class(parent)
    {
        _shellWidget = new ShellWidget(server,this);
        _outputWidget = new OutputWidget(this);
        VERIFY(connect(_shellWidget, SIGNAL(startedExecute(const EventsInfo::ExecuteInfoRequest &)), _outputWidget, SLOT(startExecute(const EventsInfo::ExecuteInfoRequest &))));
        VERIFY(connect(_shellWidget, SIGNAL(finishedExecute(const EventsInfo::ExecuteInfoResponce &)), _outputWidget, SLOT(finishExecute(const EventsInfo::ExecuteInfoResponce &))));

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addWidget(_shellWidget);
        mainLayout->addWidget(_outputWidget);

        setLayout(mainLayout);
    }

    QString QueryWidget::inputText() const
    {
        return _shellWidget->text();
    }

    void QueryWidget::reload()
    {

    }

    QueryWidget *QueryWidget::duplicate(QueryWidget *src, const QString &text)
    {
        QueryWidget *result = new QueryWidget(src->_shellWidget->server(), src->parentWidget());
        result->_shellWidget->setText(text);
        return result;
    }
}
