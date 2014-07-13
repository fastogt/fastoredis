#include "core/redis/redis_driver.h"

#include <hiredis/hiredis.h>
#include <errno.h>

extern "C" {
#include <anet.h>
#include <help.h>
#include <sds.h>
#include <version.h>
#include <release.h>
}

#include "common/qt/convert_string.h"
#include "core/command_logger.h"
#include "common/utils.h"

#define REDIS_CLI_KEEPALIVE_INTERVAL 15 /* seconds */
#define CLI_HELP_COMMAND 1
#define CLI_HELP_GROUP 2
#define OUTPUT_STANDARD 0
#define OUTPUT_RAW 1
#define OUTPUT_CSV 2
#define INFO_REQUEST "INFO"

namespace
{
    common::Value::Type toValueTypeFromRedis(int redisT)
    {
        common::Value::Type res;
        return res;
    }

    const char *toCString(const std::string &val)
    {
        return val.empty() ? NULL : val.c_str();
    }

    char *redisGitSHA1(void) {
        return REDIS_GIT_SHA1;
    }

    char *redisGitDirty(void) {
        return REDIS_GIT_DIRTY;
    }

    sds cliVersion() {
        sds version;
        version = sdscatprintf(sdsempty(), "%s", REDIS_VERSION);

        /* Add git commit and working tree status when available */
        if (strtoll(redisGitSHA1(),NULL,16)) {
            version = sdscatprintf(version, " (git:%s", redisGitSHA1());
            if (strtoll(redisGitDirty(),NULL,10))
                version = sdscatprintf(version, "-dirty");
            version = sdscat(version, ")");
        }
        return version;
    }

    typedef struct {
        int type;
        int argc;
        sds *argv;
        sds full;

        /* Only used for help on commands */
        struct commandHelp *org;
    } helpEntry;

    helpEntry *helpEntries;
    int helpEntriesLen = sizeof(commandHelp)/sizeof(struct commandHelp) + sizeof(commandGroups)/sizeof(char*);

    QStringList g_types;
    QStringList g_commands;

    QStringList getList()
    {
        int pos = 0;

        helpEntry tmp;
        helpEntries = (helpEntry*)malloc(sizeof(helpEntry)*helpEntriesLen);

        QStringList list;
        for(int i = 0; i < sizeof(commandGroups)/sizeof(char*); ++i){
            char* command = commandGroups[i];
            QString qcommand = common::convertfromString<QString>(std::string(command));
            g_types.append(qcommand);
            list.append(qcommand);
            tmp.argc = 1;
            tmp.argv = (sds*)malloc(sizeof(sds));
            tmp.argv[0] = sdscatprintf(sdsempty(),"@%s",commandGroups[i]);
            tmp.full = tmp.argv[0];
            tmp.type = CLI_HELP_GROUP;
            tmp.org = NULL;
            helpEntries[pos++] = tmp;
        }
        for(int i = 0; i < sizeof(commandHelp)/sizeof(struct commandHelp); ++i){
            struct commandHelp command = commandHelp[i];
            std::string commandN = command.name;
            QString qCommandN = common::convertfromString<QString>(commandN);
            g_commands.append(qCommandN);
            list.append(qCommandN);

            tmp.argv = sdssplitargs(commandHelp[i].name,&tmp.argc);
            tmp.full = sdsnew(commandHelp[i].name);
            tmp.type = CLI_HELP_COMMAND;
            tmp.org = &commandHelp[i];
            helpEntries[pos++] = tmp;
        }
        return list;
    }

