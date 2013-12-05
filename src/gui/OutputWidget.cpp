#include "gui/OutputWidget.h"

#include <QHBoxLayout>
#include <QMenu>

#include "gui/FastoEditor.h"
#include "gui/FastoTableView.h"
#include "gui/FastoTreeView.h"
#include "gui/FastoTreeModel.h"
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
        _textEditor->clear();
        FastoObjectPtr ptr = res._out;
        std::string str = toStdString(ptr);
        _textEditor->setText(common::utils_qt::toQString(str));
    }
}
