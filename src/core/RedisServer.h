#pragma once

#include "core/IServer.h"

namespace fastoredis
{
    class RedisServer
            : public IServer
    {
        friend class ServersManager;
        Q_OBJECT
    public:
        typedef IServer base_class;
    private:
        RedisServer(const IDriverPtr &drv);
        RedisServer(const IServerPtr &srv);
    };
}
