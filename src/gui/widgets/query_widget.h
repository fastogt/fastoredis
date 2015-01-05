#pragma once

#include <QWidget>

#include "core/iserver.h"

class QAction;

namespace fastoredis
{
    class RedisShellWidget;
    class OutputWidget;

    class QueryWidget
            : public QWidget
    {
        Q_OBJECT
    public:
        QueryWidget(IServerSPtr server, QWidget* parent = 0);

        QueryWidget* clone(const QString &text);
        connectionTypes connectionType() const;
        QString inputText() const;
        void reload();

    private:
        RedisShellWidget* shellWidget_;
        OutputWidget* outputWidget_;
    };
}
