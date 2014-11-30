#include "core/redis/redis_driver.h"

#include <errno.h>
#include <fcntl.h>

extern "C" {
#include "third-party/redis/src/release.h"
#include "third-party/redis/src/version.h"
#include "third-party/redis/src/help.h"
#include "third-party/redis/deps/hiredis/hiredis.h"
#include "third-party/redis/src/anet.h"
}

#include "common/time.h"
#include "common/utils.h"

#include "core/logger.h"
#include "core/command_logger.h"
#include "core/redis/redis_infos.h"

#define REDIS_CLI_KEEPALIVE_INTERVAL 15 /* seconds */
#define CLI_HELP_COMMAND 1
#define CLI_HELP_GROUP 2

#define INFO_REQUEST "INFO"
#define SYNC_REQUEST "SYNC"
#define FIND_BIG_KEYS_REQUEST "FIND_BIG_KEYS"
#define LATENCY_REQUEST "LATENCY"
#define GET_DATABASES "CONFIG GET databases"
#define GET_PROPERTY_SERVER "CONFIG GET *"
#define STAT_MODE_REQUEST "STAT"
#define SCAN_MODE_REQUEST "SCAN"
#define RDM_REQUEST "RDM"

#define LATENCY_SAMPLE_RATE 10 /* milliseconds. */
#define LATENCY_HISTORY_DEFAULT_INTERVAL 15000 /* milliseconds. */

#define TYPE_STRING 0
#define TYPE_LIST   1
#define TYPE_SET    2
#define TYPE_HASH   3
#define TYPE_ZSET   4
#define TYPE_NONE   5

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

    const char *redisGitSHA1(void) {
        return REDIS_GIT_SHA1;
    }

    const char *redisGitDirty(void) {
        return REDIS_GIT_DIRTY;
    }

    sds cliVersion() {
        sds version = sdscatprintf(sdsempty(), "%s", REDIS_VERSION);

        /* Add git commit and working tree status when available */
        if (strtoll(redisGitSHA1(), NULL, 16)) {
            version = sdscatprintf(version, " (git:%s", redisGitSHA1());
            if (strtoll(redisGitDirty(), NULL, 10))
                version = sdscatprintf(version, "-dirty");
            version = sdscat(version, ")");
        }
        return version;
    }

    typedef struct{
        int type;
        int argc;
        sds *argv;
        sds full;

        /* Only used for help on commands */
        struct commandHelp *org;
    } helpEntry;

    const int helpEntriesLen = sizeof(commandHelp)/sizeof(struct commandHelp) + sizeof(commandGroups)/sizeof(char*);
    struct RedisInit
    {
        helpEntry helpEntries[helpEntriesLen];
        std::vector<QString> redisTypesKeywords;
        std::vector<QString> redisCommandsKeywords;

        static const RedisInit &instance()
        {
            static RedisInit r;
            return r;
        }

      private:
        RedisInit()
        {
            int pos = 0;

            for(int i = 0; i < sizeof(commandGroups)/sizeof(char*); ++i){
                helpEntry tmp;
                char* command = commandGroups[i];
                QString qcommand = common::convertFromString<QString>(std::string(command));
                redisTypesKeywords.push_back(qcommand);
                tmp.argc = 1;
                tmp.argv = (sds*)malloc(sizeof(sds));
                tmp.argv[0] = sdscatprintf(sdsempty(), "@%s", commandGroups[i]);
                tmp.full = tmp.argv[0];
                tmp.type = CLI_HELP_GROUP;
                tmp.org = NULL;
                helpEntries[pos++] = tmp;
            }

            for(int i = 0; i < sizeof(commandHelp)/sizeof(struct commandHelp); ++i){
                helpEntry tmp;
                struct commandHelp command = commandHelp[i];
                std::string commandN = command.name;
                QString qCommandN = common::convertFromString<QString>(commandN);
                redisCommandsKeywords.push_back(qCommandN);

                tmp.argv = sdssplitargs(commandHelp[i].name, &tmp.argc);
                tmp.full = sdsnew(commandHelp[i].name);
                tmp.type = CLI_HELP_COMMAND;
                tmp.org = &commandHelp[i];
                helpEntries[pos++] = tmp;
            }
        }
    };
}

namespace fastoredis
{
    const std::vector<QString> redisTypesKeywords = RedisInit::instance().redisTypesKeywords;
    const std::vector<QString> redisCommandsKeywords = RedisInit::instance().redisCommandsKeywords;

    struct RedisDriver::pimpl
    {
        pimpl()
            : context(NULL), parent_(NULL)
        {

        }

        ~pimpl()
        {
            if(context){
                redisFree(context);
                context = NULL;
            }
        }

        redisContext *context;
        redisConfig config;        
        SSHInfo sinfo_;
        RedisDriver* parent_;

        /*------------------------------------------------------------------------------
         * Latency and latency history modes
         *--------------------------------------------------------------------------- */

        int latencyMode(FastoObjectPtr out, common::ErrorValueSPtr er) WARN_UNUSED_RESULT
        {
            redisReply *reply;
            long long start, latency, min = 0, max = 0, tot = 0, count = 0;
            long long history_interval =
                    config.interval ? config.interval/1000 :
                                      LATENCY_HISTORY_DEFAULT_INTERVAL;
            double avg;
            long long history_start = common::time::current_mstime();

            if (!context){
                er.reset(new common::ErrorValue("Not connected", common::Value::E_ERROR));
                return REDIS_ERR;
            }

            FastoObject* child = NULL;

            while(!config.shutdown) {
                start = common::time::current_mstime();
                reply = (redisReply*)redisCommand(context, "PING");
                if (reply == NULL) {
                    er.reset(new common::ErrorValue("I/O error", common::Value::E_ERROR));
                    return REDIS_ERR;
                }

                long long curTime = common::time::current_mstime();

                latency = curTime - start;
                freeReplyObject(reply);
                count++;
                if (count == 1) {
                    min = max = tot = latency;
                    avg = (double) latency;
                } else {
                    if (latency < min) min = latency;
                    if (latency > max) max = latency;
                    tot += latency;
                    avg = (double) tot/count;
                }

                char buff[1024];
                sprintf(buff, "min: %lld, max: %lld, avg: %.2f (%lld samples)",
                                    min, max, avg, count);
                common::Value *val = common::Value::createStringValue(buff);

                if(!child){
                    child = new FastoObject(out.get(), val, config.mb_delim);
                    out->addChildren(child);
                    continue;
                }

                if(config.latency_history && curTime - history_start > history_interval){
                    child = new FastoObject(out.get(), val, config.mb_delim);
                    out->addChildren(child);
                    history_start = curTime;
                    min = max = tot = count = 0;
                }
                else{
                    child->changeValue(val);
                }

                common::utils::msleep(LATENCY_SAMPLE_RATE);
            }

            return REDIS_OK;
        }

        /*------------------------------------------------------------------------------
         * Slave mode
         *--------------------------------------------------------------------------- */

