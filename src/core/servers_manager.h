#pragma once

#include "common/patterns/singleton_pattern.h"

#include "core/iserver.h"

namespace fastoredis
{
    class ServersManager
            : public common::patterns::LazySingleton<ServersManager>
    {
        friend class common::patterns::LazySingleton<ServersManager>;
    public:
        typedef std::vector<IServerSPtr> ServersContainer;

        IServerSPtr createServer(const IConnectionSettingsBaseSPtr& settings);
        void closeServer(IServerSPtr server);
        void setSyncServers(bool isSync);        

    private:
        ServersManager();
        ~ServersManager();

        void refreshSyncServers();
        IServerSPtr findServerBySetting(const IConnectionSettingsBaseSPtr& settings) const;
        std::vector<QObject*> findAllListeners(const IDriverSPtr& drv) const;

        ServersContainer servers_;
        bool syncServers_;
    };
}
