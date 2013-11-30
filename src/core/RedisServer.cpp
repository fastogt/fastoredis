#include "core/RedisServer.h"

#include "core/ConnectionSettings.h"

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
}