    fastoredis::ServerInfo makeServerInfo(const fastoredis::FastoObjectPtr &root)
    {
        static const common::unicode_string headers[8] = {UTEXT("# Server"), UTEXT("# Clients"), UTEXT("# Memory"),
                                                          UTEXT("# Persistence"), UTEXT("# Stats"),
                                                          UTEXT("# Replication"), UTEXT("# CPU"), UTEXT("# Keyspace")};
        using namespace fastoredis;
        ServerInfo result;
        const common::unicode_string content = common::convert2string(root);
        int j = 0;
        std::string word;
        size_t pos = 0;
        for(int i = 0; i < content.size(); ++i)
        {
            char ch = content[i];
            word += ch;
            if(word == headers[j]){
                if(j+1 != sizeof(headers)/sizeof(*headers)){
                    pos = content.find(headers[j+1], pos);
                }
                else{
                    break;
                }
                if(pos != std::string::npos)
                {
                    common::unicode_string part = content.substr(i + 1, pos - i - 1 );
                    switch(j)
                    {
                    case 0:
                        result.server_ = fastoredis::ServerInfo::Server(part);
                        break;
                    case 1:
                        result.clients_ = fastoredis::ServerInfo::Clients(part);
                        break;
                    case 2:
                        result.memory_ = fastoredis::ServerInfo::Memory(part);
                        break;
                    case 3:
                        result.persistence_ = fastoredis::ServerInfo::Persistence(part);
                        break;
                    case 4:
                        result.stats_ = fastoredis::ServerInfo::Stats(part);
                        break;
                    case 5:
                        result.replication_ = fastoredis::ServerInfo::Replication(part);
                        break;
                    case 6:
                        result.cpu_ = fastoredis::ServerInfo::Cpu(part);
                        break;
                    default:
                        break;
                    }
                    i = pos-1;
                    ++j;
                }
                word.clear();
            }
        }
        return result;
    }


    fastoredis::ServerPropertyInfo makeServerProperty(const fastoredis::FastoObjectPtr &root)
    {
        fastoredis::ServerPropertyInfo inf;
        fastoredis::FastoObject::child_container_type childrens = root->childrens();
        for(int i = 0; i < childrens.size(); i+=2)
        {
            inf.propertyes_.push_back(std::make_pair(childrens[i]->toString(), childrens[i+1]->toString()));
        }
        return inf;
    }

    const QStringList g_allCommands = getList();
}


namespace fastoredis
{
    struct RedisDriver::pimpl

    {
        pimpl(): interrupt_(false), context(NULL), timer_info_id_(0)
        {

        }

        ~pimpl()
        {
            if(context){
                redisFree(context);
                context = NULL;
            }
        }
        volatile bool interrupt_;
        redisContext *context;
        redisConfig config;
        int timer_info_id_;

        int cliAuth(common::ErrorValue& er) {
            redisReply *reply;
            if (config.auth.empty()) return REDIS_OK;

            reply = static_cast<redisReply*>(redisCommand(context,"AUTH %s",config.auth.c_str()));
            if (reply != NULL) {
                freeReplyObject(reply);
                return REDIS_OK;
            }            
            cliPrintContextError(er);
            return REDIS_ERR;
        }
        int cliSelect(common::ErrorValue& er) {
            redisReply *reply;
            if (config.dbnum == 0) return REDIS_OK;

            reply = static_cast<redisReply*>(redisCommand(context,"SELECT %d",config.dbnum));
            if (reply != NULL) {
                freeReplyObject(reply);
                return REDIS_OK;
            }
            cliPrintContextError(er);
            return REDIS_ERR;
        }
        int cliConnect(int force, common::ErrorValue& er)
        {
            if (context == NULL || force) {
                if (context != NULL)
                    redisFree(context);

                if (config.hostsocket.empty()) {
                    context = redisConnect(config.hostip.c_str(),config.hostport);
                } else {
                    context = redisConnectUnix(config.hostsocket.c_str());
                }

                if (context->err) {
                    char buff[512] = {0};
                    if (config.hostsocket.empty())
                        sprintf(buff,"Could not connect to Redis at %s:%d: %s\n",config.hostip.c_str(),config.hostport,context->errstr);
                    else
                        sprintf(buff,"Could not connect to Redis at %s: %s\n",config.hostsocket.c_str(),context->errstr);
                    er = common::ErrorValue(buff, common::ErrorValue::E_ERROR);
                    redisFree(context);
                    context = NULL;
                    return REDIS_ERR;
                }

                /* Set aggressive KEEP_ALIVE socket option in the Redis context socket
                 * in order to prevent timeouts caused by the execution of long
                 * commands. At the same time this improves the detection of real
                 * errors. */
                anetKeepAlive(NULL, context->fd, REDIS_CLI_KEEPALIVE_INTERVAL);

                /* Do AUTH and select the right DB. */
                if (cliAuth(er) != REDIS_OK)
                    return REDIS_ERR;
                if (cliSelect(er) != REDIS_OK)
                    return REDIS_ERR;
            }
            return REDIS_OK;
        }

