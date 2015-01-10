#include "core/memcached/memcached_driver.h"

#include <libmemcached/memcached.h>

extern "C" {
#include "third-party/redis/deps/hiredis/sds.h"
}

#include "common/utils.h"

#include "core/logger.h"
#include "core/command_logger.h"

#include "core/memcached/memcached_config.h"
#include "core/memcached/memcached_infos.h"

namespace fastoredis
{
    struct MemcachedDriver::pimpl
    {
        pimpl()
            : memc_(NULL), isConnected_(false)
        {

        }

        bool isConnected() const
        {
            return isConnected_;
        }

        common::ErrorValueSPtr connect()
        {
            if(isConnected_){
                return common::ErrorValueSPtr();
            }

            clear();
            init();

            if(!memc_){
                return common::make_error_value("Init error", common::ErrorValue::E_ERROR);
            }

            if(config_.shutdown){
                return common::make_error_value("Interrupted connect.", common::ErrorValue::E_INTERRUPTED);
            }

            memcached_return rc;
            char buff[1024] = {0};

            if(config_.user_ && config_.password_){
                rc = memcached_set_sasl_auth_data(memc_, config_.user_, config_.password_);
                if (rc != MEMCACHED_SUCCESS){
                    sprintf(buff, "Couldn't setup SASL auth: %s", memcached_strerror(memc_, rc));
                    return common::make_error_value(buff, common::ErrorValue::E_ERROR);
                }
            }

            /*rc = memcached_behavior_set(memc_, MEMCACHED_BEHAVIOR_CONNECT_TIMEOUT, 10000);
            if (rc != MEMCACHED_SUCCESS){
                sprintf(buff, "Couldn't set the connect timeout: %s", memcached_strerror(memc_, rc));
                return common::make_error_value(buff, common::ErrorValue::E_ERROR);
            }*/

            memcached_server_st *servers = NULL;
            servers = memcached_server_list_append_with_weight(servers, config_.hostip, config_.hostport, 1, &rc);
            if (rc != MEMCACHED_SUCCESS){
                sprintf(buff, "Couldn't add server: %s", memcached_strerror(memc_, rc));
                return common::make_error_value(buff, common::ErrorValue::E_ERROR);
            }

            rc = memcached_server_push(memc_, servers);
            if (rc != MEMCACHED_SUCCESS){
                sprintf(buff, "Couldn't add server: %s", memcached_strerror(memc_, rc));
                return common::make_error_value(buff, common::ErrorValue::E_ERROR);
            }

            isConnected_ = true;
            return common::ErrorValueSPtr();
        }

        common::ErrorValueSPtr disconnect()
        {
            if(!isConnected_){
                return common::ErrorValueSPtr();
            }

            clear();

            isConnected_ = false;
            return common::ErrorValueSPtr();
        }

        common::ErrorValueSPtr execute(const char *command, Command::CommandType type, FastoObject* out) WARN_UNUSED_RESULT
        {
            DCHECK(out);
            if(!out){
                return common::make_error_value("Invalid input argument", common::ErrorValue::E_ERROR);
            }

            if(!command){
                return common::make_error_value("Command empty", common::ErrorValue::E_ERROR);
            }

            LOG_COMMAND(Command(command, type));

            common::ErrorValueSPtr er;
            if (command[0] != '\0') {
                int argc;
                sds *argv = sdssplitargs(command,&argc);

                if (argv == NULL) {
                    common::StringValue *val = common::Value::createStringValue("Invalid argument(s)");
                    FastoObject* child = new FastoObject(out, val, config_.mb_delim);
                    out->addChildren(child);
                }
                else if (argc > 0) {
                    if (strcasecmp(argv[0], "quit") == 0){
                        config_.shutdown = 1;
                    }
                    else {
                        er = execute(out, argc, argv);
                    }
                }
                sdsfreesplitres(argv,argc);
            }

            return er;
        }

        ~pimpl()
        {
            clear();
        }

        memcachedConfig config_;
        SSHInfo sinfo_;

