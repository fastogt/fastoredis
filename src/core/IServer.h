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
        typedef QObject base_class;
        connectionTypes connectionType() const;
        QString name() const;
        IDriverPtr driver() const;

        //async
        void connect();
        void disconnect();

        void execute(const QString &script);

        //sync
        void stopCurrentEvent();

        virtual ~IServer();

    Q_SIGNALS: //only direct connections
        void startedConnect(const EventsInfo::ConnectInfoRequest &req);
        void finishedConnect(const EventsInfo::ConnectInfoResponce &res);

        void startedDisconnect(const EventsInfo::DisonnectInfoRequest &req);
        void finishedDisconnect(const EventsInfo::DisConnectInfoResponce &res);

        void startedExecute(const EventsInfo::ExecuteInfoRequest &req);
        void finishedExecute(const EventsInfo::ExecuteInfoResponce &res);

        void startedInterupt(const EventsInfo::InteruptInfoRequest &req);
        void finishedInterupt(const EventsInfo::InteruptInfoResponce &res);
    protected:
        void notify(QEvent *ev);
        virtual void customEvent(QEvent *event);
        IServer(const IDriverPtr &drv);
        IServer(const IServerPtr &drv);
        const IDriverPtr _drv;
    };
}
