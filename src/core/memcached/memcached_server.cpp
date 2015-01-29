#include "core/memcached/memcached_server.h"

#include "core/logger.h"

namespace fastoredis
{
    MemcachedServer::MemcachedServer(const IDriverSPtr& drv, bool isMaster)
        : IServer(drv,isMaster)
    {

    }

    void MemcachedServer::syncWithServer(IServer *src)
    {
        IServer::syncServers(src, this);
    }

    void MemcachedServer::unSyncFromServer(IServer *src)
    {
        IServer::unSyncServers(src, this);
    }

    void MemcachedServer::handleConnectEvent(events::ConnectResponceEvent* ev)
    {
        using namespace events;
        ConnectResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedConnect(v);
    }

    void MemcachedServer::handleDisconnectEvent(events::DisconnectResponceEvent* ev)
    {
        using namespace events;
        DisconnectResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedDisconnect(v);
    }

    void MemcachedServer::handleLoadDatabaseInfosEvent(events::LoadDatabasesInfoResponceEvent* ev)
    {
        using namespace events;
        LoadDatabasesInfoResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedLoadDatabases(v);
    }

    void MemcachedServer::handleLoadDatabaseContentEvent(events::LoadDatabaseContentResponceEvent* ev)
    {
        using namespace events;
        LoadDatabaseContentResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedLoadDataBaseContent(v);
    }

    void MemcachedServer::handleSetDefaultDatabaseEvent(events::SetDefaultDatabaseResponceEvent* ev)
    {
        using namespace events;
        SetDefaultDatabaseResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedSetDefaultDatabase(v);
    }

    void MemcachedServer::handleLoadServerInfoEvent(events::ServerInfoResponceEvent* ev)
    {
        using namespace events;
        ServerInfoResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedLoadServerInfo(v);
    }

    void MemcachedServer::handleLoadServerPropertyEvent(events::ServerPropertyInfoResponceEvent* ev)
    {
        using namespace events;
        ServerPropertyInfoResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedLoadServerProperty(v);
    }

    void MemcachedServer::handleServerPropertyChangeEvent(events::ChangeServerPropertyInfoResponceEvent* ev)
    {
        using namespace events;
        ChangeServerPropertyInfoResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedChangeServerProperty(v);
    }

    void MemcachedServer::handleShutdownEvent(events::ShutDownResponceEvent* ev)
    {
        using namespace events;
        ShutDownResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedShutdown(v);
    }

    void MemcachedServer::handleBackupEvent(events::BackupResponceEvent* ev)
    {
        using namespace events;
        BackupResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedBackup(v);
    }

    void MemcachedServer::handleExportEvent(events::ExportResponceEvent* ev)
    {
        using namespace events;
        ExportResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedExport(v);
    }
}
