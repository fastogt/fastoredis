#pragma once

#include "core/types.h"

#define SSDB_COMMON_LABEL "# Common"

#define SSDB_PID_LABEL "pid"
#define SSDB_UPTIME_LABEL "uptime"
#define SSDB_TIME_LABEL "time"
#define SSDB_VERSION_LABEL "version"
#define SSDB_POINTER_SIZE_LABEL "pointer_size"
#define SSDB_RUSAGE_USER_LABEL "rusage_user"
#define SSDB_RUSAGE_SYSTEM_LABEL "rusage_system"
#define SSDB_CURR_ITEMS_LABEL "curr_items"
#define SSDB_TOTAL_ITEMS_LABEL "total_items"
#define SSDB_BYTES_LABEL "bytes"
#define SSDB_CURR_CONNECTIONS_LABEL "curr_connections"
#define SSDB_TOTAL_CONNECTIONS_LABEL "total_connections"
#define SSDB_CONNECTION_STRUCTURES_LABEL "connection_structures"
#define SSDB_CMD_GET_LABEL "cmd_get"
#define SSDB_CMD_SET_LABEL "cmd_set"
#define SSDB_GET_HITS_LABEL "get_hits"
#define SSDB_GET_MISSES_LABEL "get_misses"
#define SSDB_EVICTIONS_LABEL "evictions"
#define SSDB_BYTES_READ_LABEL "bytes_read"
#define SSDB_BYTES_WRITTEN_LABEL "bytes_written"
#define SSDB_LIMIT_MAXBYTES_LABEL "limit_maxbytes"
#define SSDB_THREADS_LABEL "threads"

namespace fastoredis
{
    static const std::vector<std::string> SsdbHeaders =
    {
        SSDB_COMMON_LABEL
    };

    static const std::vector<Field> SsdbCommonFields =
    {
        Field(SSDB_PID_LABEL, common::Value::TYPE_UINTEGER),
        Field(SSDB_UPTIME_LABEL, common::Value::TYPE_UINTEGER),
        Field(SSDB_TIME_LABEL, common::Value::TYPE_UINTEGER),
        Field(SSDB_VERSION_LABEL, common::Value::TYPE_STRING),
        Field(SSDB_POINTER_SIZE_LABEL, common::Value::TYPE_UINTEGER),
        Field(SSDB_RUSAGE_USER_LABEL, common::Value::TYPE_UINTEGER),
        Field(SSDB_RUSAGE_SYSTEM_LABEL, common::Value::TYPE_UINTEGER),
        Field(SSDB_CURR_ITEMS_LABEL, common::Value::TYPE_UINTEGER),
        Field(SSDB_TOTAL_ITEMS_LABEL, common::Value::TYPE_UINTEGER),
        Field(SSDB_BYTES_LABEL, common::Value::TYPE_UINTEGER),
        Field(SSDB_CURR_CONNECTIONS_LABEL, common::Value::TYPE_UINTEGER),
        Field(SSDB_TOTAL_CONNECTIONS_LABEL, common::Value::TYPE_UINTEGER),
        Field(SSDB_CONNECTION_STRUCTURES_LABEL, common::Value::TYPE_UINTEGER),
        Field(SSDB_CMD_GET_LABEL, common::Value::TYPE_UINTEGER),
        Field(SSDB_CMD_SET_LABEL, common::Value::TYPE_UINTEGER),
        Field(SSDB_GET_HITS_LABEL, common::Value::TYPE_UINTEGER),
        Field(SSDB_GET_MISSES_LABEL, common::Value::TYPE_UINTEGER),
        Field(SSDB_EVICTIONS_LABEL, common::Value::TYPE_UINTEGER),
        Field(SSDB_BYTES_READ_LABEL, common::Value::TYPE_UINTEGER),
        Field(SSDB_BYTES_WRITTEN_LABEL, common::Value::TYPE_UINTEGER),
        Field(SSDB_LIMIT_MAXBYTES_LABEL, common::Value::TYPE_UINTEGER),
        Field(SSDB_THREADS_LABEL, common::Value::TYPE_UINTEGER)
    };

    static const std::vector<std::vector<Field> > SsdbFields =
    {
        SsdbCommonFields
    };

    class SsdbServerInfo
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

        SsdbServerInfo();
        SsdbServerInfo(const Common& common);
        virtual common::Value* valueByIndexes(unsigned char property, unsigned char field) const;
        virtual std::string toString() const;
    };

    std::ostream& operator << (std::ostream& out, const SsdbServerInfo& value);

    SsdbServerInfo* makeSsdbServerInfo(const std::string &content);
    SsdbServerInfo* makeSsdbServerInfo(FastoObject *root);

    class SsdbDataBaseInfo
            : public DataBaseInfo
    {
    public:
        SsdbDataBaseInfo(const std::string& name, size_t size, bool isDefault);
        virtual DataBaseInfo* clone() const;
    };
}
