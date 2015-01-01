#pragma once

#include "core/idriver.h"

namespace fastoredis
{
    class IServer
            : public QObject
    {
        Q_OBJECT

    public:
        virtual ~IServer();

        connectionTypes connectionType() const;
        QString name() const;
        IDriverSPtr driver() const;
        QString address() const;
        QString outputDelemitr() const;

        //async
        void connect();
        void disconnect();
        void loadDatabases();
        void loadDatabaseContent(const DataBaseInfo& inf);
        void execute(const QString& script);
        void shutDown();
        void backupToPath(const QString& path);
        void exportFromPath(const QString& path);

        //sync
        void stopCurrentEvent();
        bool isConnected() const;

        bool isMaster() const;        
        void setIsMaster(bool isMaster);

        bool isLocalHost() const;

        virtual void syncWithServer(IServer* src) = 0;
        virtual void unSyncFromServer(IServer* src) = 0;

    Q_SIGNALS: //only direct connections
        void startedConnect(const EventsInfo::ConnectInfoRequest& req);
        void finishedConnect(const EventsInfo::ConnectInfoResponce& res);

        void startedDisconnect(const EventsInfo::DisonnectInfoRequest& req);
        void finishedDisconnect(const EventsInfo::DisConnectInfoResponce& res);

        void startedShutdown(const EventsInfo::ShutDownInfoRequest& req);
        void finishedShutdown(const EventsInfo::ShutDownInfoResponce& res);

        void startedBackup(const EventsInfo::BackupInfoRequest& req);
        void finishedBackup(const EventsInfo::BackupInfoResponce& res);

        void startedExport(const EventsInfo::ExportInfoRequest& req);
        void finishedExport(const EventsInfo::ExportInfoResponce& res);

        void startedExecute(const EventsInfo::ExecuteInfoRequest& req);

        void startedLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest& req);
        void finishedLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce& res);

        void startedLoadDataBaseContent(const EventsInfo::LoadDatabasesContentRequest& req);
        void finishedLoadDataBaseContent(const EventsInfo::LoadDatabasesContentResponce& res);

        void startedLoadServerInfo(const EventsInfo::ServerInfoRequest& req);
        void finishedLoadServerInfo(const EventsInfo::ServerInfoResponce& res);

        void startedLoadServerHistoryInfo(const EventsInfo::ServerInfoHistoryRequest& req);
        void finishedLoadServerHistoryInfo(const EventsInfo::ServerInfoHistoryResponce& res);

        void startedLoadServerProperty(const EventsInfo::ServerPropertyInfoRequest& req);
        void finishedLoadServerProperty(const EventsInfo::ServerPropertyInfoResponce& res);

        void startedChangeServerProperty(const EventsInfo::ChangeServerPropertyInfoRequest& req);
        void finishedChangeServerProperty(const EventsInfo::ChangeServerPropertyInfoResponce& res);

        void progressChanged(const EventsInfo::ProgressInfoResponce& res);

        void enteredMode(const EventsInfo::EnterModeInfo& res);
        void leavedMode(const EventsInfo::LeaveModeInfo& res);

        void rootCreated(const EventsInfo::CommandRootCreatedInfo& res);
        void rootCompleated(const EventsInfo::CommandRootCompleatedInfo& res);

   Q_SIGNALS:
        void addedChild(FastoObject *child);
        void itemUpdated(FastoObject* item, const QString& val);
        void serverInfoSnapShoot(ServerInfoSnapShoot shot);

    public Q_SLOTS:
        //async
        void serverInfo();
        void serverProperty();
        void requestHistoryInfo();
        void changeProperty(const PropertyType& newValue);

    protected:
        static void syncServers(IServer* src, IServer* dsc);
        static void unSyncServers(IServer* src, IServer* dsc);
        void notify(QEvent* ev);
        virtual void customEvent(QEvent* event);

        virtual void connectEvent(Events::ConnectResponceEvent* ev) = 0;
        virtual void disconnectEvent(Events::DisconnectResponceEvent* ev) = 0;
        virtual void loadDatabaseInfosEvent(Events::LoadDatabasesInfoResponceEvent* ev) = 0;
        virtual void loadDatabaseContentEvent(Events::LoadDatabaseContentResponceEvent* ev) = 0;
        virtual void loadServerInfoEvent(Events::ServerInfoResponceEvent* ev) = 0;
        virtual void loadServerPropertyEvent(Events::ServerPropertyInfoResponceEvent* ev) = 0;
        virtual void serverPropertyChangeEvent(Events::ChangeServerPropertyInfoResponceEvent* ev) = 0;
        virtual void handleShutdownEvent(Events::ShutDownResponceEvent* ev) = 0;
        virtual void handleBackupEvent(Events::BackupResponceEvent* ev) = 0;
        virtual void handleExportEvent(Events::ExportResponceEvent* ev) = 0;

        IServer(const IDriverSPtr& drv, bool isMaster);

        const IDriverSPtr drv_;

    private:
        void loadServerInfoHistoryEvent(Events::ServerInfoHistoryResponceEvent* ev);
        void processConfigArgs();
        bool isMaster_;
    };

    typedef boost::shared_ptr<IServer> IServerSPtr;
}
