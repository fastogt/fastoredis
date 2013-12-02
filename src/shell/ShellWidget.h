#pragma once
#include <QWidget>
#include "global/global.h"
QT_BEGIN_NAMESPACE
class QAction;
class QProgressBar;
QT_END_NAMESPACE

#include "core/IServer.h"
#include "core/events/EventsInfo.hpp"

namespace fastoredis
{
    class RedisShell;
    class ShellWidget
            : public QWidget
    {
        Q_OBJECT
    public:
        typedef QWidget base_class;
        ShellWidget(const IServerPtr &server, QWidget* parent = 0);

        void reload();
        static ShellWidget *duplicate(ShellWidget *src, const QString &text);

        QString text() const;

    private Q_SLOTS:
        void execute();
        void stop();
        void connectToServer();
        void disconnectFromServer();

        void startConnect(const EventsInfo::ConnectInfoRequest &req);
        void finishConnect(const EventsInfo::ConnectInfoResponce &res);
        void startDisconnect(const EventsInfo::DisonnectInfoRequest &req);
        void finishDisconnect(const EventsInfo::DisConnectInfoResponce &res);

        void progressChange(const EventsInfo::ProgressResponceInfo &res);
        void showAutocompletion(const QString &prefix);

    private:
        void syncConnectionActions();

        const IServerPtr _server;
        QAction *_connectAction;
        QAction *_disConnectAction;
        RedisShell *_input;
        QProgressBar *_workProgressBar;
    };
}
