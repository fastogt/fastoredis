#include "core/memcached/memcached_driver.h"

#include <libmemcached/memcached.h>
#include <libmemcached/util.h>

extern "C" {
#include "third-party/redis/deps/hiredis/sds.h"
}

#include "common/utils.h"
#include "common/string_util.h"

#include "core/logger.h"
#include "core/command_logger.h"

#include "core/memcached/memcached_config.h"
#include "core/memcached/memcached_infos.h"

#define INFO_REQUEST "stats"
#define GET_KEYS "stats items"

namespace
{
    std::string getKeyFromLine(std::string input)
    {
        if(input.empty()){
            return std::string();
        }

        size_t pos = input.find_first_of(' ');
        if(pos != std::string::npos){
            input = input.substr(pos+1);
        }

        std::string trimed;
        common::TrimWhitespaceASCII(input, common::TRIM_ALL, &trimed);
        return trimed;
    }
}

namespace fastoredis
{
    namespace
    {
        class MemcachedCommand
                : public FastoObjectCommand
        {
        public:
            MemcachedCommand(FastoObject* parent, common::CommandValue* cmd, const std::string &delemitr)
                : FastoObjectCommand(parent, cmd, delemitr)
            {

            }

            virtual std::string key() const
            {
                common::CommandValue* command = cmd();
                if(command){
                    return getKeyFromLine(command->inputCommand());
                }

                return std::string();
            }
        };

        FastoObjectCommand* createCommand(FastoObject* parent, const std::string& input,
                                                   const std::string& opposite, common::Value::CommandType ct)
        {
            DCHECK(parent);
            common::CommandValue* cmd = common::Value::createCommand(input, opposite, ct);
            FastoObjectCommand* fs = new MemcachedCommand(parent, cmd, "");
            parent->addChildren(fs);
            return fs;
        }

        FastoObjectCommand* createCommand(FastoObjectIPtr parent, const std::string& input,
                                                   const std::string& opposite, common::Value::CommandType ct)
        {
            return createCommand(parent.get(), input, opposite, ct);
        }
    }

    common::ErrorValueSPtr testConnection(const char* host, int hostport, const char* user, const char* passwd)
    {
        memcached_return rc;
        char buff[1024] = {0};

        if(user && passwd){
            libmemcached_util_ping2(host, hostport, user, passwd, &rc);
            if (rc != MEMCACHED_SUCCESS){
                sprintf(buff, "Couldn't ping server: %s", memcached_strerror(NULL, rc));
                return common::make_error_value(buff, common::ErrorValue::E_ERROR);
            }
        }
        else{
            libmemcached_util_ping(host, hostport, &rc);
            if (rc != MEMCACHED_SUCCESS){
                sprintf(buff, "Couldn't ping server: %s", memcached_strerror(NULL, rc));
                return common::make_error_value(buff, common::ErrorValue::E_ERROR);
            }
        }

        return common::ErrorValueSPtr();
    }

    struct MemcachedDriver::pimpl
    {
        pimpl()
            : memc_(NULL)
        {

        }

        bool isConnected() const
        {
            if(!memc_){
                return false;
            }

            memcached_server_st *server = (memcached_server_st*)memc_->servers;
            if(!server){
                return false;
            }

            return server->state == MEMCACHED_SERVER_STATE_CONNECTED;
        }

