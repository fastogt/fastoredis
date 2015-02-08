#pragma once

#include "core/types.h"

#define MEMCACHED_COMMON_LABEL "# Common"

#define MEMCACHED_PID_LABEL "pid"
#define MEMCACHED_UPTIME_LABEL "uptime"
#define MEMCACHED_TIME_LABEL "time"
#define MEMCACHED_VERSION_LABEL "version"
#define MEMCACHED_POINTER_SIZE_LABEL "pointer_size"
#define MEMCACHED_RUSAGE_USER_LABEL "rusage_user"
#define MEMCACHED_RUSAGE_SYSTEM_LABEL "rusage_system"
#define MEMCACHED_CURR_ITEMS_LABEL "curr_items"
#define MEMCACHED_TOTAL_ITEMS_LABEL "total_items"
#define MEMCACHED_BYTES_LABEL "bytes"
#define MEMCACHED_CURR_CONNECTIONS_LABEL "curr_connections"
#define MEMCACHED_TOTAL_CONNECTIONS_LABEL "total_connections"
#define MEMCACHED_CONNECTION_STRUCTURES_LABEL "connection_structures"
#define MEMCACHED_CMD_GET_LABEL "cmd_get"
#define MEMCACHED_CMD_SET_LABEL "cmd_set"
#define MEMCACHED_GET_HITS_LABEL "get_hits"
#define MEMCACHED_GET_MISSES_LABEL "get_misses"
#define MEMCACHED_EVICTIONS_LABEL "evictions"
#define MEMCACHED_BYTES_READ_LABEL "bytes_read"
#define MEMCACHED_BYTES_WRITTEN_LABEL "bytes_written"
#define MEMCACHED_LIMIT_MAXBYTES_LABEL "limit_maxbytes"
#define MEMCACHED_THREADS_LABEL "threads"

namespace fastoredis
{
    static const std::vector<std::string> memcachedHeaders =
    {
        MEMCACHED_COMMON_LABEL
    };

    static const std::vector<Field> memcachedCommonFields =
    {
        Field(MEMCACHED_PID_LABEL, common::Value::TYPE_UINTEGER),
        Field(MEMCACHED_UPTIME_LABEL, common::Value::TYPE_UINTEGER),
        Field(MEMCACHED_TIME_LABEL, common::Value::TYPE_UINTEGER),
        Field(MEMCACHED_VERSION_LABEL, common::Value::TYPE_STRING),
        Field(MEMCACHED_POINTER_SIZE_LABEL, common::Value::TYPE_UINTEGER),
        Field(MEMCACHED_RUSAGE_USER_LABEL, common::Value::TYPE_UINTEGER),
        Field(MEMCACHED_RUSAGE_SYSTEM_LABEL, common::Value::TYPE_UINTEGER),
        Field(MEMCACHED_CURR_ITEMS_LABEL, common::Value::TYPE_UINTEGER),
        Field(MEMCACHED_TOTAL_ITEMS_LABEL, common::Value::TYPE_UINTEGER),
        Field(MEMCACHED_BYTES_LABEL, common::Value::TYPE_UINTEGER),
        Field(MEMCACHED_CURR_CONNECTIONS_LABEL, common::Value::TYPE_UINTEGER),
        Field(MEMCACHED_TOTAL_CONNECTIONS_LABEL, common::Value::TYPE_UINTEGER),
        Field(MEMCACHED_CONNECTION_STRUCTURES_LABEL, common::Value::TYPE_UINTEGER),
        Field(MEMCACHED_CMD_GET_LABEL, common::Value::TYPE_UINTEGER),
        Field(MEMCACHED_CMD_SET_LABEL, common::Value::TYPE_UINTEGER),
        Field(MEMCACHED_GET_HITS_LABEL, common::Value::TYPE_UINTEGER),
        Field(MEMCACHED_GET_MISSES_LABEL, common::Value::TYPE_UINTEGER),
        Field(MEMCACHED_EVICTIONS_LABEL, common::Value::TYPE_UINTEGER),
        Field(MEMCACHED_BYTES_READ_LABEL, common::Value::TYPE_UINTEGER),
        Field(MEMCACHED_BYTES_WRITTEN_LABEL, common::Value::TYPE_UINTEGER),
        Field(MEMCACHED_LIMIT_MAXBYTES_LABEL, common::Value::TYPE_UINTEGER),
        Field(MEMCACHED_THREADS_LABEL, common::Value::TYPE_UINTEGER)
    };

    static const std::vector<std::vector<Field> > memcachedFields =
    {
        memcachedCommonFields
    };

    class MemcachedServerInfo
            : public ServerInfo
    {
    public:
        struct Common
                : FieldByIndex
        {
            Common();
            explicit Common(const std::string& common_text);
            common::Value* valueByIndex(unsigned char index) const;

            uint32_t pid_;
            uint32_t uptime_;
            uint32_t time_;
            std::string version_;
            uint32_t pointer_size_;
            uint32_t rusage_user_;
            uint32_t rusage_system_;
            uint32_t curr_items_;
            uint32_t total_items_;
            uint32_t bytes_;
            uint32_t curr_connections_;
            uint32_t total_connections_;
            uint32_t connection_structures_;
            uint32_t cmd_get_;
            uint32_t cmd_set_;
            uint32_t get_hits_;
            uint32_t get_misses_;
            uint32_t evictions_;
            uint32_t bytes_read_;
            uint32_t bytes_written_;
            uint32_t limit_maxbytes_;
            uint32_t threads_;
        } common_;

        MemcachedServerInfo();
        MemcachedServerInfo(const Common& common);
        virtual common::Value* valueByIndexes(unsigned char property, unsigned char field) const;
        virtual std::string toString() const;
    };

    std::ostream& operator << (std::ostream& out, const MemcachedServerInfo& value);

    MemcachedServerInfo* makeMemcachedServerInfo(const std::string &content);
    MemcachedServerInfo* makeMemcachedServerInfo(FastoObject *root);

    class MemcachedBaseInfo
            : public DataBaseInfo
    {
    public:
        MemcachedBaseInfo(const std::string& name, size_t size, bool isDefault);
        virtual DataBaseInfo* clone() const;
    };
}
