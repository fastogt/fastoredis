#include "core/redis/RedisServer.h"

#include "core/redis/RedisDataBase.h"
#include "core/ConnectionSettings.h"
#include "common/qt_helper/converter_patterns.h"

namespace fastoredis
{
    RedisServer::RedisServer(const IDriverPtr &drv)
        :base_class(drv)
    {

    }

    RedisServer::RedisServer(const IServerPtr &srv)
        :base_class(srv)
    {

    }

    void RedisServer::connectEvent(Events::ConnectResponceEvent *ev)
    {
        using namespace Events;
        ConnectResponceEvent::value_type v = ev->value();
        emit finishedConnect(v);
    }

    void RedisServer::disconnectEvent(Events::DisconnectResponceEvent *ev)
    {
        using namespace Events;
        DisconnectResponceEvent::value_type v = ev->value();
        emit finishedDisconnect(v);
    }

    void RedisServer::executeEvent(Events::ExecuteResponceEvent *ev)
    {
        using namespace Events;
        ExecuteResponceEvent::value_type v = ev->value();
        emit finishedExecute(v);
    }

    void RedisServer::loadDatabasesEvent(Events::LoadDatabasesInfoResponceEvent *ev)
    {
        using namespace Events;
        LoadDatabasesInfoResponceEvent::value_type v = ev->value();
        EventsInfo::LoadDatabasesInfoResponce::database_info_cont_type cont = v.databases_;
        databases_.clear();
        for(int i = 0; i < cont.size(); ++i){
            databases_.push_back(IDatabasePtr(new RedisDatabase(this, common::utils_qt::toQString(cont[i]))));
        }
        emit finishedLoadDatabases(v);
    }
}
