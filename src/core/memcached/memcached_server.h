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
        virtual void handleConnectEvent(Events::ConnectResponceEvent* ev);
        virtual void handleDisconnectEvent(Events::DisconnectResponceEvent* ev);
        virtual void handleLoadDatabaseInfosEvent(Events::LoadDatabasesInfoResponceEvent* ev);
        virtual void handleLoadDatabaseContentEvent(Events::LoadDatabaseContentResponceEvent* ev);
        virtual void handleLoadServerInfoEvent(Events::ServerInfoResponceEvent* ev);
        virtual void handleLoadServerPropertyEvent(Events::ServerPropertyInfoResponceEvent* ev);
        virtual void handleServerPropertyChangeEvent(Events::ChangeServerPropertyInfoResponceEvent* ev);
        virtual void handleShutdownEvent(Events::ShutDownResponceEvent* ev);
        virtual void handleBackupEvent(Events::BackupResponceEvent* ev);
        virtual void handleExportEvent(Events::ExportResponceEvent* ev);

    private:
        MemcachedServer(const IDriverSPtr& drv, bool isMaster);
    };
}
