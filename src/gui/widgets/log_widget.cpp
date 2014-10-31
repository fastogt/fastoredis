#include "gui/widgets/log_widget.h"

#include <QHBoxLayout>
#include <QScrollBar>
#include <QMenu>
#include <QTime>
#include <QAction>
#include <QPlainTextEdit>
#include "common/macros.h"

namespace fastoredis
{
    LogWidget::LogWidget(QWidget* parent) 
        : QWidget(parent), logTextEdit_(new QTextEdit(this))
    {
        logTextEdit_->setReadOnly(true);
        logTextEdit_->setContextMenuPolicy(Qt::CustomContextMenu);
        VERIFY(connect(logTextEdit_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenu(const QPoint &))));
        QHBoxLayout *hlayout = new QHBoxLayout;
        hlayout->setContentsMargins(0,0,0,0);
        hlayout->addWidget(logTextEdit_);
        clear_ = new QAction(this);
        VERIFY(connect(clear_, SIGNAL(triggered()),logTextEdit_, SLOT(clear())));
        setLayout(hlayout);
        retranslateUi();
    }

    void LogWidget::showContextMenu(const QPoint& pt)
    {
        QMenu *menu = logTextEdit_->createStandardContextMenu();
        menu->addAction(clear_);
        clear_->setEnabled(!logTextEdit_->toPlainText().isEmpty());

        menu->exec(logTextEdit_->mapToGlobal(pt));
        delete menu;
    }

    void LogWidget::addLogMessage(const QString& message, common::logging::LEVEL_LOG level)
    {
        QTime time = QTime::currentTime();
        logTextEdit_->setTextColor(level == common::logging::L_CRITICAL ? QColor(Qt::red):QColor(Qt::black));
        logTextEdit_->append(time.toString("h:mm:ss AP: ") + message);
        QScrollBar *sb = logTextEdit_->verticalScrollBar();
        sb->setValue(sb->maximum());
    }

    void LogWidget::changeEvent(QEvent *e)
    {
        if(e->type() == QEvent::LanguageChange){
            retranslateUi();
        }
        QWidget::changeEvent(e);
    }

    void LogWidget::retranslateUi()
    {
        clear_->setText(tr("Clear All"));
    }
}
