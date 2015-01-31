#pragma once

#include "core/iserver.h"

namespace fastoredis
{
    class MemcachedServer
            : public IServer
    {
        friend class ServersManager;
        Q_OBJECT
    public:
        virtual void syncWithServer(IServer* src);
        virtual void unSyncFromServer(IServer* src);

    private:
        virtual IDatabaseSPtr createDatabaseImpl(DataBaseInfoSPtr info);
        MemcachedServer(const IDriverSPtr& drv, bool isMaster);
    };
}
