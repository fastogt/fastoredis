#pragma once

#include "common/patterns/singleton_pattern.hpp"
#include "core/iserver.h"
#include "core/events/events_info.hpp"

namespace fastoredis
{
    class ServersManager
            : public common::patterns::lazy_singleton<ServersManager>
    {
        friend class common::patterns::lazy_singleton<ServersManager>;
    public:
        typedef std::vector<IServerPtr> ServersContainer;

        IServerPtr createServer(const IConnectionSettingsBasePtr &settings);
        void closeServer(const IServerPtr &server);
        void setSyncServers(bool isSync);

        ~ServersManager();

    private:
        ServersManager();
        void refreshSyncServers();
        IServerPtr findServerBySetting(const IConnectionSettingsBasePtr &settings) const;
        std::vector<QObject*> findAllListeners(const IDriverPtr &drv);

        ServersContainer _servers;
        bool syncServers_;
    };
}
