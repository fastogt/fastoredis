#include "gui/widgets/LogTabWidget.h"

#include <QTabBar>

#include "common/macros.h"
#include "gui/widgets/LogWidget.h"
#include "gui/widgets/CommandsWidget.h"

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
        addTab(_log, "Errors");
        _commands = new CommandsWidget(this);
        addTab(_commands, "Commands");
    }

    void LogTabWidget::addLogMessage(const QString &message, common::logging::LEVEL_LOG level)
    {
        _log->addLogMessage(message, level);
    }

    void LogTabWidget::addCommand(const QString &command)
    {
        _commands->addCommand(command);
    }
}