        /* Sends SYNC and reads the number of bytes in the payload. Used both by
         * slaveMode() and getRDB(). */
        int sendSync(common::ErrorValueSPtr er, unsigned long long& payload) WARN_UNUSED_RESULT
        {
            /* To start we need to send the SYNC command and return the payload.
             * The hiredis client lib does not understand this part of the protocol
             * and we don't want to mess with its buffers, so everything is performed
             * using direct low-level I/O. */
            char buf[4096], *p;

            /* Send the SYNC command. */
            ssize_t nwrite = 0;
            if (redisWriteFromBuffer(context, "SYNC\r\n", &nwrite) == REDIS_ERR) {
                er.reset(new common::ErrorValue("Error writing to master", common::ErrorValue::E_ERROR));
                return REDIS_ERR;
            }

            /* Read $<payload>\r\n, making sure to read just up to "\n" */
            p = buf;
            while(1) {
                ssize_t nread = 0;
                int res = redisReadToBuffer(context, p, 1, &nread);
                if (res == REDIS_ERR) {
                    er.reset(new common::ErrorValue("Error reading bulk length while SYNCing", common::ErrorValue::E_ERROR));
                    return REDIS_ERR;
                }

                if(!nread){
                    continue;
                }

                if (*p == '\n' && p != buf) break;
                if (*p != '\n') p++;
            }
            *p = '\0';
            if (buf[0] == '-') {
                char buf2[4096];
                sprintf(buf2, "SYNC with master failed: %s", buf);
                er.reset(new common::ErrorValue(buf2, common::ErrorValue::E_ERROR));
                return REDIS_ERR;
            }

            payload = strtoull(buf+1, NULL, 10);
            return REDIS_OK;
        }

        int slaveMode(FastoObjectPtr out, common::ErrorValueSPtr er) WARN_UNUSED_RESULT
        {
            unsigned long long payload = 0;
            int res = sendSync(er, payload);
            if(res == REDIS_ERR){
                return REDIS_ERR;
            }

            char buf[1024];
            /* Discard the payload. */
            while(payload) {
                ssize_t nread = 0;
                int res = redisReadToBuffer(context, buf,(payload > sizeof(buf)) ? sizeof(buf) : payload, &nread);
                if (res == REDIS_ERR) {
                    er.reset(new common::ErrorValue("Error reading RDB payload while SYNCing", common::ErrorValue::E_ERROR));
                    return REDIS_ERR;
                }
                payload -= nread;
            }

            /* Now we can use hiredis to read the incoming protocol. */
            while (cliReadReply(out, er) == REDIS_OK){
                if (config.shutdown){
                    er.reset(new common::ErrorValue("Interrupted connect.", common::ErrorValue::E_INTERRUPTED));
                    return REDIS_OK;
                }
            }

            return REDIS_ERR;
        }

        /*------------------------------------------------------------------------------
         * RDB transfer mode
         *--------------------------------------------------------------------------- */

        /* This function implements --rdb, so it uses the replication protocol in order
         * to fetch the RDB file from a remote server. */
        int getRDB(FastoObjectPtr out, common::ErrorValueSPtr er) WARN_UNUSED_RESULT
        {
            unsigned long long payload = 0;
            int res = sendSync(er, payload);
            if(res == REDIS_ERR){
                return REDIS_ERR;
            }

            FastoObject* child = NULL;
            common::ArrayValue* val = NULL;

            int fd = INVALID_DESCRIPTOR;
            /* Write to file. */
            if (!strcmp(config.rdb_filename,"-")) {
                val = new common::ArrayValue;
                child = new FastoObject(out.get(), val, config.mb_delim);
                out->addChildren(child);
            }
            else{
                fd = open(config.rdb_filename, O_CREAT|O_WRONLY, 0644);
                if (fd == INVALID_DESCRIPTOR) {
                    char bufeEr[2048];
                    sprintf(bufeEr, "Error opening '%s': %s", config.rdb_filename,
                        strerror(errno));

                    er.reset(new common::ErrorValue(bufeEr, common::ErrorValue::E_ERROR));
                    return REDIS_ERR;
                }
            }

            char buf[4096];

            while(payload) {
                ssize_t nread, nwritten;

                int res = redisReadToBuffer(context, buf,(payload > sizeof(buf)) ? sizeof(buf) : payload, &nread);
                if (res == REDIS_ERR) {
                    er.reset(new common::ErrorValue("Error reading RDB payload while SYNCing", common::ErrorValue::E_ERROR));
                    return REDIS_ERR;
                }

                if(!nread){
                    continue;
                }

                if(fd != INVALID_DESCRIPTOR){
                    nwritten = write(fd, buf, nread);
                }
                else{
                    val->appendString(buf);
                }

                if (nwritten != nread) {
                    char bufeEr[2048];
                    sprintf(bufeEr, "Error writing data to file: %s",
                            strerror(errno));

                    er.reset(new common::ErrorValue(bufeEr, common::ErrorValue::E_ERROR));
                    break;
                }

                payload -= nread;
            }

            if(fd != INVALID_DESCRIPTOR){
                close(fd);
            }

            LOG_MSG("Transfer finished with success.", common::logging::L_INFO);
            return REDIS_OK;
        }

        /*------------------------------------------------------------------------------
         * Find big keys
         *--------------------------------------------------------------------------- */

        redisReply *sendScan(common::ErrorValueSPtr er, unsigned long long *it)
        {
            redisReply *reply = (redisReply *)redisCommand(context, "SCAN %llu", *it);

            /* Handle any error conditions */
            if(reply == NULL) {
                er.reset(new common::ErrorValue("I/O error", common::Value::E_ERROR));
                return NULL;
            } else if(reply->type == REDIS_REPLY_ERROR) {
                char buff[512];
                sprintf(buff, "SCAN error: %s", reply->str);
                er.reset(new common::ErrorValue(buff, common::Value::E_ERROR));
                return NULL;
            } else if(reply->type != REDIS_REPLY_ARRAY) {
                er.reset(new common::ErrorValue("Non ARRAY response from SCAN!", common::Value::E_ERROR));
                return NULL;
            } else if(reply->elements != 2) {
                er.reset(new common::ErrorValue("Invalid element count from SCAN!", common::Value::E_ERROR));
                return NULL;
            }

            /* Validate our types are correct */
            assert(reply->element[0]->type == REDIS_REPLY_STRING);
            assert(reply->element[1]->type == REDIS_REPLY_ARRAY);

            /* Update iterator */
            *it = atoi(reply->element[0]->str);

            return reply;
        }

        int getDbSize(common::ErrorValueSPtr er, long long& size) WARN_UNUSED_RESULT
        {
            redisReply *reply;

            reply = (redisReply *)redisCommand(context, "DBSIZE");

            if(reply == NULL || reply->type != REDIS_REPLY_INTEGER) {
                er.reset(new common::ErrorValue("Couldn't determine DBSIZE!", common::Value::E_ERROR));
                return REDIS_ERR;
            }

            /* Grab the number of keys and free our reply */
            size = reply->integer;
            freeReplyObject(reply);

            return REDIS_OK;
        }

