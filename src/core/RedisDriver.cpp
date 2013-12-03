#include "core/RedisDriver.h"

#include <hiredis/hiredis.h>
#ifdef __cplusplus
extern "C" {
#endif
#include <anet.h>
#ifdef __cplusplus
}
#endif

#define REDIS_CLI_KEEPALIVE_INTERVAL 15 /* seconds */
namespace
{
    const char *toCString(const std::string &val)
    {
        return val.empty() ? NULL : val.c_str();
    }
}

namespace fastoredis
{
    struct RedisDriver::pimpl
    {
        pimpl(): _context(NULL) {}

        void connect(const hostAndPort &h, const std::string &hostsock, const std::string &a, int dbnum, EventsInfo::ConnectInfoResponce &res)
        {
            using namespace error;
            if(!_context){
                const char *hostip = toCString(h.first);
                int hostport = h.second;
                const char *hostsocket = toCString(hostsock);
                const char *auth = toCString(a);
                if(hostsocket){
                    _context = redisConnectUnix(hostsocket);
                }
                else{
                    _context = redisConnect(hostip,hostport);
                }
                if(_context->err){
                    ErrorInfo e(_context->errstr, ErrorInfo::ERROR);
                    res.setErrorInfo(e);
                    redisFree(_context);
                    _context = NULL;
                    res._prompt = "not connected> ";
                    return;
                }

                anetKeepAlive(NULL, _context->fd, REDIS_CLI_KEEPALIVE_INTERVAL);

                /* Do AUTH and select the right DB. */
                if (auth != NULL){
                    redisReply *reply = static_cast<redisReply*>(redisCommand(_context,"AUTH %s",auth));
                    if (reply != NULL) {
                        freeReplyObject(reply);
                    }
                    else{
                        ErrorInfo e("AUTH", ErrorInfo::ERROR);
                        res.setErrorInfo(e);
                        return;
                    }
                }

                if (dbnum != 0){
                    redisReply *reply = static_cast<redisReply*>(redisCommand(_context,"SELECT %d",dbnum));
                    if (reply != NULL) {
                        freeReplyObject(reply);
                    }
                    else{
                        ErrorInfo e("SELECT", ErrorInfo::ERROR);
                        res.setErrorInfo(e);
                        return;
                    }
                }

                char prompt[512] = {0};
                int len = 0;

                if (hostsocket != NULL)
                    len = snprintf(prompt,sizeof(prompt),PROJECT_NAME_LOWERCASE" %s", hostsocket);
                else
                    len = snprintf(prompt,sizeof(prompt), strchr(hostip,':') ? PROJECT_NAME_LOWERCASE" [%s]:%d" : PROJECT_NAME_LOWERCASE" %s:%d", hostip, hostport);

                /* Add [dbnum] if needed */
                if (dbnum != 0)
                    len += snprintf(prompt+len,sizeof(prompt)-len,"[%d]", dbnum);

                snprintf(prompt+len,sizeof(prompt)-len,"> ");

                res._prompt = prompt;
            }
        }

        void query(EventsInfo::ExecuteInfoResponce &res)
        {
            using namespace error;
            const char *command = toCString(res._text);
            if(command){
                redisReply *reply = static_cast<redisReply *>(redisCommand(_context,"%s",command));
                if (reply != NULL) {
                    res._out = new FastoObject(static_cast<fastoType>(reply->type), reply->str);
                    if(reply->type == REDIS_REPLY_ERROR){
                        ErrorInfo e(reply->str, ErrorInfo::ERROR);
                        res.setErrorInfo(e);
                    }
                    freeReplyObject(reply);
                }
            }
            else{
                ErrorInfo e("Empty command", ErrorInfo::ERROR);
                res.setErrorInfo(e);
            }
        }

        void disconnect()
        {
            redisFree(_context);
            _context = NULL;
        }

        ~pimpl()
        {
            if(_context){
                redisFree(_context);
                _context = NULL;
            }
        }

        redisContext *_context;
    };

    RedisDriver::RedisDriver(const IConnectionSettingsBasePtr &settings)
        :base_class(settings), _impl(new pimpl)
    {
    }

    bool RedisDriver::isConnected() const
    {
        return _impl->_context;
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
            hostAndPort h = conf.host;
            std::string hs = conf.hostsocket;
            std::string auth = conf.auth;
            int num = conf.dbnum;
            _impl->connect(h, hs, auth, num, res);
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
