#pragma once

/**/

#include <QWidget>

#include "core/iserver.h"

class QAction;

namespace fastoredis
{
    class ShellWidget;
    class OutputWidget;

    class QueryWidget
            : public QWidget
    {
        Q_OBJECT
    public:
        QueryWidget(IServerPtr server, QWidget* parent = 0);

        static QueryWidget *duplicate(QueryWidget *src, const QString &text);
        QString inputText() const;
        void reload();

    private:
        ShellWidget *shellWidget_;
        OutputWidget *outputWidget_;
    };
}
