#include "core/redis/RedisDataBase.h"

namespace fastoredis
{
    RedisDatabase::RedisDatabase(IServer *server, const QString &name)
        : base_class(server, name)
    {

    }
}
