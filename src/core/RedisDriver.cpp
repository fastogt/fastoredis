#include "core/RedisDriver.h"

#include <hiredis/hiredis.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <anet.h>
#include <help.h>
#include <sds.h>
#include <linenoise/linenoise.h>
#include "version.h"
#include "release.h"
#ifdef __cplusplus
}
#endif

#include "common/qt_helper/converter_patterns.h"

#define REDIS_CLI_KEEPALIVE_INTERVAL 15 /* seconds */
#define REDIS_DEFAULT_PIPE_TIMEOUT 30 /* seconds */
#define CLI_HELP_COMMAND 1
#define CLI_HELP_GROUP 2
#define OUTPUT_STANDARD 0
#define OUTPUT_RAW 1
#define OUTPUT_CSV 2

namespace
{
    const char *toCString(const std::string &val)
    {
        return val.empty() ? NULL : val.c_str();
    }

    long long mstime(void) {
        struct timeval tv;
        long long mst;

        gettimeofday(&tv, NULL);
        mst = ((long long)tv.tv_sec)*1000;
        mst += tv.tv_usec/1000;
        return mst;
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
    int helpEntriesLen;

    QStringList getList()
    {
        static int commandslen = sizeof(commandHelp)/sizeof(struct commandHelp);
        static int groupslen = sizeof(commandGroups)/sizeof(char*);
        int len, pos = 0;

        helpEntry tmp;
        helpEntriesLen = len = commandslen+groupslen;
        helpEntries = (helpEntry*)malloc(sizeof(helpEntry)*len);

        QStringList list;
        for(int i = 0; i < groupslen; ++i){
            char* command = commandGroups[i];
            list.append(common::utils_qt::toQString(std::string(command)));
            tmp.argc = 1;
            tmp.argv = (sds*)malloc(sizeof(sds));
            tmp.argv[0] = sdscatprintf(sdsempty(),"@%s",commandGroups[i]);
            tmp.full = tmp.argv[0];
            tmp.type = CLI_HELP_GROUP;
            tmp.org = NULL;
            helpEntries[pos++] = tmp;
        }
        for(int i = 0; i < commandslen; ++i){
            struct commandHelp command = commandHelp[i];
            list.append(common::utils_qt::toQString(std::string(command.name)));

            tmp.argv = sdssplitargs(commandHelp[i].name,&tmp.argc);
            tmp.full = sdsnew(commandHelp[i].name);
            tmp.type = CLI_HELP_COMMAND;
            tmp.org = &commandHelp[i];
            helpEntries[pos++] = tmp;
        }
        return list;
    }
}

namespace fastoredis
{
    struct RedisDriver::pimpl
    {
        pimpl(): context(NULL)
        {
            config.hostip = sdsnew("127.0.0.1");
            config.hostport = 6379;
            config.hostsocket = NULL;
            config.repeat = 1;
            config.interval = 0;
            config.dbnum = 0;
            config.interactive = 0;
            config.shutdown = 0;
            config.monitor_mode = 0;
            config.pubsub_mode = 0;
            config.latency_mode = 0;
            config.latency_history = 0;
            config.cluster_mode = 0;
            config.slave_mode = 0;
            config.getrdb_mode = 0;
            config.rdb_filename = NULL;
            config.pipe_mode = 0;
            config.pipe_timeout = REDIS_DEFAULT_PIPE_TIMEOUT;
            config.bigkeys = 0;
            config.stdinarg = 0;
            config.auth = NULL;
            config.eval = NULL;
            if (!isatty(fileno(stdout)) && (getenv("FAKETTY") == NULL))
                config.output = OUTPUT_RAW;
            else
                config.output = OUTPUT_STANDARD;
            config.mb_delim = sdsnew("\n");
        }

