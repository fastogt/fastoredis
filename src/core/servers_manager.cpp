#include "core/servers_manager.h"

#include "core/settings_manager.h"
#include "core/redis/redis_server.h"
#include "core/redis/redis_driver.h"

namespace fastoredis
{
    ServersManager::ServersManager()
        : syncServers_(SettingsManager::instance().syncTabs())
    {

    }

    ServersManager::~ServersManager()
    {

    }

    void ServersManager::setSyncServers(bool isSync)
    {
        syncServers_ = isSync;
        refreshSyncServers();
    }

    void ServersManager::refreshSyncServers()
    {
        for(size_t i = 0; i < servers_.size(); ++i){
            IServerPtr servi = servers_[i];
            if(servi->isMaster()){
                for(size_t j = 0; j < servers_.size(); ++j){
                    IServerPtr servj = servers_[j];
                    if(servj != servi && servj->driver() == servi->driver()){
                        if(syncServers_){
                            servj->syncWithServer(servi.get());
                        }
                        else{
                            servj->unSyncFromServer(servi.get());
                        }
                    }
                }
            }
        }
    }

    IServerPtr ServersManager::createServer(const IConnectionSettingsBasePtr &settings)
    {
        IServerPtr result;
        connectionTypes conT = settings->connectionType();
        IServerPtr ser = findServerBySetting(settings);
        if(conT == REDIS){            
            RedisServer *newRed = NULL;
            if(!ser){
                IDriverPtr dr(new RedisDriver(settings));
                newRed = new RedisServer(dr, true);
            }
            else{
                newRed = new RedisServer(ser->driver(), false);
            }
            result.reset(newRed);
            servers_.push_back(result);
        }
        DCHECK(result);
        if(ser && syncServers_){
            result->syncWithServer(ser.get());
        }
        return result;
    }

    void ServersManager::closeServer(IServerPtr server)
    {
        for(size_t i = 0; i < servers_.size(); ++i){
            IServerPtr ser = servers_[i];
            if(ser == server){
                if(ser->isMaster()){
                    IDriverPtr drv = ser->driver();
                    for(size_t j = 0; j < servers_.size(); ++j){
                        IServerPtr servj = servers_[j];
                        if(servj->driver() == drv){
                            servj->setIsMaster(true);
                            break;
                        }
                    }
                }
                servers_.erase(servers_.begin()+i);
                refreshSyncServers();
                break;
            }
        }
    }

    IServerPtr ServersManager::findServerBySetting(const IConnectionSettingsBasePtr &settings) const
    {
        for(size_t i = 0; i < servers_.size(); ++i){
            IServerPtr drp = servers_[i];
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
        for(size_t j = 0; j < servers_.size(); ++j){
            IServerPtr ser = servers_[j];
            if(ser->driver() == drv){
                result.push_back(ser.get());
            }
        }
        return result;
    }
}
