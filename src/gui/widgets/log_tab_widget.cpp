#include "gui/widgets/log_tab_widget.h"

#include <QTabBar>
#include <QEvent>

#include "common/macros.h"

#include "gui/widgets/log_widget.h"
#include "gui/widgets/commands_widget.h"
#include "gui/gui_factory.h"

namespace fastoredis
{
    LogTabWidget::LogTabWidget(QWidget* parent)
        : QTabWidget(parent)
    {
        QTabBar *tab = new QTabBar(this);
        setTabBar(tab);
        setTabsClosable(false);
        setElideMode(Qt::ElideRight);
        setMovable(true);
        setDocumentMode(true);
        log_ = new LogWidget(this);
        addTab(log_, GuiFactory::instance().loggingIcon(), "Errors");
        commands_ = new CommandsWidget(this);
        addTab(commands_, GuiFactory::instance().commandIcon(), "Commands");
        retranslateUi();
    }

    void LogTabWidget::addLogMessage(const QString &message, common::logging::LEVEL_LOG level)
    {
        log_->addLogMessage(message, level);
    }

    void LogTabWidget::addCommand(const Command &command)
    {
        commands_->addCommand(command);
    }

    void LogTabWidget::changeEvent(QEvent *e)
    {
        if(e->type() == QEvent::LanguageChange){
            retranslateUi();
        }
        QTabWidget::changeEvent(e);
    }

    void LogTabWidget::retranslateUi()
    {
        setTabText(0, tr("Errors"));
        setTabText(1, tr("Commands"));
    }
}