   private:
        common::ErrorValueSPtr execute(FastoObject* out, int argc, char **argv)
        {
            if(strcasecmp(argv[0], "get") == 0){
                if(argc != 2){
                    return common::make_error_value("Invalid get input argument", common::ErrorValue::E_ERROR);
                }

                std::string ret;
                common::ErrorValueSPtr er = get(argv[1], ret);
                if(!er){
                    common::StringValue *val = common::Value::createStringValue(ret);
                    FastoObject* child = new FastoObject(out, val, config_.mb_delim);
                    out->addChildren(child);
                }
                return er;
            }
            else if(strcasecmp(argv[0], "set") == 0){
                if(argc != 5){
                    return common::make_error_value("Invalid get input argument", common::ErrorValue::E_ERROR);
                }

                common::ErrorValueSPtr er = set(argv[1], argv[4], atoi(argv[2]), atoi(argv[3]));
                if(!er){
                    common::StringValue *val = common::Value::createStringValue("STORED");
                    FastoObject* child = new FastoObject(out, val, config_.mb_delim);
                    out->addChildren(child);
                }
                return er;
            }
            else if(strcasecmp(argv[0], "add") == 0){
                if(argc != 4){
                    return common::make_error_value("Invalid get input argument", common::ErrorValue::E_ERROR);
                }

                common::ErrorValueSPtr er = add(argv[1], argv[4], atoi(argv[2]), atoi(argv[3]));
                if(!er){
                    common::StringValue *val = common::Value::createStringValue("STORED");
                    FastoObject* child = new FastoObject(out, val, config_.mb_delim);
                    out->addChildren(child);
                }
                return er;
            }
            else if(strcasecmp(argv[0], "replace") == 0){
                if(argc != 4){
                    return common::make_error_value("Invalid get input argument", common::ErrorValue::E_ERROR);
                }

                common::ErrorValueSPtr er = replace(argv[1], argv[4], atoi(argv[2]), atoi(argv[3]));
                if(!er){
                    common::StringValue *val = common::Value::createStringValue("STORED");
                    FastoObject* child = new FastoObject(out, val, config_.mb_delim);
                    out->addChildren(child);
                }
                return er;
            }
            else if(strcasecmp(argv[0], "delete") == 0){
                if(argc != 2){
                    return common::make_error_value("Invalid get input argument", common::ErrorValue::E_ERROR);
                }

                common::ErrorValueSPtr er = del(argv[1]);
                if(!er){
                    common::StringValue *val = common::Value::createStringValue("DELETED");
                    FastoObject* child = new FastoObject(out, val, config_.mb_delim);
                    out->addChildren(child);
                }
                return er;
            }
            else{
                char buff[1024] = {0};
                sprintf(buff, "Not supported command: %s", argv[0]);
                return common::make_error_value(buff, common::ErrorValue::E_ERROR);
            }
        }

        common::ErrorValueSPtr get(const std::string& key, std::string& ret_val)
        {
            ret_val.clear();
            uint32_t flags = 0;
            memcached_return error;
            size_t value_length = 0;

            char *value = memcached_get(memc_, key.c_str(), key.length(), &value_length, &flags, &error);
            if (error != MEMCACHED_SUCCESS){
                char buff[1024] = {0};
                sprintf(buff, "Get function error: %s", memcached_strerror(memc_, error));
                return common::make_error_value(buff, common::ErrorValue::E_ERROR);
            }

            if (value != NULL){
                ret_val.reserve(value_length +1); // Always provide null
                ret_val.assign(value, value + value_length +1);
                ret_val.resize(value_length);
                free(value);
            }
            return common::ErrorValueSPtr();
        }

        common::ErrorValueSPtr set(const std::string& key, const std::string& value, time_t expiration, uint32_t flags)
        {
            memcached_return_t error = memcached_set(memc_, key.c_str(), key.length(), value.c_str(), value.length(), expiration, flags);
            if (error != MEMCACHED_SUCCESS){
                char buff[1024] = {0};
                sprintf(buff, "Set function error: %s", memcached_strerror(memc_, error));
                return common::make_error_value(buff, common::ErrorValue::E_ERROR);
            }

            return common::ErrorValueSPtr();
        }

        common::ErrorValueSPtr add(const std::string& key, const std::string& value, time_t expiration, uint32_t flags)
        {
            memcached_return_t error = memcached_add(memc_, key.c_str(), key.length(), value.c_str(), value.length(), expiration, flags);
            if (error != MEMCACHED_SUCCESS){
                char buff[1024] = {0};
                sprintf(buff, "Add function error: %s", memcached_strerror(memc_, error));
                return common::make_error_value(buff, common::ErrorValue::E_ERROR);
            }

            return common::ErrorValueSPtr();
        }

        common::ErrorValueSPtr replace(const std::string& key, const std::string& value, time_t expiration, uint32_t flags)
        {
            memcached_return_t error = memcached_replace(memc_, key.c_str(), key.length(), value.c_str(), value.length(), expiration, flags);
            if (error != MEMCACHED_SUCCESS){
                char buff[1024] = {0};
                sprintf(buff, "Replace function error: %s", memcached_strerror(memc_, error));
                return common::make_error_value(buff, common::ErrorValue::E_ERROR);
            }

            return common::ErrorValueSPtr();
        }

