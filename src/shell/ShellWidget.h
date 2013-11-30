#pragma once
#include <QWidget>
#include "global/global.h"
QT_BEGIN_NAMESPACE
class QTextEdit;
class QAction;
QT_END_NAMESPACE

#include "core/IServer.h"

namespace fastoredis
{
    class ShellWidget
            : public QWidget
    {
        Q_OBJECT
    public:
        typedef QWidget base_class;
        ShellWidget(const IServerPtr &server, QWidget* parent = 0);

    private Q_SLOTS:
        void showContextMenu(const QPoint &pt);
        void execute();
        void stop();
        void connectToServer();
        void disconnectFromServer();

    private:
        const IServerPtr _server;
        QTextEdit *_input;
        QAction *_clear;
    };
}
