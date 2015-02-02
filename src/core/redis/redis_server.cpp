#include "core/redis/redis_server.h"

#include "core/redis/redis_database.h"
#include "core/logger.h"

namespace fastoredis
{
    RedisServer::RedisServer(const IDriverSPtr& drv, bool isMaster)
        : IServer(drv, isMaster)
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

    IDatabaseSPtr RedisServer::createDatabaseImpl(DataBaseInfoSPtr info)
    {
        return IDatabaseSPtr(new RedisDatabase(shared_from_this(), info));
    }
}