        common::ErrorValueSPtr del(const std::string& key)
        {
            time_t expiration = 0;

            memcached_return_t error = memcached_delete(memc_, key.c_str(), key.length(), expiration);
            if (error != MEMCACHED_SUCCESS){
                char buff[1024] = {0};
                sprintf(buff, "Delete function error: %s", memcached_strerror(memc_, error));
                return common::make_error_value(buff, common::ErrorValue::E_ERROR);
            }

            return common::ErrorValueSPtr();
        }

        void init()
        {
            DCHECK(!memc_);
            memc_ = memcached(NULL, 0);
            DCHECK(memc_);
        }

        void clear()
        {
            if(memc_){
                memcached_free(memc_);
            }
            memc_ = NULL;
        }

        memcached_st* memc_;
        bool isConnected_;
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
        return impl_->isConnected();
    }

    void MemcachedDriver::interrupt()
    {
        impl_->config_.shutdown = 1;
    }

    common::net::hostAndPort MemcachedDriver::address() const
    {
        return common::net::hostAndPort(impl_->config_.hostip, impl_->config_.hostport);
    }

    std::string MemcachedDriver::version() const
    {
        return versionApi();
    }

    std::string MemcachedDriver::outputDelemitr() const
    {
        return impl_->config_.mb_delim;
    }

    const char* MemcachedDriver::versionApi()
    {
        return memcached_lib_version();
    }

    void MemcachedDriver::customEvent(QEvent *event)
    {
        IDriver::customEvent(event);
        impl_->config_.shutdown = 0;
    }

    void MemcachedDriver::initImpl()
    {

    }

    common::ErrorValueSPtr MemcachedDriver::currentLoggingInfo(FastoObject* out)
    {

    }

    void MemcachedDriver::handleConnectEvent(Events::ConnectRequestEvent *ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            Events::ConnectResponceEvent::value_type res(ev->value());
            MemcachedConnectionSettings *set = dynamic_cast<MemcachedConnectionSettings*>(settings_.get());
            if(set){
                impl_->config_ = set->info();
                impl_->sinfo_ = set->sshInfo();
                common::ErrorValueSPtr er;
        notifyProgress(sender, 25);
                    er = impl_->connect();
                    if(er){
                        res.setErrorInfo(er);
                    }
        notifyProgress(sender, 75);
            }
            reply(sender, new Events::ConnectResponceEvent(this, res));
        notifyProgress(sender, 100);
    }

    void MemcachedDriver::handleDisconnectEvent(Events::DisconnectRequestEvent* ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            Events::DisconnectResponceEvent::value_type res(ev->value());
        notifyProgress(sender, 50);

            common::ErrorValueSPtr er = impl_->disconnect();
            if(er){
                res.setErrorInfo(er);
            }

            reply(sender, new Events::DisconnectResponceEvent(this, res));
        notifyProgress(sender, 100);
    }

    void MemcachedDriver::handleExecuteEvent(Events::ExecuteRequestEvent* ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            Events::ExecuteRequestEvent::value_type res(ev->value());
            const char *inputLine = common::utils::c_strornull(res.text_);

            common::ErrorValueSPtr er;
            if(inputLine){
                size_t length = strlen(inputLine);
                int offset = 0;
                RootLocker lock = make_locker(sender, inputLine);
                FastoObjectIPtr outRoot = lock.root_;
                double step = 100.0f/length;
                for(size_t n = 0; n < length; ++n){
                    if(impl_->config_.shutdown){
                        er.reset(new common::ErrorValue("Interrupted exec.", common::ErrorValue::E_INTERRUPTED));
                        res.setErrorInfo(er);
                        break;
                    }
                    if(inputLine[n] == '\n' || n == length-1){
        notifyProgress(sender, step * n);
                        char command[128] = {0};
                        if(n == length-1){
                            strcpy(command, inputLine + offset);
                        }
                        else{
                            strncpy(command, inputLine + offset, n - offset);
                        }
                        offset = n + 1;
                        er = impl_->execute(command, Command::UserCommand, outRoot.get());
                        if(er){
                            res.setErrorInfo(er);
                            break;
                        }
                    }
                }
            }
            else{
                er.reset(new common::ErrorValue("Empty command line.", common::ErrorValue::E_ERROR));
            }

            if(er){
                LOG_ERROR(er);
            }
        notifyProgress(sender, 100);
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