        int toIntType(common::ErrorValueSPtr er, char *key, char *type) {
            if(!strcmp(type, "string")) {
                return TYPE_STRING;
            } else if(!strcmp(type, "list")) {
                return TYPE_LIST;
            } else if(!strcmp(type, "set")) {
                return TYPE_SET;
            } else if(!strcmp(type, "hash")) {
                return TYPE_HASH;
            } else if(!strcmp(type, "zset")) {
                return TYPE_ZSET;
            } else if(!strcmp(type, "none")) {
                return TYPE_NONE;
            } else {
                char buff[4096];
                sprintf(buff, "Unknown type '%s' for key '%s'", type, key);
                er.reset(new common::ErrorValue(buff, common::Value::E_ERROR));
                return -1;
            }
        }

        int getKeyTypes(common::ErrorValueSPtr er, redisReply *keys, int *types) WARN_UNUSED_RESULT
        {
            redisReply *reply;
            unsigned int i;

            /* Pipeline TYPE commands */
            for(i=0;i<keys->elements;i++) {
                redisAppendCommand(context, "TYPE %s", keys->element[i]->str);
            }

            /* Retrieve types */
            for(i=0;i<keys->elements;i++) {
                if(redisGetReply(context, (void**)&reply)!=REDIS_OK) {
                    char buff[4096];
                    sprintf(buff, "Error getting type for key '%s' (%d: %s)",
                        keys->element[i]->str, context->err, context->errstr);
                    er.reset(new common::ErrorValue(buff, common::Value::E_ERROR));
                    return REDIS_ERR;
                } else if(reply->type != REDIS_REPLY_STATUS) {
                    char buff[4096];
                    sprintf(buff, "Invalid reply type (%d) for TYPE on key '%s'!",
                        reply->type, keys->element[i]->str);
                    er.reset(new common::ErrorValue(buff, common::Value::E_ERROR));
                    return REDIS_ERR;
                }

                int res = toIntType(er, keys->element[i]->str, reply->str);
                freeReplyObject(reply);
                if(res != -1){
                    types[i] = res;
                }
                else{
                    return REDIS_ERR;
                }
            }

            return REDIS_OK;
        }

        int getKeySizes(common::ErrorValueSPtr er, redisReply *keys, int *types,
                                unsigned long long *sizes) WARN_UNUSED_RESULT
        {
            redisReply *reply;
            const char *sizecmds[] = {"STRLEN","LLEN","SCARD","HLEN","ZCARD"};
            unsigned int i;

            /* Pipeline size commands */
            for(i=0;i<keys->elements;i++) {
                /* Skip keys that were deleted */
                if(types[i]==TYPE_NONE)
                    continue;

                redisAppendCommand(context, "%s %s", sizecmds[types[i]],
                    keys->element[i]->str);
            }

            /* Retreive sizes */
            for(i=0;i<keys->elements;i++) {
                /* Skip keys that dissapeared between SCAN and TYPE */
                if(types[i] == TYPE_NONE) {
                    sizes[i] = 0;
                    continue;
                }

                /* Retreive size */
                if(redisGetReply(context, (void**)&reply)!=REDIS_OK) {
                    char buff[4096];
                    sprintf(buff, "Error getting size for key '%s' (%d: %s)",
                        keys->element[i]->str, context->err, context->errstr);
                    er.reset(new common::ErrorValue(buff, common::Value::E_ERROR));
                    return REDIS_ERR;
                } else if(reply->type != REDIS_REPLY_INTEGER) {
                    /* Theoretically the key could have been removed and
                     * added as a different type between TYPE and SIZE */
                    char buff[4096];
                    sprintf(buff,
                        "Warning:  %s on '%s' failed (may have changed type)",
                         sizecmds[types[i]], keys->element[i]->str);
                    LOG_MSG(buff, common::logging::L_WARNING);
                    sizes[i] = 0;
                } else {
                    sizes[i] = reply->integer;
                }

                freeReplyObject(reply);
            }

            return REDIS_OK;
        }

        int findBigKeys(FastoObjectPtr out, common::ErrorValueSPtr er) WARN_UNUSED_RESULT
        {
            unsigned long long biggest[5] = {0}, counts[5] = {0}, totalsize[5] = {0};
            unsigned long long sampled = 0, totlen=0, *sizes=NULL, it=0;
            long long total_keys;
            sds maxkeys[5] = {0};
            const char *typeName[] = {"string","list","set","hash","zset"};
            const char *typeunit[] = {"bytes","items","members","fields","members"};
            redisReply *reply, *keys;
            unsigned int arrsize=0, i;
            int type, *types=NULL;
            double pct;

            /* Total keys pre scanning */
            if(getDbSize(er, total_keys) == REDIS_ERR){
                return REDIS_ERR;
            }

            /* Status message */
            LOG_MSG("# Scanning the entire keyspace to find biggest keys as well as", common::logging::L_INFO);
            LOG_MSG("# average sizes per key type.  You can use -i 0.1 to sleep 0.1 sec", common::logging::L_INFO);
            LOG_MSG("# per 100 SCAN commands (not usually needed).", common::logging::L_INFO);

            /* New up sds strings to keep track of overall biggest per type */
            for(i=0;i<TYPE_NONE; i++) {
                maxkeys[i] = sdsempty();
                if(!maxkeys[i]) {
                    er.reset(new common::ErrorValue("Failed to allocate memory for largest key names!", common::Value::E_ERROR));
                    return REDIS_ERR;
                }
            }

            /* SCAN loop */
            do {
                /* Calculate approximate percentage completion */
                pct = 100 * (double)sampled/total_keys;

                /* Grab some keys and point to the keys array */
                reply = sendScan(er, &it);
                if(er){
                    return REDIS_ERR;
                }

                keys  = reply->element[1];

                /* Reallocate our type and size array if we need to */
                if(keys->elements > arrsize) {
                    types = (int*)realloc(types, sizeof(int)*keys->elements);
                    sizes = (unsigned long long*)realloc(sizes, sizeof(unsigned long long)*keys->elements);

                    if(!types || !sizes) {
                        er.reset(new common::ErrorValue("Failed to allocate storage for keys!", common::Value::E_ERROR));
                        return REDIS_ERR;
                    }

                    arrsize = keys->elements;
                }

                /* Retreive types and then sizes */
                if(getKeyTypes(er, keys, types) == REDIS_ERR){
                    return REDIS_ERR;
                }

                if(getKeySizes(er, keys, types, sizes) == REDIS_ERR){
                    return REDIS_ERR;
                }

                /* Now update our stats */
                for(i=0;i<keys->elements;i++) {
                    if((type = types[i]) == TYPE_NONE)
                        continue;

                    totalsize[type] += sizes[i];
                    counts[type]++;
                    totlen += keys->element[i]->len;
                    sampled++;

                    if(biggest[type]<sizes[i]) {
                        char buff[4096];
                        sprintf(buff,
                           "[%05.2f%%] Biggest %-6s found so far '%s' with %llu %s",
                           pct, typeName[type], keys->element[i]->str, sizes[i],
                           typeunit[type]);

                        LOG_MSG(buff, common::logging::L_INFO);

                        /* Keep track of biggest key name for this type */
                        maxkeys[type] = sdscpy(maxkeys[type], keys->element[i]->str);
                        if(!maxkeys[type]) {
                            er.reset(new common::ErrorValue("Failed to allocate memory for key!", common::Value::E_ERROR));
                            return REDIS_ERR;
                        }

                        /* Keep track of the biggest size for this type */
                        biggest[type] = sizes[i];
                    }
                }

                /* Sleep if we've been directed to do so */
                if(sampled && (sampled %100) == 0 && config.interval) {
                    common::utils::usleep(config.interval);
                }

                freeReplyObject(reply);
            } while(it != 0);

            if(types) free(types);
            if(sizes) free(sizes);

            /* We're done */
            char buff[4096];
            sprintf(buff, "Sampled %llu keys in the keyspace!", sampled);
            LOG_MSG(buff, common::logging::L_INFO);

            memset(&buff, 0, sizeof(buff));
            sprintf(buff, "Total key length in bytes is %llu (avg len %.2f)",
               totlen, totlen ? (double)totlen/sampled : 0);
            LOG_MSG(buff, common::logging::L_INFO);

            /* Output the biggest keys we found, for types we did find */
            for(i=0;i<TYPE_NONE;i++) {
                if(sdslen(maxkeys[i])>0) {
                    memset(&buff, 0, sizeof(buff));
                    sprintf(buff, "Biggest %6s found '%s' has %llu %s", typeName[i], maxkeys[i],
                       biggest[i], typeunit[i]);
                    common::StringValue *val = common::Value::createStringValue(buff);
                    FastoObject* obj = new FastoObject(out.get(), val, config.mb_delim);
                    out->addChildren(obj);
                }
            }

            for(i=0;i<TYPE_NONE;i++) {
                memset(&buff, 0, sizeof(buff));
                sprintf(buff, "%llu %ss with %llu %s (%05.2f%% of keys, avg size %.2f)",
                   counts[i], typeName[i], totalsize[i], typeunit[i],
                   sampled ? 100 * (double)counts[i]/sampled : 0,
                   counts[i] ? (double)totalsize[i]/counts[i] : 0);
                common::StringValue *val = common::Value::createStringValue(buff);
                FastoObject* obj = new FastoObject(out.get(), val, config.mb_delim);
                out->addChildren(obj);
            }

            /* Free sds strings containing max keys */
            for(i=0;i<TYPE_NONE;i++) {
                sdsfree(maxkeys[i]);
            }

            /* Success! */
            return REDIS_OK;
        }

