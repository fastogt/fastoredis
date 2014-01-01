#include "core/redis/RedisDataBase.h"

namespace fastoredis
{
    RedisDatabase::RedisDatabase(const IServerPtr &server, const QString &name)
        : IDatabase(server, name)
    {

    }
}
