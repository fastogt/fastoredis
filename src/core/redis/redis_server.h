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
        virtual void syncWithServer(IServer *src);
        virtual void unSyncFromServer(IServer *src);

    protected:
        virtual void connectEvent(Events::ConnectResponceEvent *ev);
        virtual void disconnectEvent(Events::DisconnectResponceEvent *ev);
        virtual void executeEvent(Events::ExecuteResponceEvent *ev);
        virtual void loadDatabaseInfosEvent(Events::LoadDatabasesInfoResponceEvent *ev);
        virtual void loadDatabaseContentEvent(Events::LoadDatabaseContentResponceEvent *ev);
        virtual void loadServerInfoEvent(Events::ServerInfoResponceEvent *ev);
        virtual void loadServerPropertyEvent(Events::ServerPropertyInfoResponceEvent *ev);
        virtual void serverPropertyChangeEvent(Events::ChangeServerPropertyInfoResponceEvent *ev);

    private:
        RedisServer(const IDriverPtr &drv, bool isMaster);
    };
}
