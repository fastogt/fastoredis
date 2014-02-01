#include "core/redis/RedisServer.h"

#include "core/ConnectionSettings.h"
#include "common/qt/converter_patterns.h"
#include "core/Logger.h"

namespace fastoredis
{
    RedisServer::RedisServer(const IDriverPtr &drv, bool isMaster)
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

    void RedisServer::connectEvent(Events::ConnectResponceEvent *ev)
    {
        using namespace Events;
        ConnectResponceEvent::value_type v = ev->value();
        const common::ErrorValue &er = v.errorInfo();
        if(er.isError()){
            LOG_ERROR(er, true);
        }
        emit finishedConnect(v);
    }

    void RedisServer::disconnectEvent(Events::DisconnectResponceEvent *ev)
    {
        using namespace Events;
        DisconnectResponceEvent::value_type v = ev->value();
        const common::ErrorValue &er = v.errorInfo();
        if(er.isError()){
            LOG_ERROR(er, true);
        }
        emit finishedDisconnect(v);
    }

    void RedisServer::executeEvent(Events::ExecuteResponceEvent *ev)
    {
        using namespace Events;
        ExecuteResponceEvent::value_type v = ev->value();
        const common::ErrorValue &er = v.errorInfo();
        if(er.isError()){
            LOG_ERROR(er, true);
        }
        emit finishedExecute(v);
    }

    void RedisServer::loadDatabasesInfoEvent(Events::LoadDatabasesInfoResponceEvent *ev)
    {
        using namespace Events;
        LoadDatabasesInfoResponceEvent::value_type v = ev->value();
        const common::ErrorValue &er = v.errorInfo();
        if(er.isError()){
            LOG_ERROR(er, true);
        }
        emit finishedLoadDatabases(v);
    }

    void RedisServer::loadDatabaseContentEvent(Events::LoadDatabaseContentResponceEvent *ev)
    {
        using namespace Events;
        LoadDatabaseContentResponceEvent::value_type v = ev->value();
        const common::ErrorValue &er = v.errorInfo();
        if(er.isError()){
            LOG_ERROR(er, true);
        }
        emit finishedLoadDataBaseContent(v);
    }

    void RedisServer::serverInfoEvent(Events::ServerInfoResponceEvent *ev)
    {
        using namespace Events;
        ServerInfoResponceEvent::value_type v = ev->value();
        const common::ErrorValue &er = v.errorInfo();
        if(er.isError()){
            LOG_ERROR(er, true);
        }
        emit finishedServerInfo(v);
    }

    void RedisServer::serverPropertyEvent(Events::ServerPropertyResponceEvent *ev)
    {
        using namespace Events;
        ServerPropertyResponceEvent::value_type v = ev->value();
        const common::ErrorValue &er = v.errorInfo();
        if(er.isError()){
            LOG_ERROR(er, true);
        }
        emit finishedServerProperty(v);
    }

    void RedisServer::serverPropertyChangeEvent(Events::ServerPropertyChangeResponceEvent *ev)
    {
        using namespace Events;
        ServerPropertyChangeResponceEvent::value_type v = ev->value();
        const common::ErrorValue &er = v.errorInfo();
        if(er.isError()){
            LOG_ERROR(er, true);
        }
        emit finishedServerChangeProperty(v);
    }
}
