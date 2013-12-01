#pragma once
#include <QWidget>
#include "global/global.h"
QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE

#include "core/IServer.h"
#include "core/events/EventsInfo.hpp"

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

        void startConnect(const EventsInfo::ConnectInfoRequest &req);
        void finishConnect(const EventsInfo::ConnectInfoResponce &res);
        void startDisconnect(const EventsInfo::DisonnectInfoRequest &req);
        void finishDisconnect(const EventsInfo::DisConnectInfoResponce &res);

    private:
        void syncConnectionActions(bool isConnectAct);

        const IServerPtr _server;
        QAction *_connectAction;
        QAction *_disConnectAction;
        QsciScintilla *_input;
        QAction *_clear;
    };
}
