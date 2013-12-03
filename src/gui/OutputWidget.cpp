#include "gui/OutputWidget.h"

#include <QHBoxLayout>
#include <QMenu>
#include "gui/FastoEditor.h"

namespace fastoredis
{
    OutputWidget::OutputWidget(QWidget* parent)
        : base_class(parent)
    {
        FastoEditor *edit = new FastoEditor(this);
        QVBoxLayout *mainL = new QVBoxLayout;
        mainL->addWidget(edit);
        setLayout(mainL);
    }

    void OutputWidget::startExecute(const EventsInfo::ExecuteInfoRequest &req)
    {

    }

    void OutputWidget::finishExecute(const EventsInfo::ExecuteInfoResponce &res)
    {

    }
}
