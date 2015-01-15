#include "core/servers_manager.h"

#include "core/settings_manager.h"

#include "core/redis/redis_server.h"
#include "core/redis/redis_driver.h"

#include "core/memcached/memcached_server.h"
#include "core/memcached/memcached_driver.h"

namespace fastoredis
{
    ServersManager::ServersManager()
        : syncServers_(SettingsManager::instance().syncTabs())
    {
        qRegisterMetaType<ServerInfoSnapShoot>("ServerInfoSnapShoot");
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
            IServerSPtr servi = servers_[i];
            if(servi->isMaster()){
                for(size_t j = 0; j < servers_.size(); ++j){
                    IServerSPtr servj = servers_[j];
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

    IServerSPtr ServersManager::createServer(const IConnectionSettingsBaseSPtr &settings)
    {
        IServerSPtr result;
        connectionTypes conT = settings->connectionType();
        IServerSPtr ser = findServerBySetting(settings);
        if(conT == REDIS){            
            RedisServer *newRed = NULL;
            if(!ser){
                IDriverSPtr dr(new RedisDriver(settings));
                dr->start();
                newRed = new RedisServer(dr, true);
            }
            else{
                newRed = new RedisServer(ser->driver(), false);
            }
            result.reset(newRed);
            servers_.push_back(result);
        }
        else if(conT == MEMCACHED){
            MemcachedServer *newMem = NULL;
            if(!ser){
                IDriverSPtr dr(new MemcachedDriver(settings));
                dr->start();
                newMem = new MemcachedServer(dr, true);
            }
            else{
                newMem = new MemcachedServer(ser->driver(), false);
            }
            result.reset(newMem);
            servers_.push_back(result);
        }

        DCHECK(result);
        if(ser && syncServers_){
            result->syncWithServer(ser.get());
        }
        return result;
    }

    void ServersManager::closeServer(IServerSPtr server)
    {
        for(size_t i = 0; i < servers_.size(); ++i){
            IServerSPtr ser = servers_[i];
            if(ser == server){
                if(ser->isMaster()){
                    IDriverSPtr drv = ser->driver();
                    for(size_t j = 0; j < servers_.size(); ++j){
                        IServerSPtr servj = servers_[j];
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

    IServerSPtr ServersManager::findServerBySetting(const IConnectionSettingsBaseSPtr &settings) const
    {
        for(size_t i = 0; i < servers_.size(); ++i){
            IServerSPtr drp = servers_[i];
            IDriverSPtr curDr = drp->driver();
            if(curDr->settings() == settings){
                return drp;
            }
        }
        return IServerSPtr();
    }

    std::vector<QObject *> ServersManager::findAllListeners(const IDriverSPtr &drv) const
    {
        std::vector<QObject *> result;
        for(size_t j = 0; j < servers_.size(); ++j){
            IServerSPtr ser = servers_[j];
            if(ser->driver() == drv){
                result.push_back(ser.get());
            }
        }
        return result;
    }
}
