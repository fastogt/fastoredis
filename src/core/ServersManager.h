#pragma once

#include "common/patterns/singleton_pattern.hpp"
#include "core/IServer.h"
#include "core/events/EventsInfo.hpp"

namespace fastoredis
{
    class ServersManager
            : public common::patterns::lazy_singleton<ServersManager>
    {
        friend class common::patterns::lazy_singleton<ServersManager>;

    public:        
        typedef QObject base_class;
        typedef std::vector<IServerPtr> ServersContainer;

        IServerPtr createServer(const IConnectionSettingsBasePtr &settings);

        ~ServersManager();

    private:
        ServersManager();
        IServerPtr findServerBySetting(const IConnectionSettingsBasePtr &settings) const;
        std::vector<QObject*> findAllListeners(const IDriverPtr &drv);

        ServersContainer _servers;
    };
}