        std::string currentaddress() const
        {
            if(!context){
                return "not connected";
            }
            else{
                char address[512] = {0};
                sprintf(address, "%s:%d", config.hostip.c_str(), config.hostport);
                return address;
            }
        }

        void cliRefreshPrompt(void) {
            int len;

            if (!config.hostsocket.empty())
                len = snprintf(config.prompt,sizeof(config.prompt),"redis %s",
                               config.hostsocket.c_str());
            else
                len = snprintf(config.prompt,sizeof(config.prompt),
                               strchr(config.hostip.c_str(),':') ? "[%s]:%d" : "%s:%d",
                               config.hostip.c_str(), config.hostport);
            /* Add [dbnum] if needed */
            if (config.dbnum != 0)
                len += snprintf(config.prompt+len,sizeof(config.prompt)-len,"[%d]",
                    config.dbnum);
            snprintf(config.prompt+len,sizeof(config.prompt)-len,"> ");
        }

        void cliPrintContextError(common::ErrorValue& er) {
            if (context == NULL) return;
            char buff[512] = {0};
            sprintf(buff,"Error: %s\n",context->errstr);
            er = common::ErrorValue(buff, common::ErrorValue::E_ERROR);
        }

        void cliFormatReplyRaw(FastoObjectPtr &out, redisReply *r) {
            switch (r->type) {
            case REDIS_REPLY_NIL:
                break;
            case REDIS_REPLY_ERROR:
            case REDIS_REPLY_STATUS:
            case REDIS_REPLY_STRING:
            {
                common::StringValue *val =common::Value::createStringValue(r->str);
                FastoObject *obj = new FastoObject(out, val);
                out->addChildren(obj);
                break;
            }
            case REDIS_REPLY_INTEGER:
            {
                common::FundamentalValue *val =common::Value::createIntegerValue(r->integer);
                out->addChildren(new FastoObject(out, val));
                break;
            }
            case REDIS_REPLY_ARRAY:
            {
                FastoObjectPtr child;
                if(out->isRoot()){
                    child = out;
                }
                else{
                    common::ArrayValue *val =common::Value::createArrayValue();
                    val->appendString(out->toString());
                    child = new FastoObject(out,val);
                    out->addChildren(child);
                }
                for (size_t i = 0; i < r->elements; i++) {
                    cliFormatReplyRaw(child, r->element[i]);
                }
                break;
            }
            default:
                {
                    char tmp2[128] = {0};
                    sprintf(tmp2 ,"Unknown reply type: %d", r->type);
                    common::ErrorValue *val =common::Value::createErrorValue(tmp2, common::ErrorValue::E_NONE, common::logging::WARNING);
                    out->addChildren(new FastoObject(out, val));
                }
            }
        }

        void cliOutputCommandHelp(FastoObjectPtr &out, struct commandHelp *help, int group) {
            char buff[1024] = {0};
            sprintf(buff,"\r\n  name: %s %s\r\n  summary: %s\r\n  since: %s", help->name, help->params, help->summary, help->since);
            common::StringValue *val =common::Value::createStringValue(buff);
            out->addChildren(new FastoObject(out, val));
            if (group) {
                char buff2[1024] = {0};
                sprintf(buff2,"  group: %s", commandGroups[help->group]);
                val =common::Value::createStringValue(buff2);
                out->addChildren(new FastoObject(out, val));
            }
        }

