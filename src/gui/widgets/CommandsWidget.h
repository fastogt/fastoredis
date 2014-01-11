#pragma once

#include <QWidget>

class QTextEdit;
class QAction;

#include "common/log_levels.hpp"

namespace fastoredis
{
    class CommandsWidget : public QWidget
    {
        Q_OBJECT

    public:
        CommandsWidget(QWidget* parent = 0);

    public Q_SLOTS:
        void addCommand(const QString &command);

    private Q_SLOTS:
        void showContextMenu(const QPoint &pt);

    private:        
        QTextEdit *const _logTextEdit;
        QAction *_clear;
    };
}