        /*------------------------------------------------------------------------------
         * Stats mode
         *--------------------------------------------------------------------------- */

        /* Return the specified INFO field from the INFO command output "info".
         * A new buffer is allocated for the result, that needs to be free'd.
         * If the field is not found NULL is returned. */
        static char *getInfoField(char *info, char *field) {
            char *p = strstr(info,field);
            char *n1, *n2;
            char *result;

            if (!p) return NULL;
            p += strlen(field)+1;
            n1 = strchr(p,'\r');
            n2 = strchr(p,',');
            if (n2 && n2 < n1) n1 = n2;
            result = (char*)malloc(sizeof(char)*(n1-p)+1);
            memcpy(result,p,(n1-p));
            result[n1-p] = '\0';
            return result;
        }

        /* Like the above function but automatically convert the result into
         * a long. On error (missing field) LONG_MIN is returned. */
        static long getLongInfoField(char *info, char *field) {
            char *value = getInfoField(info,field);
            long l;

            if (!value) return LONG_MIN;
            l = strtol(value,NULL,10);
            free(value);
            return l;
        }

        /* Convert number of bytes into a human readable string of the form:
         * 100B, 2G, 100M, 4K, and so forth. */
        static void bytesToHuman(char *s, long long n) {
            double d;

            if (n < 0) {
                *s = '-';
                s++;
                n = -n;
            }
            if (n < 1024) {
                /* Bytes */
                sprintf(s,"%lluB",n);
                return;
            } else if (n < (1024*1024)) {
                d = (double)n/(1024);
                sprintf(s,"%.2fK",d);
            } else if (n < (1024LL*1024*1024)) {
                d = (double)n/(1024*1024);
                sprintf(s,"%.2fM",d);
            } else if (n < (1024LL*1024*1024*1024)) {
                d = (double)n/(1024LL*1024*1024);
                sprintf(s,"%.2fG",d);
            }
        }

        int statMode(FastoObjectPtr out, common::ErrorValueSPtr er) WARN_UNUSED_RESULT
        {
            long aux, requests = 0;

            while(!config.shutdown) {
                char buf[64];
                int j;

                redisReply *reply = NULL;
                while(reply == NULL) {
                    reply = (redisReply*)redisCommand(context,"INFO");
                    if (context->err && !(context->err & (REDIS_ERR_IO | REDIS_ERR_EOF))) {
                        char buff[2048];
                        sprintf(buff, "ERROR: %s", context->errstr);
                        er.reset(new common::ErrorValue(buff, common::ErrorValue::E_ERROR));
                        return REDIS_ERR;
                    }
                }

                if (reply->type == REDIS_REPLY_ERROR) {
                    char buff[2048];
                    sprintf(buff, "ERROR: %s", reply->str);
                    er.reset(new common::ErrorValue(buff, common::ErrorValue::E_ERROR));
                    return REDIS_ERR;
                }

                /* Keys */
                aux = 0;
                for (j = 0; j < 20; j++) {
                    long k;

                    sprintf(buf,"db%d:keys",j);
                    k = getLongInfoField(reply->str,buf);
                    if (k == LONG_MIN) continue;
                    aux += k;
                }

                std::string result;

                sprintf(buf,"keys %ld",aux);
                result += buf;

                /* Used memory */
                aux = getLongInfoField(reply->str,"used_memory");
                bytesToHuman(buf,aux);
                result += " used_memory: ";
                result += buf;

                /* Clients */
                aux = getLongInfoField(reply->str,"connected_clients");
                sprintf(buf," connected_clients: %ld",aux);
                result += buf;

                /* Blocked (BLPOPPING) Clients */
                aux = getLongInfoField(reply->str,"blocked_clients");
                sprintf(buf," blocked_clients: %ld",aux);
                result += buf;

                /* Requets */
                aux = getLongInfoField(reply->str,"total_commands_processed");
                sprintf(buf," total_commands_processed: %ld (+%ld)",aux,requests == 0 ? 0 : aux-requests);
                result += buf;
                requests = aux;

                /* Connections */
                aux = getLongInfoField(reply->str,"total_connections_received");
                sprintf(buf," total_connections_received: %ld",aux);
                result += buf;

                /* Children */
                aux = getLongInfoField(reply->str,"bgsave_in_progress");
                aux |= getLongInfoField(reply->str,"aof_rewrite_in_progress") << 1;
                switch(aux) {
                case 0: break;
                case 1:
                    result += " SAVE";
                    break;
                case 2:
                    result += " AOF";
                    break;
                case 3:
                    result += " SAVE+AOF";
                    break;
                }

                common::StringValue *val = common::Value::createStringValue(result);
                FastoObject* obj = new FastoObject(out.get(), val, config.mb_delim);
                out->addChildren(obj);

                freeReplyObject(reply);

                common::utils::usleep(config.interval);
            }

            return REDIS_OK;
        }