        void cliOutputGenericHelp(FastoObjectPtr &out) {
            sds version = cliVersion();
            char buff[512] = {0};
            sprintf(buff,
                PROJECT_NAME" based on redis-cli %s\r\n"
                "Type: \"help @<group>\" to get a list of commands in <group>\r\n"
                "      \"help <command>\" for help on <command>\r\n"
                "      \"help <tab>\" to get a list of possible help topics\r\n"
                "      \"quit\" to exit",
                version
            );
            common::StringValue *val =common::Value::createStringValue(buff);
            out->addChildren(new FastoObject(out, val));
            sdsfree(version);
        }

        void cliOutputHelp(FastoObjectPtr &out, int argc, char **argv) {
            int i, j, len;
            int group = -1;
            helpEntry *entry;
            struct commandHelp *help;

            if (argc == 0) {
                cliOutputGenericHelp(out);
                return;
            } else if (argc > 0 && argv[0][0] == '@') {
                len = sizeof(commandGroups)/sizeof(char*);
                for (i = 0; i < len; i++) {
                    if (strcasecmp(argv[0]+1,commandGroups[i]) == 0) {
                        group = i;
                        break;
                    }
                }
            }

            assert(argc > 0);
            for (i = 0; i < helpEntriesLen; i++) {
                entry = &helpEntries[i];
                if (entry->type != CLI_HELP_COMMAND) continue;

                help = entry->org;
                if (group == -1) {
                    /* Compare all arguments */
                    if (argc == entry->argc) {
                        for (j = 0; j < argc; j++) {
                            if (strcasecmp(argv[j],entry->argv[j]) != 0) break;
                        }
                        if (j == argc) {
                            cliOutputCommandHelp(out, help,1);
                        }
                    }
                } else {
                    if (group == help->group) {
                        cliOutputCommandHelp(out, help,0);
                    }
                }
            }
        }

        int cliReadReply(FastoObjectPtr &out, common::ErrorValue& er) {
            void *_reply;
            redisReply *reply;

            if (redisGetReply(context,&_reply) != REDIS_OK) {
                if (config.shutdown)
                    return REDIS_OK;

                /* Filter cases where we should reconnect */
                if (context->err == REDIS_ERR_IO && errno == ECONNRESET)
                    return REDIS_ERR;
                if (context->err == REDIS_ERR_EOF)
                    return REDIS_ERR;

                cliPrintContextError(er);
                return REDIS_ERR; /* avoid compiler warning */
            }

            reply = static_cast<redisReply*>(_reply);

            if (config.cluster_mode && reply->type == REDIS_REPLY_ERROR &&
                (!strncmp(reply->str,"MOVED",5) || !strcmp(reply->str,"ASK")))
            {
                char *p = reply->str, *s;
                int slot;

                s = strchr(p,' ');      /* MOVED[S]3999 127.0.0.1:6381 */
                p = strchr(s+1,' ');    /* MOVED[S]3999[P]127.0.0.1:6381 */
                *p = '\0';
                slot = atoi(s+1);
                s = strchr(p+1,':');    /* MOVED 3999[P]127.0.0.1[S]6381 */
                *s = '\0';                
                config.hostip = std::string(p+1);
                config.hostport = atoi(s+1);                
                char redir[512] = {0};
                sprintf(redir, "-> Redirected to slot [%d] located at %s:%d", slot, config.hostip.c_str(), config.hostport);
                common::StringValue *val =common::Value::createStringValue(redir);
                out->addChildren(new FastoObject(out, val));
                config.cluster_reissue_command = 1;
                cliRefreshPrompt();
            }
            else{
                cliFormatReplyRaw(out, reply);
            }

            freeReplyObject(reply);
            return REDIS_OK;
        }

