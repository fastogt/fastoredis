#include "shell/ShellWidget.h"

#include <QHBoxLayout>
#include <QMenu>
#include <QAction>
#include <QPlainTextEdit>
#include <QTextEdit>

#include "common/macros.h"

namespace fastoredis
{
    ShellWidget::ShellWidget(QWidget *parent)
        : base_class(parent)
    {
        _input  = new QTextEdit;
        _input->setContextMenuPolicy(Qt::CustomContextMenu);
        VERIFY(connect(_input,SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint &))));
        QHBoxLayout *hlayout = new QHBoxLayout(this);
        hlayout->setContentsMargins(0,0,0,0);
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
}
