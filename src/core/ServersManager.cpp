#include "core/ServersManager.h"

#include "core/RedisServer.h"
#include "core/RedisDriver.h"
#include "common/macros.h"
#include "core/events/Events.hpp"

namespace fastoredis
{
    ServersManager::ServersManager()
    {

    }

    ServersManager::~ServersManager()
    {

    }

    IServerPtr ServersManager::createServer(const IConnectionSettingsBasePtr &settings)
    {
        IServerPtr result;
        connectionTypes conT = settings->connectionType();
        if(conT == REDIS){
            IServerPtr ser = findServerBySetting(settings);
            RedisServer *newRed = NULL;
            if(!ser){
                IDriverPtr dr(new RedisDriver(settings));
                newRed = new RedisServer(dr);
            }
            else{
                newRed = new RedisServer(ser);
            }
            result.reset(newRed);
            _servers.push_back(result);
        }
        DCHECK(result);
        return result;
    }

    IServerPtr ServersManager::findServerBySetting(const IConnectionSettingsBasePtr &settings) const
    {
        for(size_t i = 0; i < _servers.size(); ++i){
            IServerPtr drp = _servers[i];
            IDriverPtr curDr = drp->driver();
            if(curDr->settings() == settings){
                return drp;
            }
        }
        return IServerPtr();
    }

    std::vector<QObject *> ServersManager::findAllListeners(const IDriverPtr &drv)
    {
        std::vector<QObject *> result;
        for(size_t j = 0; j < _servers.size(); ++j){
            IServerPtr ser = _servers[j];
            if(ser->driver() == drv){
                result.push_back(ser.get());
            }
        }
        return result;
    }
}