        /*------------------------------------------------------------------------------
         * Scan mode
         *--------------------------------------------------------------------------- */

        int scanMode(FastoObjectPtr out, common::ErrorValueSPtr er) WARN_UNUSED_RESULT
        {
            redisReply *reply;
            unsigned long long cur = 0;

            do {
                if (config.pattern)
                    reply = (redisReply*)redisCommand(context,"SCAN %llu MATCH %s",
                        cur,config.pattern);
                else
                    reply = (redisReply*)redisCommand(context,"SCAN %llu",cur);
                if (reply == NULL) {
                    er.reset(new common::ErrorValue("I/O error", common::ErrorValue::E_ERROR));
                    return REDIS_ERR;
                } else if (reply->type == REDIS_REPLY_ERROR) {
                    char buff[2048];
                    sprintf(buff, "ERROR: %s", reply->str);
                    er.reset(new common::ErrorValue(buff, common::ErrorValue::E_ERROR));
                    return REDIS_ERR;
                } else {
                    unsigned int j;

                    cur = strtoull(reply->element[0]->str,NULL,10);
                    for (j = 0; j < reply->element[1]->elements; j++){
                        common::StringValue *val = common::Value::createStringValue(reply->element[1]->element[j]->str);
                        FastoObject* obj = new FastoObject(out.get(), val, config.mb_delim);
                        out->addChildren(obj);
                    }
                }
                freeReplyObject(reply);
            } while(cur != 0);

            return REDIS_OK;
        }

        int cliAuth(common::ErrorValueSPtr er)
        {
            if (config.auth == NULL)
                return REDIS_OK;

            redisReply *reply = static_cast<redisReply*>(redisCommand(context, "AUTH %s", config.auth));
            if (reply != NULL) {
                freeReplyObject(reply);
                return REDIS_OK;
            }

            cliPrintContextError(er);
            return REDIS_ERR;
        }

        int cliSelect(common::ErrorValueSPtr er)
        {
            if (config.dbnum == 0)
                return REDIS_OK;

            redisReply *reply = static_cast<redisReply*>(redisCommand(context, "SELECT %d", config.dbnum));
            if (reply != NULL) {
                freeReplyObject(reply);
                return REDIS_OK;
            }
            cliPrintContextError(er);
            return REDIS_ERR;
        }