        common::ErrorValueSPtr connect()
        {
            if(isConnected()){
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

            rc = memcached_server_add(memc_, config_.hostip, config_.hostport);

            if (rc != MEMCACHED_SUCCESS){
                sprintf(buff, "Couldn't add server: %s", memcached_strerror(memc_, rc));
                return common::make_error_value(buff, common::ErrorValue::E_ERROR);
            }

            memcached_return_t error = memcached_version(memc_);
            if (error != MEMCACHED_SUCCESS){
                char buff[1024] = {0};
                sprintf(buff, "Connect to server error: %s", memcached_strerror(memc_, error));
                return common::make_error_value(buff, common::ErrorValue::E_ERROR);
            }

            return common::ErrorValueSPtr();
        }

        common::ErrorValueSPtr disconnect()
        {
            if(!isConnected()){
                return common::ErrorValueSPtr();
            }

            clear();
            return common::ErrorValueSPtr();
        }

        common::ErrorValueSPtr stats(const char* args, MemcachedServerInfo::Common& statsout)
        {
            memcached_return_t error;
            memcached_stat_st* st = memcached_stat(memc_, (char*)args, &error);
            if (error != MEMCACHED_SUCCESS){
                char buff[1024] = {0};
                sprintf(buff, "Stats function error: %s", memcached_strerror(memc_, error));
                return common::make_error_value(buff, common::ErrorValue::E_ERROR);
            }

            statsout.pid_ = st->pid;
            statsout.uptime_ = st->uptime;
            statsout.time_ = st->time;
            statsout.version_ = st->version;
            statsout.pointer_size_ = st->pointer_size;
            statsout.rusage_user_ = st->rusage_user_seconds;
            statsout.rusage_system_ = st->rusage_system_seconds;
            statsout.curr_items_ = st->curr_items;
            statsout.total_items_ = st->total_items;
            statsout.bytes_ = st->bytes;
            statsout.curr_connections_ = st->curr_connections;
            statsout.total_connections_ = st->total_connections;
            statsout.connection_structures_ = st->connection_structures;
            statsout.cmd_get_ = st->cmd_get;
            statsout.cmd_set_ = st->cmd_set;
            statsout.get_hits_ = st->get_hits;
            statsout.get_misses_ = st->get_misses;
            statsout.evictions_ = st->evictions;
            statsout.bytes_read_ = st->bytes_read;
            statsout.bytes_written_ = st->bytes_written;
            statsout.limit_maxbytes_ = st->limit_maxbytes;
            statsout.threads_ = st->threads;

            memcached_stat_free(NULL, st);
            return common::ErrorValueSPtr();
        }

        common::ErrorValueSPtr execute(FastoObjectCommand* cmd) WARN_UNUSED_RESULT
        {
            DCHECK(cmd);
            if(!cmd){
                return common::make_error_value("Invalid input argument", common::ErrorValue::E_ERROR);
            }

            const std::string command = cmd->cmd()->inputCommand();
            common::Value::CommandType type = cmd->cmd()->commandType();

            if(command.empty()){
                return common::make_error_value("Command empty", common::ErrorValue::E_ERROR);
            }

            LOG_COMMAND(Command(command, type));

            common::ErrorValueSPtr er;
            if (command[0] != '\0') {
                int argc;
                sds *argv = sdssplitargs(command.c_str(), &argc);

                if (argv == NULL) {
                    common::StringValue *val = common::Value::createStringValue("Invalid argument(s)");
                    FastoObject* child = new FastoObject(cmd, val, config_.mb_delim);
                    cmd->addChildren(child);
                }
                else if (argc > 0) {
                    if (strcasecmp(argv[0], "quit") == 0){
                        config_.shutdown = 1;
                    }
                    else {
                        er = execute(cmd, argc, argv);
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
            else if(strcasecmp(argv[0], "append") == 0){
                if(argc != 4){
                    return common::make_error_value("Invalid get input argument", common::ErrorValue::E_ERROR);
                }

                common::ErrorValueSPtr er = append(argv[1], argv[4], atoi(argv[2]), atoi(argv[3]));
                if(!er){
                    common::StringValue *val = common::Value::createStringValue("STORED");
                    FastoObject* child = new FastoObject(out, val, config_.mb_delim);
                    out->addChildren(child);
                }
                return er;
            }
            else if(strcasecmp(argv[0], "prepend") == 0){
                if(argc != 4){
                    return common::make_error_value("Invalid get input argument", common::ErrorValue::E_ERROR);
                }

                common::ErrorValueSPtr er = prepend(argv[1], argv[4], atoi(argv[2]), atoi(argv[3]));
                if(!er){
                    common::StringValue *val = common::Value::createStringValue("STORED");
                    FastoObject* child = new FastoObject(out, val, config_.mb_delim);
                    out->addChildren(child);
                }
                return er;
            }
            else if(strcasecmp(argv[0], "incr") == 0){
                if(argc != 3){
                    return common::make_error_value("Invalid get input argument", common::ErrorValue::E_ERROR);
                }

                common::ErrorValueSPtr er = incr(argv[1], common::convertFromString<uint64_t>(argv[2]));
                if(!er){
                    common::StringValue *val = common::Value::createStringValue("STORED");
                    FastoObject* child = new FastoObject(out, val, config_.mb_delim);
                    out->addChildren(child);
                }
                return er;
            }
            else if(strcasecmp(argv[0], "decr") == 0){
                if(argc != 3){
                    return common::make_error_value("Invalid get input argument", common::ErrorValue::E_ERROR);
                }

                common::ErrorValueSPtr er = decr(argv[1], common::convertFromString<uint64_t>(argv[2]));
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
            else if(strcasecmp(argv[0], "flush_all") == 0){
                if(argc < 3){
                    return common::make_error_value("Invalid get input argument", common::ErrorValue::E_ERROR);
                }

                common::ErrorValueSPtr er = flush_all(argc == 2 ? common::convertFromString<time_t>(argv[1]) : 0);
                if(!er){
                    common::StringValue *val = common::Value::createStringValue("STORED");
                    FastoObject* child = new FastoObject(out, val, config_.mb_delim);
                    out->addChildren(child);
                }
                return er;
            }
            else if(strcasecmp(argv[0], "stats") == 0){
                if(argc > 2){
                    return common::make_error_value("Invalid stats input argument", common::ErrorValue::E_ERROR);
                }

                MemcachedServerInfo::Common statsout;
                common::ErrorValueSPtr er = stats(argc == 2 ? argv[1] : 0, statsout);
                if(!er){
                    common::StringValue *val = common::Value::createStringValue(MemcachedServerInfo(statsout).toString());
                    FastoObject* child = new FastoObject(out, val, config_.mb_delim);
                    out->addChildren(child);
                }
                return er;
            }
            else if(strcasecmp(argv[0], "version") == 0){
                if(argc != 1){
                    return common::make_error_value("Invalid get input argument", common::ErrorValue::E_ERROR);
                }

                return version_server();
            }
            else if(strcasecmp(argv[0], "verbosity") == 0){
                if(argc != 1){
                    return common::make_error_value("Invalid get input argument", common::ErrorValue::E_ERROR);
                }

                return verbosity();
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

        common::ErrorValueSPtr append(const std::string& key, const std::string& value, time_t expiration, uint32_t flags)
        {
            memcached_return_t error = memcached_append(memc_, key.c_str(), key.length(), value.c_str(), value.length(), expiration, flags);
            if (error != MEMCACHED_SUCCESS){
                char buff[1024] = {0};
                sprintf(buff, "Append function error: %s", memcached_strerror(memc_, error));
                return common::make_error_value(buff, common::ErrorValue::E_ERROR);
            }

            return common::ErrorValueSPtr();
        }

        common::ErrorValueSPtr prepend(const std::string& key, const std::string& value, time_t expiration, uint32_t flags)
        {
            memcached_return_t error = memcached_prepend(memc_, key.c_str(), key.length(), value.c_str(), value.length(), expiration, flags);
            if (error != MEMCACHED_SUCCESS){
                char buff[1024] = {0};
                sprintf(buff, "Prepend function error: %s", memcached_strerror(memc_, error));
                return common::make_error_value(buff, common::ErrorValue::E_ERROR);
            }

            return common::ErrorValueSPtr();
        }

        common::ErrorValueSPtr incr(const std::string& key, uint64_t value)
        {
            memcached_return_t error = memcached_increment(memc_, key.c_str(), key.length(), 0, &value);
            if (error != MEMCACHED_SUCCESS){
                char buff[1024] = {0};
                sprintf(buff, "Incr function error: %s", memcached_strerror(memc_, error));
                return common::make_error_value(buff, common::ErrorValue::E_ERROR);
            }

            return common::ErrorValueSPtr();
        }

        common::ErrorValueSPtr decr(const std::string& key, uint64_t value)
        {
            memcached_return_t error = memcached_decrement(memc_, key.c_str(), key.length(), 0, &value);
            if (error != MEMCACHED_SUCCESS){
                char buff[1024] = {0};
                sprintf(buff, "Decr function error: %s", memcached_strerror(memc_, error));
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

        common::ErrorValueSPtr flush_all(time_t expiration)
        {
            memcached_return_t error = memcached_flush(memc_, expiration);
            if (error != MEMCACHED_SUCCESS){
                char buff[1024] = {0};
                sprintf(buff, "Fluss all function error: %s", memcached_strerror(memc_, error));
                return common::make_error_value(buff, common::ErrorValue::E_ERROR);
            }

            return common::ErrorValueSPtr();
        }

        common::ErrorValueSPtr version_server() const
        {
            memcached_return_t error = memcached_version(memc_);
            if (error != MEMCACHED_SUCCESS){
                char buff[1024] = {0};
                sprintf(buff, "Get server version error: %s", memcached_strerror(memc_, error));
                return common::make_error_value(buff, common::ErrorValue::E_ERROR);
            }

            return common::ErrorValueSPtr();
        }

        common::ErrorValueSPtr verbosity() const
        {
            /*memcached_return_t error = memcached_verbosity(memc_, 1);
            if (error != MEMCACHED_SUCCESS){
                char buff[1024] = {0};
                sprintf(buff, "Verbosity error: %s", memcached_strerror(memc_, error));
                return common::make_error_value(buff, common::ErrorValue::E_ERROR);
            }*/

            return common::make_error_value("Not supported command", common::ErrorValue::E_ERROR);
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

    std::string MemcachedDriver::commandByType(CommandKey::cmdtype type)
    {
        if(type == CommandKey::C_LOAD){
            return "GET";
        }
        else if(type == CommandKey::C_DELETE){
            return "DELETE";
        }
        else{
            return std::string();
        }
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
        currentDatabaseInfo_.reset(new MemcachedDataBaseInfo("0", 0, true));
    }

    void MemcachedDriver::clearImpl()
    {

    }

    common::ErrorValueSPtr MemcachedDriver::currentLoggingInfo(ServerInfo **info)
    {
        *info = NULL;
        LOG_COMMAND(Command(INFO_REQUEST, common::Value::C_INNER));
        MemcachedServerInfo::Common cm;
        common::ErrorValueSPtr err = impl_->stats(NULL, cm);
        if(!err){
            *info = new MemcachedServerInfo(cm);
        }

        return err;
    }

    void MemcachedDriver::handleConnectEvent(events::ConnectRequestEvent *ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            events::ConnectResponceEvent::value_type res(ev->value());
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
            reply(sender, new events::ConnectResponceEvent(this, res));
        notifyProgress(sender, 100);
    }

    void MemcachedDriver::handleDisconnectEvent(events::DisconnectRequestEvent* ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            events::DisconnectResponceEvent::value_type res(ev->value());
        notifyProgress(sender, 50);

            common::ErrorValueSPtr er = impl_->disconnect();
            if(er){
                res.setErrorInfo(er);
            }

            reply(sender, new events::DisconnectResponceEvent(this, res));
        notifyProgress(sender, 100);
    }

    void MemcachedDriver::handleExecuteEvent(events::ExecuteRequestEvent* ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            events::ExecuteRequestEvent::value_type res(ev->value());
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
                        FastoObjectCommand* cmd = createCommand(outRoot, command, command, common::Value::C_USER);
                        er = impl_->execute(cmd);
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
                LOG_ERROR(er, true);
            }
        notifyProgress(sender, 100);
    }

    void MemcachedDriver::handleLoadDatabaseInfosEvent(events::LoadDatabasesInfoRequestEvent* ev)
    {
        QObject *sender = ev->sender();
    notifyProgress(sender, 0);
        events::LoadDatabasesInfoResponceEvent::value_type res(ev->value());
    notifyProgress(sender, 50);
        res.databases_.push_back(currentDatabaseInfo_);
        reply(sender, new events::LoadDatabasesInfoResponceEvent(this, res));
    notifyProgress(sender, 100);
    }

    void MemcachedDriver::handleLoadDatabaseContentEvent(events::LoadDatabaseContentRequestEvent *ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            events::LoadDatabaseContentResponceEvent::value_type res(ev->value());
            FastoObjectIPtr root = FastoObject::createRoot(GET_KEYS);
        notifyProgress(sender, 50);
            FastoObjectCommand* cmd = createCommand(root, GET_KEYS, "", common::Value::C_INNER);
            common::ErrorValueSPtr er = impl_->execute(cmd);
            if(er){
                res.setErrorInfo(er);
            }
            else{
                FastoObject::child_container_type rchildrens = cmd->childrens();
                if(rchildrens.size()){
                    DCHECK(rchildrens.size() == 1);
                    FastoObjectArray* array = dynamic_cast<FastoObjectArray*>(rchildrens[0]);
                    if(!array){
                        goto done;
                    }
                    common::ArrayValue* ar = array->array();
                    if(!ar){
                        goto done;
                    }

                    for(int i = 0; i < ar->getSize(); ++i)
                    {
                        std::string ress;
                        bool isok = ar->getString(i, &ress);
                        if(isok){
                            res.keys_.push_back(ress);
                        }
                    }
                }
            }
    done:
        notifyProgress(sender, 75);
            reply(sender, new events::LoadDatabaseContentResponceEvent(this, res));
        notifyProgress(sender, 100);
    }

    void MemcachedDriver::handleSetDefaultDatabaseEvent(events::SetDefaultDatabaseRequestEvent* ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            events::SetDefaultDatabaseResponceEvent::value_type res(ev->value());
        notifyProgress(sender, 50);
            reply(sender, new events::SetDefaultDatabaseResponceEvent(this, res));
        notifyProgress(sender, 100);
    }

    void MemcachedDriver::handleLoadServerInfoEvent(events::ServerInfoRequestEvent* ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            events::ServerInfoResponceEvent::value_type res(ev->value());
        notifyProgress(sender, 50);
            LOG_COMMAND(Command(INFO_REQUEST, common::Value::C_INNER));
            MemcachedServerInfo::Common cm;
            common::ErrorValueSPtr err = impl_->stats(NULL, cm);
            if(err){
                res.setErrorInfo(err);
            }
            else{
                ServerInfoSPtr mem(new MemcachedServerInfo(cm));
                res.setInfo(mem);
            }
        notifyProgress(sender, 75);
            reply(sender, new events::ServerInfoResponceEvent(this, res));
        notifyProgress(sender, 100);
    }

    void MemcachedDriver::handleLoadServerPropertyEvent(events::ServerPropertyInfoRequestEvent* ev)
    {

    }

    void MemcachedDriver::handleServerPropertyChangeEvent(events::ChangeServerPropertyInfoRequestEvent* ev)
    {

    }

    void MemcachedDriver::handleDbValueChangeEvent(events::ChangeDbValueRequestEvent* ev)
    {

    }

    void MemcachedDriver::handleProcessCommandLineArgs(events::ProcessConfigArgsRequestEvent* ev)
    {

    }

    void MemcachedDriver::handleShutdownEvent(events::ShutDownRequestEvent* ev)
    {

    }

    void MemcachedDriver::handleBackupEvent(events::BackupRequestEvent* ev)
    {

    }

    void MemcachedDriver::handleExportEvent(events::ExportRequestEvent* ev)
    {

    }

    void MemcachedDriver::handleCommandRequestEvent(events::CommandRequestEvent* ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            events::CommandResponceEvent::value_type res(ev->value());
        notifyProgress(sender, 50);
            reply(sender, new events::CommandResponceEvent(this, res));
        notifyProgress(sender, 100);
    }

    ServerInfoSPtr MemcachedDriver::makeServerInfoFromString(const std::string& val)
    {
        ServerInfoSPtr res(makeMemcachedServerInfo(val));
        return res;
    }
}