        void connect(const RedisConnectionSettings::config &conf, EventsInfo::ConnectInfoResponce &res)
        {
            using namespace error;

            hostAndPort h = conf.host;
            config.hostip = const_cast<char*>(toCString(h.first));
            config.hostport = h.second;
            config.hostsocket = const_cast<char*>(toCString(conf.hostsocket));
            config.auth = const_cast<char*>(toCString(conf.auth));
            config.dbnum = conf.dbnum;
            ErrorInfo er;
            if(cliConnect(0, er) == REDIS_ERR){
                res.setErrorInfo(er);
            }
            cliRefreshPrompt();
        }

        void query(EventsInfo::ExecuteInfoResponce &res)
        {
            using namespace error;
            const char *command = toCString(res._text);
            ErrorInfo er;
            std::string out;
            repl(command, out, er);
            res._out = new FastoObject(STRING,out.c_str());
        }

        void disconnect()
        {
            redisFree(context);
            context = NULL;
        }

        ~pimpl()
        {
            if(context){
                redisFree(context);
                context = NULL;
            }
        }

        redisContext *context;
    private:
        struct config {
            char *hostip;
            int hostport;
            char *hostsocket;
            long repeat;
            long interval;
            int dbnum;
            int interactive;
            int shutdown;
            int monitor_mode;
            int pubsub_mode;
            int latency_mode;
            int latency_history;
            int cluster_mode;
            int cluster_reissue_command;
            int slave_mode;
            int pipe_mode;
            int pipe_timeout;
            int getrdb_mode;
            int stat_mode;
            char *rdb_filename;
            int bigkeys;
            int stdinarg; /* get last arg from stdin. (-x option) */
            char *auth;
            int output; /* output mode, see OUTPUT_* defines */
            sds mb_delim;
            char prompt[128];
            char *eval;
        } config;

