#pragma once

#include "core/icluster.h"

namespace fastoredis
{
    class RedisCluster
            : public ICluster
    {
        friend class ServersManager;
        Q_OBJECT
    public:

    private:
        RedisCluster(IServerSPtr root, const std::string& name);
    };
}
