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

namespace fastoredis
{
    struct RedisDriver::pimpl
    {
        pimpl(): _context(NULL) {}

        void connect(const hostAndPort &h, const std::string &hostsock, const std::string &a, int dbnum, EventsInfo::ConnectInfoResponce &res)
        {
            if(!_context){
                const char *hostip = h.first.c_str();
                int hostport = h.second;
                const char *hostsocket = hostsock.empty() ? NULL : hostsock.c_str();
                const char *auth = a.empty() ? NULL : a.c_str();
                if(hostsocket){
                    _context = redisConnectUnix(hostsocket);
                }
                else{
                    _context = redisConnect(hostip,hostport);
                }
                if(_context->err){
                    Error::ErrorInfo e(_context->errstr, Error::ErrorInfo::_ERROR);
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
                        Error::ErrorInfo e("AUTH", Error::ErrorInfo::_ERROR);
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
                        Error::ErrorInfo e("SELECT", Error::ErrorInfo::_ERROR);
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

    }

    void RedisDriver::disconnectImpl(EventsInfo::DisConnectInfoResponce &res)
    {

    }
}
