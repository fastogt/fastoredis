#pragma once

#include <QWidget>
QT_BEGIN_NAMESPACE
class QTextEdit;
class QAction;
QT_END_NAMESPACE

#include "global/global.h"
#include "common/log_levels.hpp"

namespace fastoredis
{
    class LogWidget : public QWidget
    {
        Q_OBJECT

    public:
        typedef QWidget BaseClass;
        LogWidget(QWidget* parent = 0);        

    public Q_SLOTS:
        void addMessage(const QString &message, common::logging::LEVEL_LOG level);

    private Q_SLOTS:
        void showContextMenu(const QPoint &pt);

    private:        
        QTextEdit *const _logTextEdit;
        QAction *_clear;
    };
}
