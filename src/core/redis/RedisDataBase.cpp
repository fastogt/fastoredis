#include "core/redis/RedisDataBase.h"

namespace fastoredis
{
    RedisDatabase::RedisDatabase(const IServerPtr &server, const QString &name)
        : base_class(server, name)
    {

    }
}
