#include "core/memcached/memcached_driver.h"

#include <libmemcached/memcached.hpp>

#include "core/memcached/memcached_config.h"
#include "core/memcached/memcached_infos.h"

namespace fastoredis
{
    struct MemcachedDriver::pimpl
    {
        pimpl()
        {

        }

        ~pimpl()
        {
        }

        memcachedConfig config;
        SSHInfo sinfo_;
   };

    MemcachedDriver::MemcachedDriver(const IConnectionSettingsBaseSPtr &settings)
        : IDriver(settings), impl_(new pimpl)
    {
    }

    MemcachedDriver::~MemcachedDriver()
    {
        delete impl_;
    }

    bool MemcachedDriver::isConnected() const
    {
        return false;
    }

    void MemcachedDriver::interrupt()
    {

    }

    common::net::hostAndPort MemcachedDriver::address() const
    {
        return common::net::hostAndPort(impl_->config.hostip, impl_->config.hostport);
    }

    std::string MemcachedDriver::version() const
    {
        return versionApi();
    }

    std::string MemcachedDriver::outputDelemitr() const
    {
        return impl_->config.mb_delim;
    }

    const char* MemcachedDriver::versionApi()
    {
        return memcached_lib_version();
    }

    void MemcachedDriver::customEvent(QEvent *event)
    {
        IDriver::customEvent(event);
    }

    void MemcachedDriver::initImpl()
    {

    }

    common::ErrorValueSPtr MemcachedDriver::currentLoggingInfo(FastoObject* out)
    {

    }

    void MemcachedDriver::handleConnectEvent(Events::ConnectRequestEvent *ev)
    {

    }

    void MemcachedDriver::handleDisconnectEvent(Events::DisconnectRequestEvent* ev)
    {

    }

    void MemcachedDriver::handleExecuteEvent(Events::ExecuteRequestEvent* ev)
    {

    }

    void MemcachedDriver::handleLoadDatabaseInfosEvent(Events::LoadDatabasesInfoRequestEvent* ev)
    {

    }

    void MemcachedDriver::handleLoadDatabaseContentEvent(Events::LoadDatabaseContentRequestEvent* ev)
    {

    }

    void MemcachedDriver::handleLoadServerInfoEvent(Events::ServerInfoRequestEvent* ev)
    {

    }

    void MemcachedDriver::handleLoadServerPropertyEvent(Events::ServerPropertyInfoRequestEvent* ev)
    {

    }

    void MemcachedDriver::handleServerPropertyChangeEvent(Events::ChangeServerPropertyInfoRequestEvent* ev)
    {

    }

    void MemcachedDriver::handleProcessCommandLineArgs(Events::ProcessConfigArgsRequestEvent* ev)
    {

    }

    void MemcachedDriver::handleShutdownEvent(Events::ShutDownRequestEvent* ev)
    {

    }

    void MemcachedDriver::handleBackupEvent(Events::BackupRequestEvent* ev)
    {

    }

    void MemcachedDriver::handleExportEvent(Events::ExportRequestEvent* ev)
    {

    }

    ServerInfoSPtr MemcachedDriver::makeServerInfoFromString(const std::string& val)
    {
        return makeMemcachedServerInfo(val);
    }
}
