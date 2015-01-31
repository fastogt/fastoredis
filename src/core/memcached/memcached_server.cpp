#include "core/memcached/memcached_server.h"

#include "core/memcached/memcached_database.h"
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

    IDatabaseSPtr MemcachedServer::createDatabaseImpl(DataBaseInfoSPtr info)
    {
        return IDatabaseSPtr(new MemcachedDatabase(shared_from_this(), info));
    }
}
