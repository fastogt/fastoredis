#include "gui/widgets/LogTabWidget.h"

#include <QTabBar>
#include <QEvent>

#include "common/macros.h"

#include "gui/widgets/LogWidget.h"
#include "gui/widgets/CommandsWidget.h"
#include "gui/GuiFactory.h"

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
        _log = new LogWidget(this);
        addTab(_log, GuiFactory::instance().loggingIcon(), "Errors");
        _commands = new CommandsWidget(this);
        addTab(_commands, GuiFactory::instance().commandIcon(), "Commands");
        retranslateUi();
    }

    void LogTabWidget::addLogMessage(const QString &message, common::logging::LEVEL_LOG level)
    {
        _log->addLogMessage(message, level);
    }

    void LogTabWidget::addCommand(const Command &command)
    {
        _commands->addCommand(command);
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
        setTabText(0,tr("Errors"));
        setTabText(1,tr("Commands"));
    }
}
