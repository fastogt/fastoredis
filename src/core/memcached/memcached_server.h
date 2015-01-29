#pragma once

#include "core/iserver.h"

namespace fastoredis
{
    class MemcachedServer
            : public IServer
    {
        friend class ServersManager;
        Q_OBJECT
    public:
        virtual void syncWithServer(IServer* src);
        virtual void unSyncFromServer(IServer* src);

    protected:
        virtual void handleConnectEvent(events::ConnectResponceEvent* ev);
        virtual void handleDisconnectEvent(events::DisconnectResponceEvent* ev);
        virtual void handleLoadDatabaseInfosEvent(events::LoadDatabasesInfoResponceEvent* ev);
        virtual void handleLoadServerInfoEvent(events::ServerInfoResponceEvent* ev);
        virtual void handleLoadServerPropertyEvent(events::ServerPropertyInfoResponceEvent* ev);
        virtual void handleServerPropertyChangeEvent(events::ChangeServerPropertyInfoResponceEvent* ev);
        virtual void handleShutdownEvent(events::ShutDownResponceEvent* ev);
        virtual void handleBackupEvent(events::BackupResponceEvent* ev);
        virtual void handleExportEvent(events::ExportResponceEvent* ev);

// ============== database =============//
        virtual void handleLoadDatabaseContentEvent(events::LoadDatabaseContentResponceEvent* ev);
        virtual void handleSetDefaultDatabaseEvent(events::SetDefaultDatabaseResponceEvent* ev);
// ============== database =============//
    private:
        MemcachedServer(const IDriverSPtr& drv, bool isMaster);
    };
}