        int cliAuth(error::ErrorInfo& er) {
            redisReply *reply;
            if (config.auth == NULL) return REDIS_OK;

            reply = static_cast<redisReply*>(redisCommand(context,"AUTH %s",config.auth));
            if (reply != NULL) {
                freeReplyObject(reply);
                return REDIS_OK;
            }            
            cliPrintContextError(er);
            return REDIS_ERR;
        }
        int cliSelect(error::ErrorInfo& er) {
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
        int cliConnect(int force, error::ErrorInfo& er)
        {
            if (context == NULL || force) {
                if (context != NULL)
                    redisFree(context);

                if (config.hostsocket == NULL) {
                    context = redisConnect(config.hostip,config.hostport);
                } else {
                    context = redisConnectUnix(config.hostsocket);
                }

                if (context->err) {
                    char buff[512] = {0};
                    if (config.hostsocket == NULL)
                        sprintf(buff,"Could not connect to Redis at %s:%d: %s\n",config.hostip,config.hostport,context->errstr);
                    else
                        sprintf(buff,"Could not connect to Redis at %s: %s\n",config.hostsocket,context->errstr);
                    er = error::ErrorInfo(buff, error::ErrorInfo::E_ERROR);
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

        void cliRefreshPrompt(void) {
            int len;

            if (config.hostsocket != NULL)
                len = snprintf(config.prompt,sizeof(config.prompt),"redis %s",
                               config.hostsocket);
            else
                len = snprintf(config.prompt,sizeof(config.prompt),
                               strchr(config.hostip,':') ? "[%s]:%d" : "%s:%d",
                               config.hostip, config.hostport);
            /* Add [dbnum] if needed */
            if (config.dbnum != 0)
                len += snprintf(config.prompt+len,sizeof(config.prompt)-len,"[%d]",
                    config.dbnum);
            snprintf(config.prompt+len,sizeof(config.prompt)-len,"> ");
        }
        void cliPrintContextError(error::ErrorInfo& er) {
            if (context == NULL) return;
            char buff[512] = {0};
            sprintf(buff,"Error: %s\n",context->errstr);
            er = error::ErrorInfo(buff, error::ErrorInfo::E_ERROR);
        }

        sds cliFormatReplyTTY(redisReply *r, char *prefix) {
            sds out = sdsempty();
            switch (r->type) {
            case REDIS_REPLY_ERROR:
                out = sdscatprintf(out,"(error) %s\n", r->str);
            break;
            case REDIS_REPLY_STATUS:
                out = sdscat(out,r->str);
                out = sdscat(out,"\n");
            break;
            case REDIS_REPLY_INTEGER:
                out = sdscatprintf(out,"(integer) %lld\n",r->integer);
            break;
            case REDIS_REPLY_STRING:
                /* If you are producing output for the standard output we want
                * a more interesting output with quoted characters and so forth */
                out = sdscatrepr(out,r->str,r->len);
                out = sdscat(out,"\n");
            break;
            case REDIS_REPLY_NIL:
                out = sdscat(out,"(nil)\n");
            break;
            case REDIS_REPLY_ARRAY:
                if (r->elements == 0) {
                    out = sdscat(out,"(empty list or set)\n");
                } else {
                    unsigned int i, idxlen = 0;
                    char _prefixlen[16];
                    char _prefixfmt[16];
                    sds _prefix;
                    sds tmp;

                    /* Calculate chars needed to represent the largest index */
                    i = r->elements;
                    do {
                        idxlen++;
                        i /= 10;
                    } while(i);

                    /* Prefix for nested multi bulks should grow with idxlen+2 spaces */
                    memset(_prefixlen,' ',idxlen+2);
                    _prefixlen[idxlen+2] = '\0';
                    _prefix = sdscat(sdsnew(prefix),_prefixlen);

                    /* Setup prefix format for every entry */
                    snprintf(_prefixfmt,sizeof(_prefixfmt),"%%s%%%dd) ",idxlen);

                    for (i = 0; i < r->elements; i++) {
                        /* Don't use the prefix for the first element, as the parent
                         * caller already prepended the index number. */
                        out = sdscatprintf(out,_prefixfmt,i == 0 ? "" : prefix,i+1);

                        /* Format the multi bulk entry */
                        tmp = cliFormatReplyTTY(r->element[i],_prefix);
                        out = sdscatlen(out,tmp,sdslen(tmp));
                        sdsfree(tmp);
                    }
                    sdsfree(_prefix);
                }
            break;
            default:
                sprintf(out ,"Unknown reply type: %d\n", r->type);
            }
            return out;
        }
        sds cliFormatReplyRaw(redisReply *r) {
            sds out = sdsempty(), tmp;
            size_t i;

            switch (r->type) {
            case REDIS_REPLY_NIL:
                /* Nothing... */
                break;
            case REDIS_REPLY_ERROR:
                out = sdscatlen(out,r->str,r->len);
                out = sdscatlen(out,"\n",1);
                break;
            case REDIS_REPLY_STATUS:
            case REDIS_REPLY_STRING:
                out = sdscatlen(out,r->str,r->len);
                break;
            case REDIS_REPLY_INTEGER:
                out = sdscatprintf(out,"%lld",r->integer);
                break;
            case REDIS_REPLY_ARRAY:
                for (i = 0; i < r->elements; i++) {
                    if (i > 0) out = sdscat(out,config.mb_delim);
                    tmp = cliFormatReplyRaw(r->element[i]);
                    out = sdscatlen(out,tmp,sdslen(tmp));
                    sdsfree(tmp);
                }
                break;
            default:
                sprintf(out ,"Unknown reply type: %d\n", r->type);
            }
            return out;
        }
        sds cliFormatReplyCSV(redisReply *r) {
            unsigned int i;

            sds out = sdsempty();
            switch (r->type) {
            case REDIS_REPLY_ERROR:
                out = sdscat(out,"ERROR,");
                out = sdscatrepr(out,r->str,strlen(r->str));
            break;
            case REDIS_REPLY_STATUS:
                out = sdscatrepr(out,r->str,r->len);
            break;
            case REDIS_REPLY_INTEGER:
                out = sdscatprintf(out,"%lld",r->integer);
            break;
            case REDIS_REPLY_STRING:
                out = sdscatrepr(out,r->str,r->len);
            break;
            case REDIS_REPLY_NIL:
                out = sdscat(out,"NIL\n");
            break;
            case REDIS_REPLY_ARRAY:
                for (i = 0; i < r->elements; i++) {
                    sds tmp = cliFormatReplyCSV(r->element[i]);
                    out = sdscatlen(out,tmp,sdslen(tmp));
                    if (i != r->elements-1) out = sdscat(out,",");
                    sdsfree(tmp);
                }
            break;
            default:
                sprintf(out ,"Unknown reply type: %d\n", r->type);
            }
            return out;
        }

        void cliOutputCommandHelp(std::string &out, struct commandHelp *help, int group) {
            char buff[1024] = {0};
            sprintf(buff,"\r\n  name: %s %s\r\n  summary: %s\r\n  since: %s\r\n", help->name, help->params, help->summary, help->since);
            out += buff;
            if (group) {
                char buff2[1024] = {0};
                sprintf(buff2,"  group: %s\r\n", commandGroups[help->group]);
                out +=buff2;
            }
        }
        void cliOutputGenericHelp(std::string &out) {
            sds version = cliVersion();
            char buff[512] = {0};
            sprintf(buff,
                PROJECT_NAME" based on redis-cli %s\r\n"
                "Type: \"help @<group>\" to get a list of commands in <group>\r\n"
                "      \"help <command>\" for help on <command>\r\n"
                "      \"help <tab>\" to get a list of possible help topics\r\n"
                "      \"quit\" to exit\r\n",
                version
            );
            out += buff;
            sdsfree(version);
        }
        void cliOutputHelp(std::string &out, int argc, char **argv) {
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
            out += "\r\n";
        }
        int cliReadReply(std::string &out, error::ErrorInfo& er, int output_raw_strings) {
            void *_reply;
            redisReply *reply;

            if (redisGetReply(context,&_reply) != REDIS_OK) {
                if (config.shutdown)
                    return REDIS_OK;
                if (config.interactive) {
                    /* Filter cases where we should reconnect */
                    if (context->err == REDIS_ERR_IO && errno == ECONNRESET)
                        return REDIS_ERR;
                    if (context->err == REDIS_ERR_EOF)
                        return REDIS_ERR;
                }
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
                sdsfree(config.hostip);
                config.hostip = sdsnew(p+1);
                config.hostport = atoi(s+1);
                if (config.interactive){
                    char redir[512] = {0};
                    sprintf(redir, "-> Redirected to slot [%d] located at %s:%d\n", slot, config.hostip, config.hostport);
                    out += redir;
                }
                config.cluster_reissue_command = 1;
                cliRefreshPrompt();
            }
            else{
                if (output_raw_strings) {
                    out += cliFormatReplyRaw(reply);
                } else {
                    if (config.output == OUTPUT_RAW) {
                        out += cliFormatReplyRaw(reply);
                        out += "\n";
                    } else if (config.output == OUTPUT_STANDARD) {
                        out += cliFormatReplyTTY(reply,"");
                    } else if (config.output == OUTPUT_CSV) {
                        out += cliFormatReplyCSV(reply);
                        out += "\n";
                    }
                }
            }

            freeReplyObject(reply);
            return REDIS_OK;
        }
        int cliSendCommand(std::string &out, error::ErrorInfo& er, int argc, char **argv, int repeat)
        {
            char *command = argv[0];
            size_t *argvlen;
            int output_raw;

            if (!strcasecmp(command,"help") || !strcasecmp(command,"?")) {
                cliOutputHelp(out, --argc, ++argv);
                return REDIS_OK;
            }

            if (context == NULL)
                return REDIS_ERR;

            output_raw = 0;
            if (!strcasecmp(command,"info") ||
                (argc == 2 && !strcasecmp(command,"cluster") &&
                              (!strcasecmp(argv[1],"nodes") ||
                               !strcasecmp(argv[1],"info"))) ||
                (argc == 2 && !strcasecmp(command,"client") &&
                               !strcasecmp(argv[1],"list")))

            {
                output_raw = 1;
            }

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
                    if (cliReadReply(out, er, output_raw) != REDIS_OK){
                        return REDIS_ERR;
                    }
                }

                if (config.pubsub_mode) {
                    if (config.output != OUTPUT_RAW)
                    while (1) {
                        if (cliReadReply(out, er, output_raw) != REDIS_OK)
                            return REDIS_ERR;
                    }
                }

                if (cliReadReply(out, er, output_raw) != REDIS_OK) {
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
        void repl(const char *line, std::string &out, error::ErrorInfo &er) {
            sds historyfile = NULL;
            int history = 0;
            int argc;
            sds *argv;

            config.interactive = 1;

            /* Only use history when stdin is a tty. */
            if (isatty(fileno(stdin))) {
                history = 1;

                if (getenv("HOME") != NULL) {
                    historyfile = sdscatprintf(sdsempty(),"%s/.rediscli_history",getenv("HOME"));
                    linenoiseHistoryLoad(historyfile);
                }
            }

            cliRefreshPrompt();

            if (line[0] != '\0') {
                argv = sdssplitargs(line,&argc);
                if (history) linenoiseHistoryAdd(line);
                if (historyfile) linenoiseHistorySave(historyfile);

                    if (argv == NULL) {
                        out += "Invalid argument(s)\n";
                        return ;
                    }
                    else if (argc > 0)
                    {
                        if (strcasecmp(argv[0],"quit") == 0 ||
                            strcasecmp(argv[0],"exit") == 0)
                        {
                            config.shutdown = 1;
                        } else if (argc == 3 && !strcasecmp(argv[0],"connect")) {
                            sdsfree(config.hostip);
                            config.hostip = sdsnew(argv[1]);
                            config.hostport = atoi(argv[2]);
                            cliRefreshPrompt();
                            cliConnect(1, er);
                        } else if (argc == 1 && !strcasecmp(argv[0],"clear")) {
                            //linenoiseClearScreen();
                            return;
                        } else {
                            long long start_time = mstime(), elapsed;
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
                            elapsed = mstime()-start_time;
                            if (elapsed >= 500) {
                                char time[128] = {0};
                                sprintf(time,"(%.2fs)\n",(double)elapsed/1000);
                                out += time;
                            }
                        }
                    }
                    /* Free the argument vector */
                    sdsfreesplitres(argv,argc);
                }
            }
    };

    RedisDriver::RedisDriver(const IConnectionSettingsBasePtr &settings)
        :base_class(settings), _impl(new pimpl)
    {
        allCommands();
    }

    QStringList RedisDriver::allCommands()
    {
        static const QStringList allCommands = getList();
        return allCommands;
    }

    bool RedisDriver::isConnected() const
    {
        return _impl->context;
    }

    void RedisDriver::customEvent(QEvent *event)
    {
        return base_class::customEvent(event);
    }

    void RedisDriver::initImpl()
    {

    }

    RedisDriver::~RedisDriver()
    {

    }

    void RedisDriver::connectImpl(EventsInfo::ConnectInfoResponce &res)
    {
        RedisConnectionSettings *set = dynamic_cast<RedisConnectionSettings*>(settings().get());
        if(set){
            RedisConnectionSettings::config conf = set->info();            
            _impl->connect(conf, res);
        }
    }

    void RedisDriver::executeImpl(EventsInfo::ExecuteInfoResponce &res)
    {
        _impl->query(res);
    }

    void RedisDriver::disconnectImpl(EventsInfo::DisConnectInfoResponce &res)
    {
        _impl->disconnect();
    }
}
