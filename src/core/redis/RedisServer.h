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

    protected:
        virtual void connectEvent(Events::ConnectResponceEvent *ev);
        virtual void disconnectEvent(Events::DisconnectResponceEvent *ev);
        virtual void executeEvent(Events::ExecuteResponceEvent *ev);
        virtual void loadDatabasesEvent(Events::LoadDatabasesInfoResponceEvent *ev);

    private:
        RedisServer(const IDriverPtr &drv);
        RedisServer(const IServerPtr &srv);
    };
}
