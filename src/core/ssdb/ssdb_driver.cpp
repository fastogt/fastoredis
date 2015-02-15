#include "core/ssdb/ssdb_driver.h"

extern "C" {
#include "third-party/redis/deps/hiredis/sds.h"
}

#include "common/sprintf.h"
#include "common/utils.h"

#include "core/logger.h"
#include "core/command_logger.h"

#include "core/ssdb/ssdb_config.h"
#include "core/ssdb/ssdb_infos.h"

#include "SSDB.h"

#define INFO_REQUEST "STATS"
#define GET_KEYS "STATS ITEMS"
#define DELETE_KEY "DELETE"
#define LOAD_KEY "GET"

namespace
{
    std::vector<std::pair<std::string, std::string > > oppositeCommands = { {"GET", "SET"} };
}

namespace fastoredis
{
    namespace
    {
        class SsdbCommand
                : public FastoObjectCommand
        {
        public:
            SsdbCommand(FastoObject* parent, common::CommandValue* cmd, const std::string &delemitr)
                : FastoObjectCommand(parent, cmd, delemitr)
            {

            }

            virtual std::string inputCmd() const
            {
                common::CommandValue* command = cmd();
                if(command){
                    std::pair<std::string, std::string> kv = getKeyValueFromLine(command->inputCommand());
                    return kv.first;
                }

                return std::string();
            }

            virtual std::string inputArgs() const
            {
                common::CommandValue* command = cmd();
                if(command){
                    std::pair<std::string, std::string> kv = getKeyValueFromLine(command->inputCommand());
                    return kv.second;
                }

                return std::string();
            }
        };

        FastoObjectCommand* createCommand(FastoObject* parent, const std::string& input, common::Value::CommandType ct)
        {
            DCHECK(parent);
            std::pair<std::string, std::string> kv = getKeyValueFromLine(input);
            std::string opposite = getOppositeCommand(kv.first, oppositeCommands);
            if(!opposite.empty()){
                opposite += " " + kv.second;
            }
            common::CommandValue* cmd = common::Value::createCommand(input, opposite, ct);
            FastoObjectCommand* fs = new SsdbCommand(parent, cmd, "");
            parent->addChildren(fs);
            return fs;
        }

        FastoObjectCommand* createCommand(FastoObjectIPtr parent, const std::string& input, common::Value::CommandType ct)
        {
            return createCommand(parent.get(), input, ct);
        }
    }

    struct SsdbDriver::pimpl
    {
        pimpl()
            : ssdb_(NULL)
        {

        }

        bool isConnected() const
        {
            if(!ssdb_){
                return false;
            }

            return true;
        }

