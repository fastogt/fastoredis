#include "gui/widgets/commands_widget.h"

#include <QScrollBar>
#include <QTime>
#include <QMenu>
#include <QAction>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QEvent>

#include "common/qt/convert_string.h"

namespace fastoredis
{
    CommandsWidget::CommandsWidget(QWidget* parent)
        : QWidget(parent), logTextEdit_(new QTextEdit(this))
    {
        logTextEdit_->setReadOnly(true);
        logTextEdit_->setContextMenuPolicy(Qt::CustomContextMenu);
        VERIFY(connect(logTextEdit_, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint &))));
        QHBoxLayout *hlayout = new QHBoxLayout;
        hlayout->setContentsMargins(0, 0, 0, 0);
        hlayout->addWidget(logTextEdit_);
        clear_ = new QAction(this);
        VERIFY(connect(clear_, SIGNAL(triggered()),logTextEdit_, SLOT(clear())));
        setLayout(hlayout);
        retranslateUi();
    }

    void CommandsWidget::showContextMenu(const QPoint &pt)
    {
        QMenu *menu = logTextEdit_->createStandardContextMenu();
        menu->addAction(clear_);
        clear_->setEnabled(!logTextEdit_->toPlainText().isEmpty());

        menu->exec(logTextEdit_->mapToGlobal(pt));
        delete menu;
    }

    void CommandsWidget::addCommand(const Command &command)
    {
        QTime time = QTime::currentTime();
        logTextEdit_->setTextColor(command.type() == Command::InnerCommand ? QColor(Qt::gray):QColor(Qt::black));
        logTextEdit_->append(time.toString("h:mm:ss AP: ") + common::convertFromString16<QString>(command.message()));
        QScrollBar *sb = logTextEdit_->verticalScrollBar();
        sb->setValue(sb->maximum());
    }

    void CommandsWidget::changeEvent(QEvent *e)
    {
        if(e->type() == QEvent::LanguageChange){
            retranslateUi();
        }
        QWidget::changeEvent(e);
    }

    void CommandsWidget::retranslateUi()
    {
        clear_->setText(tr("Clear All"));
    }
}
