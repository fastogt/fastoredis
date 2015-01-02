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

    void RedisServer::handleConnectEvent(Events::ConnectResponceEvent* ev)
    {
        using namespace Events;
        ConnectResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedConnect(v);
    }

    void RedisServer::handleDisconnectEvent(Events::DisconnectResponceEvent* ev)
    {
        using namespace Events;
        DisconnectResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedDisconnect(v);
    }

    void RedisServer::handleLoadDatabaseInfosEvent(Events::LoadDatabasesInfoResponceEvent* ev)
    {
        using namespace Events;
        LoadDatabasesInfoResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedLoadDatabases(v);
    }

    void RedisServer::handleLoadDatabaseContentEvent(Events::LoadDatabaseContentResponceEvent* ev)
    {
        using namespace Events;
        LoadDatabaseContentResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedLoadDataBaseContent(v);
    }

    void RedisServer::handleLoadServerInfoEvent(Events::ServerInfoResponceEvent* ev)
    {
        using namespace Events;
        ServerInfoResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedLoadServerInfo(v);
    }

    void RedisServer::handleLoadServerPropertyEvent(Events::ServerPropertyInfoResponceEvent* ev)
    {
        using namespace Events;
        ServerPropertyInfoResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedLoadServerProperty(v);
    }

    void RedisServer::handleServerPropertyChangeEvent(Events::ChangeServerPropertyInfoResponceEvent* ev)
    {
        using namespace Events;
        ChangeServerPropertyInfoResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedChangeServerProperty(v);
    }

    void RedisServer::handleShutdownEvent(Events::ShutDownResponceEvent* ev)
    {
        using namespace Events;
        ShutDownResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedShutdown(v);
    }

    void RedisServer::handleBackupEvent(Events::BackupResponceEvent* ev)
    {
        using namespace Events;
        BackupResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedBackup(v);
    }

    void RedisServer::handleExportEvent(Events::ExportResponceEvent* ev)
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
