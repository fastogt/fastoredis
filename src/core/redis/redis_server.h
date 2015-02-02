#pragma once

#include "core/iserver.h"

namespace fastoredis
{
    class RedisServer
            : public IServer
    {
        friend class ServersManager;
        Q_OBJECT
    public:
        virtual void syncWithServer(IServer* src);
        virtual void unSyncFromServer(IServer* src);

    private:
        virtual IDatabaseSPtr createDatabaseImpl(DataBaseInfoSPtr info);
        RedisServer(const IDriverSPtr& drv, bool isMaster);
    };
}