        int cliConnect(int force, common::ErrorValueSPtr er)
        {
            if (context == NULL || force) {
                if (context != NULL)
                    redisFree(context);

                if (config.hostsocket == NULL) {
                    const char *username = toCString(sinfo_.userName_);
                    const char *password = toCString(sinfo_.password_);
                    const char *ssh_address = toCString(sinfo_.hostName_);
                    int ssh_port = sinfo_.port_;
                    int curM = sinfo_.currentMethod_;
                    const char *publicKey = toCString(sinfo_.publicKey_);
                    const char *privateKey = toCString(sinfo_.privateKey_);
                    const char *passphrase = toCString(sinfo_.passphrase_);

                    context = redisConnect(config.hostip, config.hostport, ssh_address, ssh_port, username, password,
                                           publicKey, privateKey, passphrase, curM);
                }
                else {
                    context = redisConnectUnix(config.hostsocket);
                }

                if (context->err) {
                    char buff[512] = {0};
                    if (config.hostsocket)
                        sprintf(buff, "Could not connect to Redis at %s:%d: %s", config.hostip, config.hostport, context->errstr);
                    else
                        sprintf(buff, "Could not connect to Redis at %s: %s", config.hostsocket, context->errstr);

                    er.reset(new common::ErrorValue(buff, common::Value::E_ERROR));
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

        void cliPrintContextError(common::ErrorValueSPtr er)
        {
            if (context == NULL)
                return;

            char buff[512] = {0};
            sprintf(buff,"Error: %s",context->errstr);
            er.reset(new common::ErrorValue(buff, common::ErrorValue::E_ERROR));
        }

        void cliFormatReplyRaw(FastoObject* out, redisReply *r)
        {
            DCHECK(out);
            if(!out){
                return;
            }

            FastoObject* obj = NULL;
            switch (r->type) {
                case REDIS_REPLY_NIL:
                {
                    common::Value *val = common::Value::createNullValue();
                    obj = new FastoObject(out, val, config.mb_delim);
                    out->addChildren(obj);
                    break;
                }
                case REDIS_REPLY_ERROR:
                {
                    common::ErrorValue *val = common::Value::createErrorValue(r->str, common::ErrorValue::E_NONE, common::logging::L_WARNING);
                    obj = new FastoObject(out, val, config.mb_delim);
                    out->addChildren(obj);
                    break;
                }
                case REDIS_REPLY_STATUS:
                case REDIS_REPLY_STRING:
                {
                    common::StringValue *val = common::Value::createStringValue(r->str);
                    obj = new FastoObject(out, val, config.mb_delim);
                    out->addChildren(obj);
                    break;
                }
                case REDIS_REPLY_INTEGER:
                {
                    common::FundamentalValue *val =common::Value::createIntegerValue(r->integer);
                    obj = new FastoObject(out, val, config.mb_delim);
                    out->addChildren(obj);
                    break;
                }
                case REDIS_REPLY_ARRAY:
                {
                    for (size_t i = 0; i < r->elements; i++) {
                        cliFormatReplyRaw(out, r->element[i]);
                    }
                    break;
                }
            default:
                {
                    char tmp2[128] = {0};
                    sprintf(tmp2, "Unknown reply type: %d", r->type);
                    common::ErrorValue *val = common::Value::createErrorValue(tmp2, common::ErrorValue::E_NONE, common::logging::L_WARNING);
                    obj = new FastoObject(out, val, config.mb_delim);
                    out->addChildren(obj);
                }
            }

        }

        void cliOutputCommandHelp(FastoObjectPtr out, struct commandHelp *help, int group)
        {
            char buff[1024] = {0};
            sprintf(buff,"name: %s %s\r\n  summary: %s\r\n  since: %s", help->name, help->params, help->summary, help->since);
            common::StringValue *val =common::Value::createStringValue(buff);
            FastoObject* child = new FastoObject(out.get(), val, config.mb_delim);
            out->addChildren(child);
            if (group) {
                char buff2[1024] = {0};
                sprintf(buff2,"  group: %s", commandGroups[help->group]);
                val = common::Value::createStringValue(buff2);
                FastoObject* gchild = new FastoObject(out.get(), val, config.mb_delim);
                out->addChildren(gchild);
            }
        }

        void cliOutputGenericHelp(FastoObjectPtr out)
        {
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
            FastoObject* child = new FastoObject(out.get(), val, config.mb_delim);
            out->addChildren(child);
            sdsfree(version);
        }

        void cliOutputHelp(FastoObjectPtr out, int argc, char **argv)
        {
            int i, j, len;
            int group = -1;
            const helpEntry *entry;
            struct commandHelp *help;

            if (argc == 0) {
                cliOutputGenericHelp(out);
                return;
            }
            else if (argc > 0 && argv[0][0] == '@') {
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
                entry = &RedisInit::instance().helpEntries[i];
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
                }
                else {
                    if (group == help->group) {
                        cliOutputCommandHelp(out, help,0);
                    }
                }
            }
        }

        int cliReadReply(FastoObjectPtr out, common::ErrorValueSPtr er)
        {
            DCHECK(out);

            void *_reply;
            redisReply *reply;
            int output = 1;

            if (redisGetReply(context, &_reply) != REDIS_OK) {
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
            config.last_cmd_type = reply->type;

            if (config.cluster_mode && reply->type == REDIS_REPLY_ERROR &&
                (!strncmp(reply->str,"MOVED",5) || !strcmp(reply->str,"ASK")))
            {
                char *p = reply->str, *s;
                int slot;

                output = 0;

                s = strchr(p,' ');      /* MOVED[S]3999 127.0.0.1:6381 */
                p = strchr(s+1,' ');    /* MOVED[S]3999[P]127.0.0.1:6381 */
                *p = '\0';
                slot = atoi(s+1);
                s = strchr(p+1,':');    /* MOVED 3999[P]127.0.0.1[S]6381 */
                *s = '\0';                
                config.hostip = p+1;
                config.hostport = atoi(s+1);                
                char redir[512] = {0};
                sprintf(redir, "-> Redirected to slot [%d] located at %s:%d", slot, config.hostip, config.hostport);
                common::StringValue *val = common::Value::createStringValue(redir);
                FastoObject* child = new FastoObject(out.get(), val, config.mb_delim);
                out->addChildren(child);
                config.cluster_reissue_command = 1;
            }

            if (output) {
                cliFormatReplyRaw(out.get(), reply);
            }

            freeReplyObject(reply);
            return REDIS_OK;
        }

        int cliSendCommand(FastoObjectPtr out, common::ErrorValueSPtr er, int argc, char **argv, int repeat)
        {
            DCHECK(out);

            char *command = argv[0];
            size_t *argvlen;
            int j, output_raw;

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
                               !strcasecmp(argv[1],"list")) ||
                (argc == 3 && !strcasecmp(command,"latency") &&
                               !strcasecmp(argv[1],"graph")) ||
                (argc == 2 && !strcasecmp(command,"latency") &&
                               !strcasecmp(argv[1],"doctor")))
            {
                output_raw = 1;
            }

            if (!strcasecmp(command,"shutdown")) config.shutdown = 1;
            if (!strcasecmp(command,"monitor")) config.monitor_mode = 1;
            if (!strcasecmp(command,"subscribe") ||
                !strcasecmp(command,"psubscribe")) config.pubsub_mode = 1;
            if (!strcasecmp(command,"sync") ||
                !strcasecmp(command,"psync")) config.slave_mode = 1;

            /* Setup argument length */
            argvlen = (size_t*)malloc(argc*sizeof(size_t));
            for (j = 0; j < argc; j++)
                argvlen[j] = sdslen(argv[j]);

            while(repeat--) {
                redisAppendCommandArgv(context,argc,(const char**)argv,argvlen);
                while (config.monitor_mode) {
                    if (cliReadReply(out, er) != REDIS_OK){
                        return REDIS_ERR;
                    }
                }

                /*if (config.pubsub_mode) {
                    if (config.output != OUTPUT_RAW)
                    while (1) {
                        if (cliReadReply(output_raw, out, er) != REDIS_OK)
                            return REDIS_ERR;
                    }
                }*/

                if (config.slave_mode) {
                    slaveMode(out, er);
                    config.slave_mode = 0;
                    free(argvlen);
                    return REDIS_ERR;  /* Error = slaveMode lost connection to master */
                }

                if (cliReadReply(out, er) != REDIS_OK) {
                    free(argvlen);
                    return REDIS_ERR;
                } else {
                    /* Store database number when SELECT was successfully executed. */
                    if (!strcasecmp(command,"select") && argc == 2) {
                        config.dbnum = atoi(argv[1]);
                    } else if (!strcasecmp(command,"auth") && argc == 2) {
                        cliSelect(er);
                    }
                }
                if (config.interval){
                    common::utils::usleep(config.interval);
                }
            }

            free(argvlen);
            return REDIS_OK;
        }

        int execute(const char *command, Command::CommandType type, FastoObjectPtr out, common::ErrorValueSPtr er) WARN_UNUSED_RESULT
        {
            DCHECK(out);
            if(!out){
                er.reset(new common::ErrorValue("Invalid input argument", common::ErrorValue::E_ERROR));
                return REDIS_ERR;
            }

            if(!command){
                er.reset(new common::ErrorValue("Command empty", common::ErrorValue::E_ERROR));
                return REDIS_ERR;
            }

            LOG_COMMAND(Command(command, type));

            if (command[0] != '\0') {
                int argc;
                sds *argv = sdssplitargs(command,&argc);

                if (argv == NULL) {
                    common::StringValue *val = common::Value::createStringValue("Invalid argument(s)");
                    FastoObject* child = new FastoObject(out.get(), val, config.mb_delim);
                    out->addChildren(child);
                }
                else if (argc > 0)
                {
                    if (strcasecmp(argv[0], "quit") == 0 || strcasecmp(argv[0], "exit") == 0)
                    {
                        config.shutdown = 1;
                    }
                    else if (argc == 3 && !strcasecmp(argv[0], "connect")) {
                        config.hostip = argv[1];
                        config.hostport = atoi(argv[2]);
                        int res = cliConnect(1, er);
                        if(res == REDIS_ERR){
                            return REDIS_ERR;
                        }
                    }
                    else {
                        int repeat, skipargs = 0;

                        repeat = atoi(argv[0]);
                        if (argc > 1 && repeat) {
                            skipargs = 1;
                        } else {
                            repeat = 1;
                        }

                        while (1) {
                            config.cluster_reissue_command = 0;
                            if (cliSendCommand(out, er, argc-skipargs, argv+skipargs, repeat) != REDIS_OK)
                            {
                                cliConnect(1, er);

                                /* If we still cannot send the command print error.
                                * We'll try to reconnect the next time. */
                                if (cliSendCommand(out, er, argc-skipargs, argv+skipargs, repeat) != REDIS_OK){
                                    cliPrintContextError(er);
                                    sdsfreesplitres(argv,argc);
                                    return REDIS_ERR;
                                }
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

            return REDIS_OK;
        }
    };

    RedisDriver::RedisDriver(const IConnectionSettingsBasePtr &settings)
        : IDriver(settings), impl_(new pimpl)
    {
        impl_->parent_ = this;
    }

    RedisDriver::~RedisDriver()
    {
    }

    std::string RedisDriver::address() const
    {
        if(impl_->context){
            char address[512] = {0};
            sprintf(address, "%s:%d", impl_->config.hostip, impl_->config.hostport);
            return address;
        }
        else{
            return "not connected";
        }
    }

    std::string RedisDriver::version() const
    {
        return versionApi();
    }

    const char* RedisDriver::versionApi()
    {
        return REDIS_VERSION;
    }

    bool RedisDriver::isConnected() const
    {
        return impl_->context;
    }

    void RedisDriver::customEvent(QEvent *event)
    {
        IDriver::customEvent(event);
        impl_->config.shutdown = 0;
    }    

    void RedisDriver::initImpl()
    {

    }

    common::ErrorValueSPtr RedisDriver::currentLoggingInfo(FastoObjectPtr& outInfo)
    {
        RootLocker rl = make_locker(NULL, INFO_REQUEST);
        FastoObjectPtr outRoot = rl.root_;
        outInfo = outRoot;
        common::ErrorValueSPtr er;
        int res = impl_->execute(INFO_REQUEST, Command::InnerCommand, outRoot, er);
        return er;
    }

    void RedisDriver::connectEvent(Events::ConnectRequestEvent *ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            Events::ConnectResponceEvent::value_type res(ev->value());
            RedisConnectionSettings *set = dynamic_cast<RedisConnectionSettings*>(settings_.get());
            if(set){
                impl_->config = set->info();
                impl_->sinfo_ = set->sshInfo();
                common::ErrorValueSPtr er;
        notifyProgress(sender, 25);
                    if(impl_->config.shutdown){
                        common::ErrorValueSPtr er(new common::ErrorValue("Interrupted connect.", common::ErrorValue::E_INTERRUPTED));
                        res.setErrorInfo(er);
                    }
                    else{
                        if(impl_->cliConnect(0, er) == REDIS_ERR){
                            res.setErrorInfo(er);
                        }
                    }
        notifyProgress(sender, 75);
            }
            reply(sender, new Events::ConnectResponceEvent(this, res));
        notifyProgress(sender, 100);
    }

    void RedisDriver::processCommandLineArgs(Events::ProcessConfigArgsRequestEvent* ev)
    {
        /* Latency mode */
        if (impl_->config.latency_mode) {
            latencyMode(ev);
        }

        /* Slave mode */
        if (impl_->config.slave_mode) {
            slaveMode(ev);
        }

        /* Get RDB mode. */
        if (impl_->config.getrdb_mode) {
            getRDBMode(ev);
        }

        /* Pipe mode */
        /*if (impl_->config.pipe_mode) {
            pipeMode(ev);
        }*/

        /* Find big keys */
        if (impl_->config.bigkeys) {
            findBigKeysMode(ev);
        }

        /* Stat mode */
        if (impl_->config.stat_mode) {
            if (impl_->config.interval == 0){
                impl_->config.interval = 1000000;
            }
            statMode(ev);
        }

        /* Scan mode */
        if (impl_->config.scan_mode) {
            scanMode(ev);
        }

        interacteveMode(ev);

        QObject *sender = ev->sender();
        Events::ProcessConfigArgsResponceEvent::value_type res(ev->value());
        reply(sender, new Events::ProcessConfigArgsResponceEvent(this, res));
    }

    void RedisDriver::interacteveMode(Events::ProcessConfigArgsRequestEvent *ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
        Events::EnterModeEvent::value_type res(IntaractiveMode);
        reply(sender, new Events::EnterModeEvent(this, res));

        Events::LeaveModeEvent::value_type res2(IntaractiveMode);
        reply(sender, new Events::LeaveModeEvent(this, res2));
        notifyProgress(sender, 100);
    }

    int RedisDriver::latencyMode(Events::ProcessConfigArgsRequestEvent* ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
        Events::EnterModeEvent::value_type resEv(LatencyMode);
        reply(sender, new Events::EnterModeEvent(this, resEv));

        common::ErrorValueSPtr er;
        RootLocker lock = make_locker(sender, LATENCY_REQUEST);

        FastoObjectPtr obj = lock.root_;
        int res = impl_->latencyMode(obj, er);
        if(res == REDIS_ERR){
            LOG_ERROR(er);
        }

        Events::LeaveModeEvent::value_type resEv2(LatencyMode);
        reply(sender, new Events::LeaveModeEvent(this, resEv2));
        notifyProgress(sender, 100);
        return res;
    }

    int RedisDriver::slaveMode(Events::ProcessConfigArgsRequestEvent* ev)
    {
        QObject* sender = ev->sender();
        notifyProgress(sender, 0);
        Events::EnterModeEvent::value_type resEv(SlaveMode);
        reply(sender, new Events::EnterModeEvent(this, resEv));

        common::ErrorValueSPtr er;
        RootLocker lock = make_locker(sender, SYNC_REQUEST);

        FastoObjectPtr obj = lock.root_;
        int res = impl_->slaveMode(obj, er);
        if(res == REDIS_ERR){
            LOG_ERROR(er);
        }

        Events::LeaveModeEvent::value_type resEv2(SlaveMode);
        reply(sender, new Events::LeaveModeEvent(this, resEv2));
        notifyProgress(sender, 100);
    }

    int RedisDriver::getRDBMode(Events::ProcessConfigArgsRequestEvent* ev)
    {
        QObject* sender = ev->sender();
        notifyProgress(sender, 0);
        Events::EnterModeEvent::value_type resEv(GetRDBMode);
        reply(sender, new Events::EnterModeEvent(this, resEv));

        common::ErrorValueSPtr er;
        RootLocker lock = make_locker(sender, RDM_REQUEST);

        FastoObjectPtr obj = lock.root_;
        int res = impl_->getRDB(obj, er);
        if(res == REDIS_ERR){
            LOG_ERROR(er);
        }

        Events::LeaveModeEvent::value_type resEv2(GetRDBMode);
        reply(sender, new Events::LeaveModeEvent(this, resEv2));
        notifyProgress(sender, 100);
        return res;
    }

    /*void RedisDriver::pipeMode(Events::ProcessConfigArgsRequestEvent* ev)
    {
        QObject* sender = ev->sender();
        notifyProgress(sender, 0);
        Events::EnterModeEvent::value_type res(PipeMode);
        reply(sender, new Events::EnterModeEvent(this, res));

        Events::LeaveModeEvent::value_type res2(PipeMode);
        reply(sender, new Events::LeaveModeEvent(this, res2));
        notifyProgress(sender, 100);
    }*/

    int RedisDriver::findBigKeysMode(Events::ProcessConfigArgsRequestEvent* ev)
    {
        QObject* sender = ev->sender();
        notifyProgress(sender, 0);
        Events::EnterModeEvent::value_type resEv(FindBigKeysMode);
        reply(sender, new Events::EnterModeEvent(this, resEv));

        common::ErrorValueSPtr er;
        RootLocker lock = make_locker(sender, FIND_BIG_KEYS_REQUEST);

        FastoObjectPtr obj = lock.root_;
        int res = impl_->findBigKeys(obj, er);
        if(res == REDIS_ERR){
            LOG_ERROR(er);
        }

        Events::LeaveModeEvent::value_type resEv2(FindBigKeysMode);
        reply(sender, new Events::LeaveModeEvent(this, resEv2));
        notifyProgress(sender, 100);
        return res;
    }

    int RedisDriver::statMode(Events::ProcessConfigArgsRequestEvent* ev)
    {
        QObject* sender = ev->sender();
        notifyProgress(sender, 0);
        Events::EnterModeEvent::value_type resEv(StatMode);
        reply(sender, new Events::EnterModeEvent(this, resEv));

        common::ErrorValueSPtr er;
        RootLocker lock = make_locker(sender, STAT_MODE_REQUEST);

        FastoObjectPtr obj = lock.root_;
        int res = impl_->statMode(obj, er);
        if(res == REDIS_ERR){
            LOG_ERROR(er);
        }

        Events::LeaveModeEvent::value_type resEv2(StatMode);
        reply(sender, new Events::LeaveModeEvent(this, resEv2));
        notifyProgress(sender, 100);
        return res;
    }

    int RedisDriver::scanMode(Events::ProcessConfigArgsRequestEvent* ev)
    {
        QObject* sender = ev->sender();
        notifyProgress(sender, 0);
        Events::EnterModeEvent::value_type resEv(ScanMode);
        reply(sender, new Events::EnterModeEvent(this, resEv));

        common::ErrorValueSPtr er;
        RootLocker lock = make_locker(sender, SCAN_MODE_REQUEST);

        FastoObjectPtr obj = lock.root_;
        int res = impl_->scanMode(obj, er);
        if(res == REDIS_ERR){
            LOG_ERROR(er);
        }

        Events::LeaveModeEvent::value_type resEv2(ScanMode);
        reply(sender, new Events::LeaveModeEvent(this, resEv2));
        notifyProgress(sender, 100);
        return res;
    }

    void RedisDriver::executeEvent(Events::ExecuteRequestEvent *ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            Events::ExecuteRequestEvent::value_type res(ev->value());
            const char *inputLine = toCString(res._text);

            common::ErrorValueSPtr er;
            if(inputLine){
                size_t length = strlen(inputLine);
                int offset = 0;
                RootLocker lock = make_locker(sender, inputLine);
                FastoObjectPtr outRoot = lock.root_;
                double step = 100.0f/length;
                for(size_t n = 0; n < length; ++n){
                    if(impl_->config.shutdown){
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
                        int resEx = impl_->execute(command, Command::UserCommand, outRoot, er);
                        if(resEx == REDIS_ERR){
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

    void RedisDriver::disconnectEvent(Events::DisconnectRequestEvent *ev)
    {
        QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            Events::DisconnectResponceEvent::value_type res(ev->value());            
        notifyProgress(sender, 50);

            redisFree(impl_->context);
            impl_->context = NULL;

            reply(sender, new Events::DisconnectResponceEvent(this, res));
        notifyProgress(sender, 100);
    }

    void RedisDriver::loadDatabaseInfosEvent(Events::LoadDatabasesInfoRequestEvent *ev)
    {
            QObject *sender = ev->sender();
        notifyProgress(sender, 0);
            Events::LoadDatabasesInfoResponceEvent::value_type res(ev->value());
            RootLocker lock = make_locker(sender, GET_DATABASES);
            FastoObjectPtr root = lock.root_;
            common::ErrorValueSPtr er;
        notifyProgress(sender, 50);
            int resEx = impl_->execute(GET_DATABASES, Command::InnerCommand, root, er);
            if(resEx == REDIS_ERR){
                res.setErrorInfo(er);
            }
            else{
                FastoObject::child_container_type rchildrens = root->childrens();
                if(rchildrens.size()){
                    DCHECK(rchildrens.size() == 1);
                    FastoObject::child_container_type childrens = rchildrens[0]->childrens();
                    for(int i = 0; i < childrens.size(); ++i){
                        DataBaseInfo dbInf(childrens[i]->toString(), 0);
                        res.databases_.push_back(dbInf);
                    }
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
            RootLocker lock = make_locker(sender, INFO_REQUEST);
            FastoObjectPtr root = lock.root_;
            common::ErrorValueSPtr er;
        notifyProgress(sender, 50);
            int resEx = impl_->execute(INFO_REQUEST, Command::InnerCommand, root, er);
            if(resEx == REDIS_ERR){
                res.setErrorInfo(er);
            }
            else{
                FastoObject::child_container_type ch = root->childrens();
                if(ch.size()){
                    DCHECK(ch.size() == 1);
                    res.setInfo(makeRedisServerInfo(ch[0]));
                }
            }
        notifyProgress(sender, 75);
            reply(sender, new Events::ServerInfoResponceEvent(this, res));
        notifyProgress(sender, 100);
    }

    void RedisDriver::loadServerPropertyEvent(Events::ServerPropertyInfoRequestEvent *ev)
    {
        QObject* sender = ev->sender();
        notifyProgress(sender, 0);
        Events::ServerPropertyInfoResponceEvent::value_type res(ev->value());
            RootLocker lock = make_locker(sender, GET_PROPERTY_SERVER);
            FastoObjectPtr root = lock.root_;
            common::ErrorValueSPtr er;
        notifyProgress(sender, 50);
            int resEx = impl_->execute(GET_PROPERTY_SERVER, Command::InnerCommand, root, er);
            if(resEx == REDIS_ERR){
                res.setErrorInfo(er);
            }
            else{
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
        common::ErrorValueSPtr er;
        notifyProgress(sender, 50);
        const std::string changeRequest = "CONFIG SET " + res.newItem_.first + " " + res.newItem_.second;
        RootLocker lock = make_locker(sender, changeRequest);
        FastoObjectPtr root = lock.root_;
        int resEx = impl_->execute(changeRequest.c_str(), Command::InnerCommand, root, er);
        if(resEx == REDIS_ERR){
            res.setErrorInfo(er);
        }
        else{
            res.isChange_ = true;
        }
        notifyProgress(sender, 75);
            reply(sender, new Events::ChangeServerPropertyInfoResponceEvent(this, res));
        notifyProgress(sender, 100);
    }

    ServerInfoSPtr RedisDriver::makeServerInfoFromString(const std::string& val)
    {
        return makeRedisServerInfo(val);
    }

    void RedisDriver::interrupt()
    {
        impl_->config.shutdown = 1;
    }
}
