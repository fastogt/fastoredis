#pragma once

#include <QObject>

#include "core/connection_settings.h"
#include "core/events/events_info.h"
#include "core/idriver.h"

namespace fastoredis
{
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
        void requestHistoryInfo();

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

        void startedLoadServerInfo(const EventsInfo::ServerInfoRequest &req);
        void finishedLoadServerInfo(const EventsInfo::ServerInfoResponce &res);

        void startedLoadServerHistoryInfo(const EventsInfo::ServerInfoHistoryRequest &req);
        void finishedLoadServerHistoryInfo(const EventsInfo::ServerInfoHistoryResponce &res);

        void startedLoadServerProperty(const EventsInfo::ServerPropertyInfoRequest &req);
        void finishedLoadServerProperty(const EventsInfo::ServerPropertyInfoResponce &res);

        void startedChangeServerProperty(const EventsInfo::ChangeServerPropertyInfoRequest &req);
        void finishedChangeServerProperty(const EventsInfo::ChangeServerPropertyInfoResponce &res);

        void progressChanged(const EventsInfo::ProgressInfoResponce &res);

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
        virtual void loadDatabaseInfosEvent(Events::LoadDatabasesInfoResponceEvent *ev) = 0;
        virtual void loadDatabaseContentEvent(Events::LoadDatabaseContentResponceEvent *ev) = 0;
        virtual void loadServerInfoEvent(Events::ServerInfoResponceEvent *ev) = 0;
        virtual void loadServerPropertyEvent(Events::ServerPropertyInfoResponceEvent *ev) = 0;
        virtual void serverPropertyChangeEvent(Events::ChangeServerPropertyInfoResponceEvent *ev) = 0;

        IServer(const IDriverPtr &drv, bool isMaster);

        const IDriverPtr _drv;

    private:
        void loadServerInfoHistoryEvent(Events::ServerInfoHistoryResponceEvent *ev);
        bool _isMaster;
    };

    typedef boost::shared_ptr<IServer> IServerPtr;
}
