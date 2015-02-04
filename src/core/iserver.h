#pragma once

#include "core/core_fwd.h"

#include "core/events/events.h"

namespace fastoredis
{
    class IServer
            : public QObject, public std::enable_shared_from_this<IServer>
    {
        Q_OBJECT

    public:
        typedef std::vector<IDatabaseSPtr> databases_container_t;

        virtual ~IServer();

        connectionTypes type() const;
        QString name() const;
        IDriverSPtr driver() const;
        QString address() const;
        QString outputDelemitr() const;
        IDatabaseSPtr findDatabaseByInfo(DataBaseInfoSPtr inf) const;
        IDatabaseSPtr findDatabaseByName(const std::string& name) const;

        //async
        void connect();
        void disconnect();
        void loadDatabases();
        void loadDatabaseContent(DataBaseInfoSPtr inf);
        void setDefaultDb(DataBaseInfoSPtr inf);
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

        void startedLoadServerInfo(const EventsInfo::ServerInfoRequest& req);
        void finishedLoadServerInfo(const EventsInfo::ServerInfoResponce& res);

        void startedLoadServerHistoryInfo(const EventsInfo::ServerInfoHistoryRequest& req);
        void finishedLoadServerHistoryInfo(const EventsInfo::ServerInfoHistoryResponce& res);

        void startedLoadServerProperty(const EventsInfo::ServerPropertyInfoRequest& req);
        void finishedLoadServerProperty(const EventsInfo::ServerPropertyInfoResponce& res);

// ============== change =============//
        void startedChangeDbValue(const EventsInfo::ChangeDbValueRequest& req);
        void finishedChangeDbValue(const EventsInfo::ChangeDbValueResponce& res);

        void startedChangeServerProperty(const EventsInfo::ChangeServerPropertyInfoRequest& req);
        void finishedChangeServerProperty(const EventsInfo::ChangeServerPropertyInfoResponce& res);
// ============== change =============//

        void progressChanged(const EventsInfo::ProgressInfoResponce& res);

        void enteredMode(const EventsInfo::EnterModeInfo& res);
        void leavedMode(const EventsInfo::LeaveModeInfo& res);

        void rootCreated(const EventsInfo::CommandRootCreatedInfo& res);
        void rootCompleated(const EventsInfo::CommandRootCompleatedInfo& res);

// ============== database =============//
        void startedLoadDataBaseContent(const EventsInfo::LoadDatabaseContentRequest& req);
        void finishedLoadDatabaseContent(const EventsInfo::LoadDatabaseContentResponce& res);

        void startedSetDefaultDatabase(const EventsInfo::SetDefaultDatabaseRequest& req);
        void finishedSetDefaultDatabase(const EventsInfo::SetDefaultDatabaseResponce& res);
// ============== database =============//
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
        void changeValue(const DbValue& newValue, const std::string &command);

    protected:
        virtual IDatabaseSPtr createDatabaseImpl(DataBaseInfoSPtr info) = 0;
        static void syncServers(IServer* src, IServer* dsc);
        static void unSyncServers(IServer* src, IServer* dsc);
        void notify(QEvent* ev);
        virtual void customEvent(QEvent* event);

        virtual void handleConnectEvent(events::ConnectResponceEvent* ev);
        virtual void handleDisconnectEvent(events::DisconnectResponceEvent* ev);
        virtual void handleLoadDatabaseInfosEvent(events::LoadDatabasesInfoResponceEvent* ev);
        virtual void handleLoadServerInfoEvent(events::ServerInfoResponceEvent* ev);
        virtual void handleLoadServerPropertyEvent(events::ServerPropertyInfoResponceEvent* ev);
        virtual void handleServerPropertyChangeEvent(events::ChangeServerPropertyInfoResponceEvent* ev);
        virtual void handleChangeDbValueEvent(events::ChangeDbValueResponceEvent* ev);
        virtual void handleShutdownEvent(events::ShutDownResponceEvent* ev);
        virtual void handleBackupEvent(events::BackupResponceEvent* ev);
        virtual void handleExportEvent(events::ExportResponceEvent* ev);

// ============== database =============//
        virtual void handleLoadDatabaseContentEvent(events::LoadDatabaseContentResponceEvent* ev);
        virtual void handleSetDefaultDatabaseEvent(events::SetDefaultDatabaseResponceEvent* ev);
// ============== database =============//

        IServer(const IDriverSPtr& drv, bool isMaster);

        const IDriverSPtr drv_;
        databases_container_t databases_;

    private:
        void handleLoadServerInfoHistoryEvent(events::ServerInfoHistoryResponceEvent* ev);
        void processConfigArgs();
        bool isMaster_;
    };
}
