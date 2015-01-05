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

    void MemcachedServer::handleConnectEvent(Events::ConnectResponceEvent* ev)
    {
        using namespace Events;
        ConnectResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedConnect(v);
    }

    void MemcachedServer::handleDisconnectEvent(Events::DisconnectResponceEvent* ev)
    {
        using namespace Events;
        DisconnectResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedDisconnect(v);
    }

    void MemcachedServer::handleLoadDatabaseInfosEvent(Events::LoadDatabasesInfoResponceEvent* ev)
    {
        using namespace Events;
        LoadDatabasesInfoResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedLoadDatabases(v);
    }

    void MemcachedServer::handleLoadDatabaseContentEvent(Events::LoadDatabaseContentResponceEvent* ev)
    {
        using namespace Events;
        LoadDatabaseContentResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedLoadDataBaseContent(v);
    }

    void MemcachedServer::handleLoadServerInfoEvent(Events::ServerInfoResponceEvent* ev)
    {
        using namespace Events;
        ServerInfoResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedLoadServerInfo(v);
    }

    void MemcachedServer::handleLoadServerPropertyEvent(Events::ServerPropertyInfoResponceEvent* ev)
    {
        using namespace Events;
        ServerPropertyInfoResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedLoadServerProperty(v);
    }

    void MemcachedServer::handleServerPropertyChangeEvent(Events::ChangeServerPropertyInfoResponceEvent* ev)
    {
        using namespace Events;
        ChangeServerPropertyInfoResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedChangeServerProperty(v);
    }

    void MemcachedServer::handleShutdownEvent(Events::ShutDownResponceEvent* ev)
    {
        using namespace Events;
        ShutDownResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedShutdown(v);
    }

    void MemcachedServer::handleBackupEvent(Events::BackupResponceEvent* ev)
    {
        using namespace Events;
        BackupResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedBackup(v);
    }

    void MemcachedServer::handleExportEvent(Events::ExportResponceEvent* ev)
    {
        using namespace Events;
        ExportResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedExport(v);
    }
}
