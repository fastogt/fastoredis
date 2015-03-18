#include "core/redis/redis_cluster.h"

namespace fastoredis
{
    RedisCluster::RedisCluster(IServerSPtr root, const std::string& name)
        : ICluster(root, name)
    {

    }
}
