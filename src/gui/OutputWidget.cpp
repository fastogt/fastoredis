#include "gui/OutputWidget.h"

#include <QHBoxLayout>
#include <QMenu>

#include "gui/FastoEditor.h"
#include "common/qt_helper/converter_patterns.h"

namespace fastoredis
{
    OutputWidget::OutputWidget(QWidget* parent)
        : base_class(parent)
    {
        _editor = new FastoEditor(this);
        QVBoxLayout *mainL = new QVBoxLayout;
        mainL->addWidget(_editor);
        setLayout(mainL);
    }

    void OutputWidget::startExecute(const EventsInfo::ExecuteInfoRequest &req)
    {

    }

    void OutputWidget::finishExecute(const EventsInfo::ExecuteInfoResponce &res)
    {
        _editor->clear();
        FastoObjectPtr ptr = res._out;
        std::string str = ptr->c_str();
        _editor->setText(common::utils_qt::toQString(str));
    }
}
