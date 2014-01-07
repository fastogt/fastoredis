#include "core/redis/RedisDataBase.h"

namespace fastoredis
{
    RedisDatabase::RedisDatabase(IServer *server, const DataBaseInfo &info)
        : IDatabase(server, info)
    {

    }
}