        int cliSendCommand(FastoObjectPtr &out, common::ErrorValue& er, int argc, char **argv, int repeat)
        {
            char *command = argv[0];
            size_t *argvlen;

            if (!strcasecmp(command,"help") || !strcasecmp(command,"?")) {
                cliOutputHelp(out, --argc, ++argv);
                return REDIS_OK;
            }

            if (context == NULL)
                return REDIS_ERR;

            if (!strcasecmp(command,"shutdown")) config.shutdown = 1;
            if (!strcasecmp(command,"monitor")) config.monitor_mode = 1;
            if (!strcasecmp(command,"subscribe") ||
                !strcasecmp(command,"psubscribe")) config.pubsub_mode = 1;

            /* Setup argument length */
            argvlen = static_cast<size_t*>(malloc(argc*sizeof(size_t)));
            for (int j = 0; j < argc; j++){
                argvlen[j] = sdslen(argv[j]);
            }

            while(repeat--) {
                redisAppendCommandArgv(context,argc,(const char**)argv,argvlen);
                while (config.monitor_mode) {
                    if (cliReadReply(out, er) != REDIS_OK){
                        return REDIS_ERR;
                    }
                }

                if (config.pubsub_mode) {
                    if (config.output != OUTPUT_RAW)
                    while (1) {
                        if (cliReadReply(out, er) != REDIS_OK)
                            return REDIS_ERR;
                    }
                }

                if (cliReadReply(out, er) != REDIS_OK) {
                    free(argvlen);
                    return REDIS_ERR;
                } else {
                    if (!strcasecmp(command,"select") && argc == 2) {
                        config.dbnum = atoi(argv[1]);
                        cliRefreshPrompt();
                    }
                }
            }

            free(argvlen);
            return REDIS_OK;
        }

        void repl_impl(FastoObjectPtr &out, common::ErrorValue &er) {
            const char *command = out->toString().c_str();
            if (command[0] != '\0') {
                int argc;
                sds *argv = sdssplitargs(command,&argc);

                if (!argv) {
                    common::StringValue *val =common::Value::createStringValue("Invalid argument(s)");
                    out->addChildren(new FastoObject(out, val));
                }
                else if (argc > 0)
                {
                    if (strcasecmp(argv[0], "quit") == 0 ||
                    strcasecmp(argv[0], "exit") == 0)
                    {
                        config.shutdown = 1;
                    } else if (argc == 3 && !strcasecmp(argv[0],"connect")) {
                        config.hostip = argv[1];
                        config.hostport = atoi(argv[2]);
                        cliRefreshPrompt();
                        cliConnect(1, er);
                    } else {
                        int repeat, skipargs = 0;

                        repeat = atoi(argv[0]);
                        if (argc > 1 && repeat) {
                            skipargs = 1;
                        } else {
                            repeat = 1;
                        }

                        while (1) {
                            config.cluster_reissue_command = 0;
                            if (cliSendCommand(out, er, argc-skipargs,argv+skipargs,repeat)
                            != REDIS_OK)
                            {
                                cliConnect(1, er);

                            /* If we still cannot send the command print error.
                            * We'll try to reconnect the next time. */
                            if (cliSendCommand(out, er, argc-skipargs,argv+skipargs,repeat)
                            != REDIS_OK)
                                cliPrintContextError(er);
                            }
                            /* Issue the command again if we got redirected in cluster mode */
                            if (config.cluster_mode && config.cluster_reissue_command) {
                                cliConnect(1, er);
                            } else {
                                break;
                            }
                        }
                    }
                }
                sdsfreesplitres(argv,argc);
            }
        }
    };

    RedisDriver::RedisDriver(const IConnectionSettingsBasePtr &settings)
        : IDriver(settings), impl_(new pimpl)
    {

    }

    const QStringList &RedisDriver::allCommands()
    {
        return g_allCommands;
    }

