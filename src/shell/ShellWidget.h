#pragma once
#include <QWidget>
#include "global/global.h"
QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE

#include "core/IServer.h"

class QsciScintilla;

namespace fastoredis
{
    class ShellWidget
            : public QWidget
    {
        Q_OBJECT
    public:
        typedef QWidget base_class;
        ShellWidget(const IServerPtr &server, QWidget* parent = 0);

        QString text() const;
        void setText(const QString &text);

        void reload();
        static ShellWidget *duplicate(ShellWidget *src, const QString &text);

    private Q_SLOTS:
        void showContextMenu(const QPoint &pt);
        void execute();
        void stop();
        void connectToServer();
        void disconnectFromServer();

    private:
        const IServerPtr _server;
        QsciScintilla *_input;
        QAction *_clear;
    };
}
