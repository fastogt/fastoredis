#pragma once

/**/

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
        QueryWidget(IServerPtr server, QWidget* parent = 0);

        QueryWidget* clone(const QString &text);
        QString inputText() const;
        void reload();

    private:
        RedisShellWidget* RedisShellWidget_;
        OutputWidget* outputWidget_;
    };
}