    const QStringList &RedisDriver::typesKeywords()
    {
        return g_types;
    }

    const QStringList &RedisDriver::commandsKeywords()
    {
        return g_commands;
    }

    std::string RedisDriver::address() const
    {
        return impl_->currentaddress();
    }

    bool RedisDriver::isConnected() const
    {
        return impl_->context;
    }

    void RedisDriver::customEvent(QEvent *event)
    {
        IDriver::customEvent(event);
        impl_->interrupt_ = false;
    }

    void RedisDriver::timerEvent(QTimerEvent * event)
    {
        if(impl_->timer_info_id_ == event->timerId() && isConnected()){            
            FastoObjectPtr root = FastoObject::createRoot(INFO_REQUEST);
            common::ErrorValue er;
            impl_->repl_impl(root, er);
            if(!er.isError()){
                ServerInfo inf = makeServerInfo(root);

            }
        }
    }

    void RedisDriver::initImpl()
    {
        impl_->timer_info_id_ = startTimer(1000);
        DCHECK(impl_->timer_info_id_);
    }

    RedisDriver::~RedisDriver()
    {
        killTimer(impl_->timer_info_id_);
        impl_->timer_info_id_ = 0;
    }

    void RedisDriver::connectEvent(Events::ConnectRequestEvent *ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            Events::ConnectResponceEvent::value_type res(ev->value());
            RedisConnectionSettings *set = dynamic_cast<RedisConnectionSettings*>(settings_.get());
            if(set){
                impl_->config = set->info();
                common::ErrorValue er;
        notifyProgress(sender, 25);
                    if(impl_->interrupt_){
                        res.setErrorInfo(common::ErrorValue("Interrupted connect.", common::ErrorValue::E_INTERRUPTED));
                    }
                    else if(impl_->cliConnect(0, er) == REDIS_ERR){
                        res.setErrorInfo(er);
                    }
        notifyProgress(sender, 75);
                impl_->cliRefreshPrompt();
            }
            reply(sender, new Events::ConnectResponceEvent(this, res));
        notifyProgress(sender, 100);
    }

    void RedisDriver::executeEvent(Events::ExecuteRequestEvent *ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            Events::ExecuteResponceEvent::value_type res(ev->value());
            const char *inputLine = toCString(res._text);

            common::ErrorValue er;
            if(inputLine){
                impl_->cliRefreshPrompt();

                size_t length = strlen(inputLine);
                int offset = 0;
                res._out = FastoObject::createRoot(inputLine);
                double step = 100.0f/length;
                for(size_t n = 0; n < length; ++n){
                    if(impl_->interrupt_){
                        res.setErrorInfo(common::ErrorValue("Interrupted exec.", common::ErrorValue::E_INTERRUPTED));
                        break;
                    }
                    if(inputLine[n] == '\n' || n == length-1){
        notifyProgress(sender, step*n);
                        char command[128] = {0};
                        if(n == length-1){
                            strcpy(command, inputLine + offset);
                        }
                        else{
                            strncpy(command, inputLine + offset, n - offset);
                        }
                        offset = n + 1;
                        common::StringValue *val =common::Value::createStringValue(command);
                        FastoObjectPtr child = new FastoObject(res._out, val);
                        res._out->addChildren(child);
                        LOG_COMMAND(Command(command,Command::UserCommand));
                        impl_->repl_impl(child, er);
                    }
                }
            }
            else{
                res.setErrorInfo(common::ErrorValue("Empty command line.", common::ErrorValue::E_ERROR));
            }            
            reply(sender, new Events::ExecuteResponceEvent(this, res));
        notifyProgress(sender, 100);
    }

    void RedisDriver::disconnectEvent(Events::DisconnectRequestEvent *ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            Events::DisconnectResponceEvent::value_type res(ev->value());
            redisFree(impl_->context);
        notifyProgress(sender, 50);
            impl_->context = NULL;
            reply(sender, new Events::DisconnectResponceEvent(this, res));
        notifyProgress(sender, 100);
    }

