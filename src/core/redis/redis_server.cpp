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

    void RedisServer::connectEvent(Events::ConnectResponceEvent* ev)
    {
        using namespace Events;
        ConnectResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedConnect(v);
    }

    void RedisServer::disconnectEvent(Events::DisconnectResponceEvent* ev)
    {
        using namespace Events;
        DisconnectResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedDisconnect(v);
    }

    void RedisServer::loadDatabaseInfosEvent(Events::LoadDatabasesInfoResponceEvent* ev)
    {
        using namespace Events;
        LoadDatabasesInfoResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedLoadDatabases(v);
    }

    void RedisServer::loadDatabaseContentEvent(Events::LoadDatabaseContentResponceEvent* ev)
    {
        using namespace Events;
        LoadDatabaseContentResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedLoadDataBaseContent(v);
    }

    void RedisServer::loadServerInfoEvent(Events::ServerInfoResponceEvent* ev)
    {
        using namespace Events;
        ServerInfoResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedLoadServerInfo(v);
    }

    void RedisServer::loadServerPropertyEvent(Events::ServerPropertyInfoResponceEvent* ev)
    {
        using namespace Events;
        ServerPropertyInfoResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedLoadServerProperty(v);
    }

    void RedisServer::serverPropertyChangeEvent(Events::ChangeServerPropertyInfoResponceEvent* ev)
    {
        using namespace Events;
        ChangeServerPropertyInfoResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedChangeServerProperty(v);
    }

    void RedisServer::shutdownEvent(Events::ShutDownResponceEvent* ev)
    {
        using namespace Events;
        ShutDownResponceEvent::value_type v = ev->value();
        common::ErrorValueSPtr er(v.errorInfo());
        if(er && er->isError()){
            LOG_ERROR(er, true);
        }
        emit finishedShutdown(v);
    }
}
