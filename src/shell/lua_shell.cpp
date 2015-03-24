#include "shell/lua_shell.h"

#include <Qsci/qscilexerlua.h>
#include <Qsci/qsciapis.h>

#include "core/lua_engine.h"

namespace
{
    const QString redis_lua_commands[] =
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
    class RedisLuaLexer
            : public QsciLexerLua
    {
    public:
        RedisLuaLexer(QObject* parent = 0)
            : QsciLexerLua(parent)
        {
            QsciAPIs* api = new QsciAPIs(this);
            for(int i = 0; i < SIZEOFMASS(redis_lua_commands); ++i){
                api->add(redis_lua_commands[i]);
            }
            api->prepare();
            setAPIs(api);
        }
    };

    LuaShell::LuaShell(bool showAutoCompl, QWidget* parent)
        : FastoEditorShell(LuaEngine::version(), showAutoCompl, parent)
    {
        QsciLexerLua* lexer = new QsciLexerLua(this);
        setLexer(lexer);

        setAutoCompletionThreshold(1);
        if(showAutoCompletion_){
            setAutoCompletionSource(QsciScintilla::AcsAPIs);
        }
        else{
            setAutoCompletionSource(QsciScintilla::AcsNone);
        }
        setAutoCompletionCaseSensitivity(false);
        setIndentationsUseTabs(false);

        setIndentationWidth(4);
        setTabWidth(4);
    }

    RedisLuaShell::RedisLuaShell(bool showAutoCompl, QWidget* parent)
        : LuaShell(showAutoCompl, parent)
    {
        RedisLuaLexer* lexer = new RedisLuaLexer(this);
        setLexer(lexer);
    }
}

