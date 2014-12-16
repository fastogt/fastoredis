#pragma once

#include "common/patterns/singleton_pattern.h"

#include "core/iserver.h"

namespace fastoredis
{
    class ServersManager
            : public common::patterns::lazy_singleton<ServersManager>
    {
        friend class common::patterns::lazy_singleton<ServersManager>;
    public:
        typedef std::vector<IServerSPtr> ServersContainer;

        IServerSPtr createServer(const IConnectionSettingsBasePtr& settings);
        void closeServer(IServerSPtr server);
        void setSyncServers(bool isSync);        

    private:
        ServersManager();
        ~ServersManager();

        void refreshSyncServers();
        IServerSPtr findServerBySetting(const IConnectionSettingsBasePtr& settings) const;
        std::vector<QObject*> findAllListeners(const IDriverSPtr& drv);

        ServersContainer servers_;
        bool syncServers_;
    };
}
