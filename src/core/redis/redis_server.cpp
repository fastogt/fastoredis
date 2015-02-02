#include "core/redis/redis_server.h"

#include "core/logger.h"

namespace fastoredis
{
    RedisServer::RedisServer(const IDriverSPtr& drv, bool isMaster)
        : IServer(drv,isMaster)
    {

    }

    void RedisServer::syncWithServer(IServer *src)
    {
        IServer::syncServers(src, this);
    }

    void RedisServer::unSyncFromServer(IServer *src)
    {
        IServer::unSyncServers(src, this);
    }

    void RedisServer::handleConnectEvent(events::ConnectResponceEvent* ev)
    {
        using namespace events;
        ConnectResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedConnect(v);
    }

    void RedisServer::handleDisconnectEvent(events::DisconnectResponceEvent* ev)
    {
        using namespace events;
        DisconnectResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedDisconnect(v);
    }

    void RedisServer::handleLoadDatabaseInfosEvent(events::LoadDatabasesInfoResponceEvent* ev)
    {
        using namespace events;
        LoadDatabasesInfoResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedLoadDatabases(v);
    }

    void RedisServer::handleLoadDatabaseContentEvent(events::LoadDatabaseContentResponceEvent* ev)
    {
        using namespace events;
        LoadDatabaseContentResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedLoadDataBaseContent(v);
    }

    void RedisServer::handleSetDefaultDatabaseEvent(events::SetDefaultDatabaseResponceEvent* ev)
    {
        using namespace events;
        SetDefaultDatabaseResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedSetDefaultDatabase(v);
    }

    void RedisServer::handleLoadServerInfoEvent(events::ServerInfoResponceEvent* ev)
    {
        using namespace events;
        ServerInfoResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedLoadServerInfo(v);
    }

    void RedisServer::handleLoadServerPropertyEvent(events::ServerPropertyInfoResponceEvent* ev)
    {
        using namespace events;
        ServerPropertyInfoResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedLoadServerProperty(v);
    }

    void RedisServer::handleServerPropertyChangeEvent(events::ChangeServerPropertyInfoResponceEvent* ev)
    {
        using namespace events;
        ChangeServerPropertyInfoResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedChangeServerProperty(v);
    }

    void RedisServer::handleChangeDbValueEvent(events::ChangeDbValueResponceEvent* ev)
    {
        using namespace events;
        ChangeDbValueResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }

        emit finishedChangeDbValue(v);
    }

    void RedisServer::handleShutdownEvent(events::ShutDownResponceEvent* ev)
    {
        using namespace events;
        ShutDownResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedShutdown(v);
    }

    void RedisServer::handleBackupEvent(events::BackupResponceEvent* ev)
    {
        using namespace events;
        BackupResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedBackup(v);
    }

    void RedisServer::handleExportEvent(events::ExportResponceEvent* ev)
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
