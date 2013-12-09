#pragma once

#include <QObject>

#include "core/ConnectionSettings.h"
#include "core/events/EventsInfo.hpp"
#include "core/IDriver.h"

namespace fastoredis
{
    class IServer;
    typedef boost::shared_ptr<IServer> IServerPtr;

    class IServer
            : public QObject
    {
        Q_OBJECT
    public:
        struct ServerInfo
        {

        };

        typedef QObject base_class;
        connectionTypes connectionType() const;
        QString name() const;
        IDriverPtr driver() const;
        QString address() const;

        //async
        void connect();
        void disconnect();
        void execute(const QString &script);

        //sync
        void stopCurrentEvent();
        bool isConnected() const;

        virtual ~IServer();

    Q_SIGNALS: //only direct connections
        void startedConnect(const EventsInfo::ConnectInfoRequest &req);
        void finishedConnect(const EventsInfo::ConnectInfoResponce &res);

        void startedDisconnect(const EventsInfo::DisonnectInfoRequest &req);
        void finishedDisconnect(const EventsInfo::DisConnectInfoResponce &res);

        void startedExecute(const EventsInfo::ExecuteInfoRequest &req);
        void finishedExecute(const EventsInfo::ExecuteInfoResponce &res);

        void progressChanged(const EventsInfo::ProgressResponceInfo &res);

    protected:
        static void syncServers(IServer *src, IServer *dsc);
        void notify(QEvent *ev);
        virtual void customEvent(QEvent *event);
        IServer(const IDriverPtr &drv);
        IServer(const IServerPtr &drv);
        const IDriverPtr _drv;
    };
}
