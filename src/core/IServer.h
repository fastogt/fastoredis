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
        connectionTypes connectionType() const;
        QString name() const;
        IDriverPtr driver() const;
        QString address() const;

        //async
        void connect();
        void disconnect();
        void loadDatabases();
        void loadDatabaseContent(const DataBaseInfo &inf);
        void execute(const QString &script);

        //sync
        void stopCurrentEvent();
        bool isConnected() const;

        bool isMaster() const;
        void setIsMaster(bool isMaster);

        virtual void syncWithServer(IServer *src) = 0;
        virtual void unSyncFromServer(IServer *src) = 0;

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

        void startedLoadDataBaseContent(const EventsInfo::LoadDatabasesContentRequest &req);
        void finishedLoadDataBaseContent(const EventsInfo::LoadDatabasesContentResponce &res);

        void startedServerInfo(const EventsInfo::ServerInfoRequest &req);
        void finishedServerInfo(const EventsInfo::ServerInfoResponce &res);

        void startedServerProperty(const EventsInfo::ServerPropertyRequest &req);
        void finishedServerProperty(const EventsInfo::ServerPropertyResponce &res);

        void startedServerChangeProperty(const EventsInfo::ServerPropertyChangeRequest &req);
        void finishedServerChangeProperty(const EventsInfo::ServerPropertyChangeResponce &res);

        void progressChanged(const EventsInfo::ProgressResponceInfo &res);

    public Q_SLOTS:
        //async
        void serverInfo();
        void serverProperty();
        void changeProperty(const PropertyType& newValue);

    protected:
        static void syncServers(IServer *src, IServer *dsc);
        static void unSyncServers(IServer *src, IServer *dsc);
        void notify(QEvent *ev);
        virtual void customEvent(QEvent *event);

        virtual void connectEvent(Events::ConnectResponceEvent *ev) = 0;
        virtual void disconnectEvent(Events::DisconnectResponceEvent *ev) = 0;
        virtual void executeEvent(Events::ExecuteResponceEvent *ev) = 0;
        virtual void loadDatabasesInfoEvent(Events::LoadDatabasesInfoResponceEvent *ev) = 0;
        virtual void loadDatabaseContentEvent(Events::LoadDatabaseContentResponceEvent *ev) = 0;
        virtual void serverInfoEvent(Events::ServerInfoResponceEvent *ev) = 0;
        virtual void serverPropertyEvent(Events::ServerPropertyResponceEvent *ev) = 0;
        virtual void serverPropertyChangeEvent(Events::ServerPropertyChangeResponceEvent *ev) = 0;

        IServer(const IDriverPtr &drv, bool isMaster);

        const IDriverPtr _drv;

    private:
        bool _isMaster;
    };
}
