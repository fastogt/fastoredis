#pragma once

#include "core/iserver.h"

namespace fastoredis
{
    class RedisServer
            : public IServer
    {
        friend class ServersManager;
        Q_OBJECT
    public:
        virtual void syncWithServer(IServer* src);
        virtual void unSyncFromServer(IServer* src);

    protected:
        virtual void connectEvent(Events::ConnectResponceEvent* ev);
        virtual void disconnectEvent(Events::DisconnectResponceEvent* ev);
        virtual void loadDatabaseInfosEvent(Events::LoadDatabasesInfoResponceEvent* ev);
        virtual void loadDatabaseContentEvent(Events::LoadDatabaseContentResponceEvent* ev);
        virtual void loadServerInfoEvent(Events::ServerInfoResponceEvent* ev);
        virtual void loadServerPropertyEvent(Events::ServerPropertyInfoResponceEvent* ev);
        virtual void serverPropertyChangeEvent(Events::ChangeServerPropertyInfoResponceEvent* ev);
        virtual void handleShutdownEvent(Events::ShutDownResponceEvent* ev);
        virtual void handleBackupEvent(Events::BackupResponceEvent* ev);
        virtual void handleExportEvent(Events::ExportResponceEvent* ev);

    private:
        RedisServer(const IDriverSPtr& drv, bool isMaster);
    };
}
