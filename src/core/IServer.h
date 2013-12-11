#pragma once

#include <QObject>

#include "core/ConnectionSettings.h"
#include "core/events/EventsInfo.hpp"
#include "core/IDriver.h"

namespace fastoredis
{
    class IServer;
    typedef boost::shared_ptr<IServer> IServerPtr;
    class IDatabase;
    typedef boost::shared_ptr<IDatabase> IDatabasePtr;

    class IServer
            : public QObject
    {
        Q_OBJECT
    public:
        typedef QObject base_class;
        typedef std::vector<IDatabasePtr> databases_cont_type;
        connectionTypes connectionType() const;
        QString name() const;
        IDriverPtr driver() const;
        QString address() const;

        //async
        void connect();
        void disconnect();
        void loadDatabases();
        void execute(const QString &script);

        //sync
        void stopCurrentEvent();
        bool isConnected() const;
        bool isMaster() const;
        databases_cont_type databases() const;

        virtual ~IServer();

    Q_SIGNALS: //only direct connections
        void startedConnect(const EventsInfo::ConnectInfoRequest &req);
        void finishedConnect(const EventsInfo::ConnectInfoResponce &res);

        void startedDisconnect(const EventsInfo::DisonnectInfoRequest &req);
        void finishedDisconnect(const EventsInfo::DisConnectInfoResponce &res);

        void startedExecute(const EventsInfo::ExecuteInfoRequest &req);
        void finishedExecute(const EventsInfo::ExecuteInfoResponce &res);

        void startedLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &req);
        void finishedLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &res);

        void progressChanged(const EventsInfo::ProgressResponceInfo &res);

    protected:
        static void syncServers(IServer *src, IServer *dsc);
        void notify(QEvent *ev);
        virtual void customEvent(QEvent *event);

        virtual void connectEvent(Events::ConnectResponceEvent *ev) = 0;
        virtual void disconnectEvent(Events::DisconnectResponceEvent *ev) = 0;
        virtual void executeEvent(Events::ExecuteResponceEvent *ev) = 0;
        virtual void loadDatabasesEvent(Events::LoadDatabasesInfoResponceEvent *ev) = 0;

        IServer(const IDriverPtr &drv);
        IServer(const IServerPtr &drv);

        databases_cont_type databases_;
        const bool _isMaster;
        const IDriverPtr _drv;
    };
}
