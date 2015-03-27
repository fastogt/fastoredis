#include "shell/python_shell.h"

#include <Qsci/qscilexerpython.h>
#include <Qsci/qsciapis.h>

#include "core/python_engine.h"

namespace
{
    const QString redis_py_commands[] =
    {
        "lrem", "pipeline", "setex", "zadd", //redis.Redis

        "append", "bgrewriteaof", "bgsave", "blpop", "brpop",
        "brpoplpush", "config_get", "config_set", "dbsize",
        "debug_object", "decr", "delete", "echo", "execute_command",
        "exists", "expire", "expireat", "flushall", "flushdb", "get",
        "getbit", "getset", "hdel", "hexists", "hget", "hgetall",
        "hincrby", "hkeys", "hlen", "hmget", "hmset", "hset",
        "hsetnx", "hvals", "incr", "info", "keys", "lastsave",
        "lindex", "linsert", "llen", "lock", "lpop", "lpush",
        "lpushx", "lrange", "lrem", "lset", "ltrim", "mget",
        "move", "mset", "msetnx", "object", "parse_response", "persist",
        "ping", "pipeline", "publish", "pubsub", "randomkey", "rename",
        "renamenx", "rpop", "rpoplpush", "rpush", "rpushx", "sadd",
        "save", "scard", "sdiff", "sdiffstore", "set", "set_response_callback",
        "setbit", "setex", "setnx", "setrange", "shutdown", "sinter",
        "sinterstore", "sismember", "slaveof", "slowlog_get", "slowlog_len", "slowlog_reset",
        "smembers", "smove", "sort", "spop", "srandmember", "srem",
        "strlen", "substr", "sunion", "sunionstore", "transaction", "ttl",
        "type", "unwatch", "watch", "zadd", "zcard", "zincrby",
        "zinterstore", "zrange", "zrangebyscore", "zrank", "zrem", "zremrangebyrank",
        "zremrangebyscore", "zrevrange", "zrevrangebyscore", "zrevrank", "zscore",
        "zunionstore", "disconnect", "get_connection", "make_connection", "release", //redis.StrictRedis

        "connect", "disconnect", "encode",  //redis.Connection
        "on_connect", "pack_command", "read_response",
        "send_command", "send_packed_command", "redis"
    };
}

namespace fastoredis
{
    class RedisPythonLexer
            : public QsciLexerPython
    {
    public:
        RedisPythonLexer(QObject* parent = 0)
            : QsciLexerPython(parent)
        {
            QsciAPIs* api = new QsciAPIs(this);
            for(int i = 0; i < SIZEOFMASS(redis_py_commands); ++i){
                api->add(redis_py_commands[i]);
            }
            api->prepare();
            setAPIs(api);
        }
    };

    PythonShell::PythonShell(bool showAutoCompl, QWidget* parent)
        : FastoEditorShell(PythonEngine::version(), showAutoCompl, parent)
    {
        QsciLexerPython* lexer = new QsciLexerPython(this);
        setLexer(lexer);
    }

    RedisPyShell::RedisPyShell(bool showAutoCompl, QWidget* parent)
        : PythonShell(showAutoCompl, parent)
    {
        RedisPythonLexer* lexer = new RedisPythonLexer(this);
        setLexer(lexer);
    }
}