    void RedisDriver::loadDatabaseInfosEvent(Events::LoadDatabasesInfoRequestEvent *ev)
    {
        static const char* loadDabasesString = "CONFIG GET databases";
            QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            Events::LoadDatabasesInfoResponceEvent::value_type res(ev->value());
            FastoObjectPtr root = FastoObject::createRoot(loadDabasesString);
            common::ErrorValue er;
        notifyProgress(sender, 50);
            LOG_COMMAND(Command(loadDabasesString));
            impl_->repl_impl(root, er);
            if(er.isError()){
                res.setErrorInfo(er);
            }else{
                FastoObject::child_container_type childrens = root->childrens();
                for(int i = 0; i < childrens.size(); ++i){
                    DataBaseInfo dbInf(childrens[i]->toString(), 0);
                    res.databases_.push_back(dbInf);
                }
            }
        notifyProgress(sender, 75);
            reply(sender, new Events::LoadDatabasesInfoResponceEvent(this, res));
        notifyProgress(sender, 100);
    }

    void RedisDriver::loadDatabaseContentEvent(Events::LoadDatabaseContentRequestEvent *ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            Events::LoadDatabaseContentResponceEvent::value_type res(ev->value());
        notifyProgress(sender, 50);
            reply(sender, new Events::LoadDatabaseContentResponceEvent(this, res));
        notifyProgress(sender, 100);
    }

    void RedisDriver::loadServerInfoEvent(Events::ServerInfoRequestEvent *ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            Events::ServerInfoResponceEvent::value_type res(ev->value());
            FastoObjectPtr root = FastoObject::createRoot(INFO_REQUEST);
            common::ErrorValue er;
        notifyProgress(sender, 50);
            LOG_COMMAND(Command(INFO_REQUEST));
            impl_->repl_impl(root, er);
            if(er.isError()){
                res.setErrorInfo(er);
            }else{
                res.info_ = makeServerInfo(root);
            }
        notifyProgress(sender, 75);
            reply(sender, new Events::ServerInfoResponceEvent(this, res));
        notifyProgress(sender, 100);
    }

    void RedisDriver::loadServerPropertyEvent(Events::ServerPropertyInfoRequestEvent *ev)
    {
        static const char* propetyString = "CONFIG GET *";
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
        Events::ServerPropertyInfoResponceEvent::value_type res(ev->value());
            FastoObjectPtr root = FastoObject::createRoot(propetyString);
            common::ErrorValue er;
        notifyProgress(sender, 50);
            LOG_COMMAND(Command(propetyString));
            impl_->repl_impl(root, er);
            if(er.isError()){
                res.setErrorInfo(er);
            }else{
                res.info_ = makeServerProperty(root);
            }
        notifyProgress(sender, 75);
            reply(sender, new Events::ServerPropertyInfoResponceEvent(this, res));
        notifyProgress(sender, 100);
    }

    void RedisDriver::serverPropertyChangeEvent(Events::ChangeServerPropertyInfoRequestEvent *ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
        Events::ChangeServerPropertyInfoResponceEvent::value_type res(ev->value());
            common::ErrorValue er;
        notifyProgress(sender, 50);
        const std::string &changeRequest = "CONFIG SET " + res.newItem_.first + " " + res.newItem_.second;
        FastoObjectPtr root = FastoObject::createRoot(changeRequest);
            LOG_COMMAND(Command(changeRequest));
            impl_->repl_impl(root, er);
            if(er.isError()){
                res.setErrorInfo(er);
            }else{
                res.isChange_ = true;
            }
        notifyProgress(sender, 75);
            reply(sender, new Events::ChangeServerPropertyInfoResponceEvent(this, res));
        notifyProgress(sender, 100);
    }

    void RedisDriver::interrupt()
    {
        impl_->interrupt_ = true;
    }
}
