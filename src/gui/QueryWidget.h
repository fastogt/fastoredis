#pragma once

#include <QWidget>
QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE

#include "global/global.h"
#include "core/IServer.h"

namespace fastoredis
{
    class ShellWidget;
    class OutputWidget;

    class QueryWidget
            : public QWidget
    {
        Q_OBJECT

    public:
        typedef QWidget base_class;
        QueryWidget(const IServerPtr &server, QWidget* parent = 0);

        static QueryWidget *duplicate(QueryWidget *src, const QString &text);
        QString inputText() const;
        void reload();

    public Q_SLOTS:

    private:
        ShellWidget *_shellWidget;
        OutputWidget *_outputWidget;
    };
}