        common::ErrorValueSPtr connect()
        {
            if(isConnected()){
                return common::ErrorValueSPtr();
            }

            clear();
            init();

            ssdb_ = ssdb::Client::connect(config_.hostip, config_.hostport);
            if (!ssdb_){
                return common::make_error_value("Fail to connect to server!", common::ErrorValue::E_ERROR);
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

        common::ErrorValueSPtr stats(const char* args, SsdbServerInfo::Common& statsout)
        {
            /*Ssdb_return_t error;
            Ssdb_stat_st* st = Ssdb_stat(memc_, (char*)args, &error);
            if (error != Ssdb_SUCCESS){
                char buff[1024] = {0};
                sprintf(buff, "Stats function error: %s", Ssdb_strerror(memc_, error));
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

            Ssdb_stat_free(NULL, st);*/
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

        ssdbConfig config_;
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

                common::ErrorValueSPtr er = set(argv[1], argv[2]);
                if(!er){
                    common::StringValue *val = common::Value::createStringValue("STORED");
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
            std::string ex;
            ssdb::Status st = ssdb_->get(key, &ex);
            if (st.error()){
                char buff[1024] = {0};
                common::SNPrintf(buff, sizeof(buff), "Set function error: %s", st.code());
                return common::make_error_value(buff, common::ErrorValue::E_ERROR);
            }
            ret_val = ex;
            return common::ErrorValueSPtr();
        }

        common::ErrorValueSPtr set(const std::string& key, const std::string& value)
        {
            ssdb::Status st = ssdb_->set(key, value);
            if (st.error()){
                char buff[1024] = {0};
                common::SNPrintf(buff, sizeof(buff), "Get function error: %s", st.code());
                return common::make_error_value(buff, common::ErrorValue::E_ERROR);
            }
            return common::ErrorValueSPtr();
        }

        void init()
        {

        }

        void clear()
        {
            delete ssdb_;
            ssdb_ = NULL;
        }

        ssdb::Client* ssdb_;
    };

    SsdbDriver::SsdbDriver(const IConnectionSettingsBaseSPtr &settings)
        : IDriver(settings), impl_(new pimpl)
    {

    }

    SsdbDriver::~SsdbDriver()
    {
        delete impl_;
    }

    bool SsdbDriver::isConnected() const
    {
        return impl_->isConnected();
    }

    void SsdbDriver::interrupt()
    {
        impl_->config_.shutdown = 1;
    }

    std::string SsdbDriver::commandByType(CommandKey::cmdtype type)
    {
        if(type == CommandKey::C_LOAD){
            return LOAD_KEY;
        }
        else if(type == CommandKey::C_DELETE){
            return DELETE_KEY;
        }
        else{
            return std::string();
        }
    }

    common::net::hostAndPort SsdbDriver::address() const
    {
        return common::net::hostAndPort(impl_->config_.hostip, impl_->config_.hostport);
    }

    std::string SsdbDriver::version() const
    {
        return versionApi();
    }

    std::string SsdbDriver::outputDelemitr() const
    {
        return impl_->config_.mb_delim;
    }

    const char* SsdbDriver::versionApi()
    {
        return "unknown";
    }

    void SsdbDriver::customEvent(QEvent *event)
    {
        IDriver::customEvent(event);
        impl_->config_.shutdown = 0;
    }

    void SsdbDriver::initImpl()
    {
        currentDatabaseInfo_.reset(new SsdbDataBaseInfo("0", 0, true));
    }

    void SsdbDriver::clearImpl()
    {

    }

    common::ErrorValueSPtr SsdbDriver::currentLoggingInfo(ServerInfo **info)
    {
        *info = NULL;
        LOG_COMMAND(Command(INFO_REQUEST, common::Value::C_INNER));
        SsdbServerInfo::Common cm;
        common::ErrorValueSPtr err = impl_->stats(NULL, cm);
        if(!err){
            *info = new SsdbServerInfo(cm);
        }

        return err;
    }

    void SsdbDriver::handleConnectEvent(events::ConnectRequestEvent *ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            events::ConnectResponceEvent::value_type res(ev->value());
            SsdbConnectionSettings *set = dynamic_cast<SsdbConnectionSettings*>(settings_.get());
            if(set){
                impl_->config_ = set->info();
                impl_->sinfo_ = set->sshInfo();
        notifyProgress(sender, 25);
                    common::ErrorValueSPtr er = impl_->connect();
                    if(er){
                        res.setErrorInfo(er);
                    }
        notifyProgress(sender, 75);
            }
            reply(sender, new events::ConnectResponceEvent(this, res));
        notifyProgress(sender, 100);
    }

    void SsdbDriver::handleDisconnectEvent(events::DisconnectRequestEvent* ev)
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

    void SsdbDriver::handleExecuteEvent(events::ExecuteRequestEvent* ev)
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
                        FastoObjectCommand* cmd = createCommand(outRoot, command, common::Value::C_USER);
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

    void SsdbDriver::handleLoadDatabaseInfosEvent(events::LoadDatabasesInfoRequestEvent* ev)
    {
        QObject *sender = ev->sender();
    notifyProgress(sender, 0);
        events::LoadDatabasesInfoResponceEvent::value_type res(ev->value());
    notifyProgress(sender, 50);
        res.databases_.push_back(currentDatabaseInfo_);
        reply(sender, new events::LoadDatabasesInfoResponceEvent(this, res));
    notifyProgress(sender, 100);
    }

    void SsdbDriver::handleLoadDatabaseContentEvent(events::LoadDatabaseContentRequestEvent *ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            events::LoadDatabaseContentResponceEvent::value_type res(ev->value());
            FastoObjectIPtr root = FastoObject::createRoot(GET_KEYS);
        notifyProgress(sender, 50);
            FastoObjectCommand* cmd = createCommand(root, GET_KEYS, common::Value::C_INNER);
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

    void SsdbDriver::handleSetDefaultDatabaseEvent(events::SetDefaultDatabaseRequestEvent* ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            events::SetDefaultDatabaseResponceEvent::value_type res(ev->value());
        notifyProgress(sender, 50);
            reply(sender, new events::SetDefaultDatabaseResponceEvent(this, res));
        notifyProgress(sender, 100);
    }

    void SsdbDriver::handleLoadServerInfoEvent(events::ServerInfoRequestEvent* ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            events::ServerInfoResponceEvent::value_type res(ev->value());
        notifyProgress(sender, 50);
            LOG_COMMAND(Command(INFO_REQUEST, common::Value::C_INNER));
            SsdbServerInfo::Common cm;
            common::ErrorValueSPtr err = impl_->stats(NULL, cm);
            if(err){
                res.setErrorInfo(err);
            }
            else{
                ServerInfoSPtr mem(new SsdbServerInfo(cm));
                res.setInfo(mem);
            }
        notifyProgress(sender, 75);
            reply(sender, new events::ServerInfoResponceEvent(this, res));
        notifyProgress(sender, 100);
    }

    void SsdbDriver::handleLoadServerPropertyEvent(events::ServerPropertyInfoRequestEvent* ev)
    {

    }

    void SsdbDriver::handleServerPropertyChangeEvent(events::ChangeServerPropertyInfoRequestEvent* ev)
    {

    }

    void SsdbDriver::handleDbValueChangeEvent(events::ChangeDbValueRequestEvent* ev)
    {

    }

    void SsdbDriver::handleProcessCommandLineArgs(events::ProcessConfigArgsRequestEvent* ev)
    {

    }

    void SsdbDriver::handleShutdownEvent(events::ShutDownRequestEvent* ev)
    {

    }

    void SsdbDriver::handleBackupEvent(events::BackupRequestEvent* ev)
    {

    }

    void SsdbDriver::handleExportEvent(events::ExportRequestEvent* ev)
    {

    }

    void SsdbDriver::handleCommandRequestEvent(events::CommandRequestEvent* ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            events::CommandResponceEvent::value_type res(ev->value());
            std::string cmdtext;
            CommandKey::cmdtype t =  res.cmd_.type();
            if( t == CommandKey::C_DELETE){
                cmdtext = std::string(DELETE_KEY) + " " + res.cmd_.key();
            }
            else if(t == CommandKey::C_LOAD){
                cmdtext = std::string(LOAD_KEY) + " " + res.cmd_.key();
            }
            FastoObjectIPtr root = FastoObject::createRoot(cmdtext);
            FastoObjectCommand* cmd = createCommand(root, cmdtext, common::Value::C_INNER);
        notifyProgress(sender, 50);
            common::ErrorValueSPtr er = impl_->execute(cmd);
            if(er){
                res.setErrorInfo(er);
            }
            else{
                res.cmd_.setExecCommand(cmdtext);
            }
            reply(sender, new events::CommandResponceEvent(this, res));
        notifyProgress(sender, 100);
    }

    ServerInfoSPtr SsdbDriver::makeServerInfoFromString(const std::string& val)
    {
        ServerInfoSPtr res(makeSsdbServerInfo(val));
        return res;
    }
}
