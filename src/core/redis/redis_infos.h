#pragma once

#include <vector>

#include "common/types.h"
#include "global/global.h"
#include "core/types.h"

#define REDIS_SERVER_LABEL "# Server"
#define REDIS_CLIENTS_LABEL "# Clients"
#define REDIS_MEMORY_LABEL "# Memory"
#define REDIS_PERSISTENCE_LABEL "# Persistence"
#define REDIS_STATS_LABEL "# Stats"
#define REDIS_REPLICATION_LABEL "# Replication"
#define REDIS_CPU_LABEL "# CPU"
#define REDIS_KEYSPACE_LABEL "# Keyspace"

//Server
#define REDIS_VERSION_LABEL "redis_version"
#define REDIS_GIT_SHA1_LABEL "redis_git_sha1"
#define REDIS_GIT_DIRTY_LABEL "redis_git_dirty"
#define REDIS_BUILD_ID_LABEL "redis_build_id"
#define REDIS_MODE_LABEL "redis_mode"
#define REDIS_OS_LABEL "os"
#define REDIS_ARCH_BITS_LABEL "arch_bits"
#define REDIS_MULTIPLEXING_API_LABEL "multiplexing_api"
#define REDIS_GCC_VERSION_LABEL "gcc_version"
#define REDIS_PROCESS_ID_LABEL "process_id"
#define REDIS_RUN_ID_LABEL "run_id"
#define REDIS_TCP_PORT_LABEL "tcp_port"
#define REDIS_UPTIME_IN_SECONDS_LABEL "uptime_in_seconds"
#define REDIS_UPTIME_IN_DAYS_LABEL "uptime_in_days"
#define REDIS_HZ_LABEL "hz"
#define REDIS_LRU_CLOCK_LABEL "lru_clock"

//Clients
#define REDIS_CONNECTED_CLIENTS_LABEL "connected_clients"
#define REDIS_CLIENT_LONGEST_OUTPUT_LIST_LABEL "client_longest_output_list"
#define REDIS_CLIENT_BIGGEST_INPUT_BUF_LABEL "client_biggest_input_buf"
#define REDIS_BLOCKED_CLIENTS_LABEL "blocked_clients"

//Memory
#define REDIS_USED_MEMORY_LABEL "used_memory"
#define REDIS_USED_MEMORY_HUMAN_LABEL "used_memory_human"
#define REDIS_USED_MEMORY_RSS_LABEL "used_memory_rss"
#define REDIS_USED_MEMORY_PEAK_LABEL "used_memory_peak"
#define REDIS_USED_MEMORY_PEAK_HUMAN_LABEL "used_memory_peak_human"
#define REDIS_USED_MEMORY_LUA_LABEL "used_memory_lua"
#define REDIS_MEM_FRAGMENTATION_RATIO_LABEL "mem_fragmentation_ratio"
#define REDIS_MEM_ALLOCATOR_LABEL "mem_allocator"

//Persistence
#define REDIS_LOADING_LABEL "loading"
#define REDIS_RDB_CHANGES_SINCE_LAST_SAVE_LABEL "rdb_changes_since_last_save"
#define REDIS_RDB_DGSAVE_IN_PROGRESS_LABEL "rdb_bgsave_in_progress"
#define REDIS_RDB_LAST_SAVE_TIME_LABEL "rdb_last_save_time"
#define REDIS_RDB_LAST_DGSAVE_STATUS_LABEL "rdb_last_bgsave_status"
#define REDIS_RDB_LAST_DGSAVE_TIME_SEC_LABEL "rdb_last_bgsave_time_sec"
#define REDIS_RDB_CURRENT_DGSAVE_TIME_SEC_LABEL "rdb_current_bgsave_time_sec"
#define REDIS_AOF_ENABLED_LABEL "aof_enabled"
#define REDIS_AOF_REWRITE_IN_PROGRESS_LABEL "aof_rewrite_in_progress"
#define REDIS_AOF_REWRITE_SHEDULED_LABEL "aof_rewrite_scheduled"
#define REDIS_AOF_LAST_REWRITE_TIME_SEC_LABEL "aof_last_rewrite_time_sec"
#define REDIS_AOF_CURRENT_REWRITE_TIME_SEC_LABEL "aof_current_rewrite_time_sec"
#define REDIS_AOF_LAST_DGREWRITE_STATUS_LABEL "aof_last_bgrewrite_status"
#define REDIS_AOF_LAST_WRITE_STATUS_LABEL "aof_last_write_status"

//Stats
#define REDIS_TOTAL_CONNECTIONS_RECEIVED_LABEL "total_connections_received"
#define REDIS_TOTAL_COMMANDS_PROCESSED_LABEL "total_commands_processed"
#define REDIS_INSTANTANEOUS_OPS_PER_SEC_LABEL "instantaneous_ops_per_sec"
#define REDIS_REJECTED_CONNECTIONS_LABEL "rejected_connections"
#define REDIS_SYNC_FULL_LABEL "sync_full"
#define REDIS_SYNC_PARTIAL_OK_LABEL "sync_partial_ok"
#define REDIS_SYNC_PARTIAL_ERR_LABEL "sync_partial_err"
#define REDIS_EXPIRED_KEYS_LABEL "expired_keys"
#define REDIS_EVICTED_KEYS_LABEL "evicted_keys"
#define REDIS_KEYSPACE_HITS_LABEL "keyspace_hits"
#define REDIS_KEYSPACE_MISSES_LABEL "keyspace_misses"
#define REDIS_PUBSUB_CHANNELS_LABEL "pubsub_channels"
#define REDIS_PUBSUB_PATTERNS_LABEL "pubsub_patterns"
#define REDIS_LATEST_FORK_USEC_LABEL "latest_fork_usec"

//Replication
#define REDIS_ROLE_LABEL "role"
#define REDIS_CONNECTED_SLAVES_LABEL "connected_slaves"
#define REDIS_REPL_OFFSET_LABEL "master_repl_offset"
#define REDIS_BACKLOG_ACTIVE_LABEL "repl_backlog_active"
#define REDIS_BACKLOG_SIZE_LABEL "repl_backlog_size"
#define REDIS_BACKLOG_FIRST_BYTE_OFFSET_LABEL "repl_backlog_first_byte_offset"
#define REDIS_BACKLOG_HISTEN_LABEL "repl_backlog_histlen"

//CPU
#define REDIS_USED_CPU_SYS_LABEL "used_cpu_sys"
#define REDIS_USED_CPU_USER_LABEL "used_cpu_user"
#define REDIS_USED_CPU_SYS_CHILDREN_LABEL "used_cpu_sys_children"
#define REDIS_USED_CPU_USER_CHILDREN_LABEL "used_cpu_user_children"

namespace fastoredis
{
    static const std::string redisHeaders[] =
    {
        REDIS_SERVER_LABEL,
        REDIS_CLIENTS_LABEL,
        REDIS_MEMORY_LABEL,
        REDIS_PERSISTENCE_LABEL,
        REDIS_STATS_LABEL,
        REDIS_REPLICATION_LABEL,
        REDIS_CPU_LABEL,
        REDIS_KEYSPACE_LABEL
    };

    static const std::string redisServerFields[] =
    {
        REDIS_VERSION_LABEL,
        REDIS_GIT_SHA1_LABEL,
        REDIS_GIT_DIRTY_LABEL,
        REDIS_BUILD_ID_LABEL,
        REDIS_MODE_LABEL,
        REDIS_OS_LABEL,
        REDIS_ARCH_BITS_LABEL,
        REDIS_MULTIPLEXING_API_LABEL,
        REDIS_GCC_VERSION_LABEL,
        REDIS_PROCESS_ID_LABEL,
        REDIS_RUN_ID_LABEL,
        REDIS_TCP_PORT_LABEL,
        REDIS_UPTIME_IN_SECONDS_LABEL,
        REDIS_UPTIME_IN_DAYS_LABEL,
        REDIS_HZ_LABEL,
        REDIS_LRU_CLOCK_LABEL
    };

    static const unsigned char redisServerFieldsIntIndexes[] = { 6,9,11,12,13,14,15 };

    static const std::string redisClientFields[] =
    {
        REDIS_CONNECTED_CLIENTS_LABEL,
        REDIS_CLIENT_LONGEST_OUTPUT_LIST_LABEL,
        REDIS_CLIENT_BIGGEST_INPUT_BUF_LABEL,
        REDIS_BLOCKED_CLIENTS_LABEL
    };

    static const unsigned char redisClientFieldsIntIndexes[] = { 0,1,2,3 };

    static const std::string redisMemoryFields[] =
    {
        REDIS_USED_MEMORY_LABEL,
        REDIS_USED_MEMORY_HUMAN_LABEL,
        REDIS_USED_MEMORY_RSS_LABEL,
        REDIS_USED_MEMORY_PEAK_LABEL,
        REDIS_USED_MEMORY_PEAK_HUMAN_LABEL,
        REDIS_USED_MEMORY_LUA_LABEL,
        REDIS_MEM_FRAGMENTATION_RATIO_LABEL,
        REDIS_MEM_ALLOCATOR_LABEL
    };

    static const unsigned char redisMemoryFieldsIntIndexes[] = { 0,2,3,5,6 };

    static const std::string redisPersistenceFields[] =
    {
        REDIS_LOADING_LABEL,//
        REDIS_RDB_CHANGES_SINCE_LAST_SAVE_LABEL,//
        REDIS_RDB_DGSAVE_IN_PROGRESS_LABEL,//
        REDIS_RDB_LAST_SAVE_TIME_LABEL,//
        REDIS_RDB_LAST_DGSAVE_STATUS_LABEL,//
        REDIS_RDB_LAST_DGSAVE_TIME_SEC_LABEL,//
        REDIS_RDB_CURRENT_DGSAVE_TIME_SEC_LABEL,
        REDIS_AOF_ENABLED_LABEL,//
        REDIS_AOF_REWRITE_IN_PROGRESS_LABEL,//
        REDIS_AOF_REWRITE_SHEDULED_LABEL,//
        REDIS_AOF_LAST_REWRITE_TIME_SEC_LABEL,//
        REDIS_AOF_CURRENT_REWRITE_TIME_SEC_LABEL,//
        REDIS_AOF_LAST_DGREWRITE_STATUS_LABEL,
        REDIS_AOF_LAST_WRITE_STATUS_LABEL
    };

    static const unsigned char redisPersistenceFieldsIntIndexes[] = { 0,1,2,3,5,6,7,8,9,10,11 };

    static const std::string redisStatsFields[] =
    {
        REDIS_TOTAL_CONNECTIONS_RECEIVED_LABEL,
        REDIS_TOTAL_COMMANDS_PROCESSED_LABEL,
        REDIS_INSTANTANEOUS_OPS_PER_SEC_LABEL,
        REDIS_REJECTED_CONNECTIONS_LABEL,
        REDIS_SYNC_FULL_LABEL,
        REDIS_SYNC_PARTIAL_OK_LABEL,
        REDIS_SYNC_PARTIAL_ERR_LABEL,
        REDIS_EXPIRED_KEYS_LABEL,
        REDIS_EVICTED_KEYS_LABEL,
        REDIS_KEYSPACE_HITS_LABEL,
        REDIS_KEYSPACE_MISSES_LABEL,
        REDIS_PUBSUB_CHANNELS_LABEL,
        REDIS_PUBSUB_PATTERNS_LABEL,
        REDIS_LATEST_FORK_USEC_LABEL
    };

    static const unsigned char redisStatsFieldsIntIndexes[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13 };

    static const std::string redisReplicationFields[] =
    {
        REDIS_ROLE_LABEL,
        REDIS_CONNECTED_SLAVES_LABEL,
        REDIS_REPL_OFFSET_LABEL,
        REDIS_BACKLOG_ACTIVE_LABEL,
        REDIS_BACKLOG_SIZE_LABEL,
        REDIS_BACKLOG_FIRST_BYTE_OFFSET_LABEL,
        REDIS_BACKLOG_HISTEN_LABEL
    };

    static const unsigned char redisReplicationFieldsIntIndexes[] = { 1,2,3,4,5,6 };

    static const std::string redisCpuFields[] =
    {
        REDIS_USED_CPU_SYS_LABEL,
        REDIS_USED_CPU_USER_LABEL,
        REDIS_USED_CPU_SYS_CHILDREN_LABEL,
        REDIS_USED_CPU_USER_CHILDREN_LABEL
    };

    static const unsigned char redisCpuFieldsIntIndexes[] = { 0,1,2,3 };

    static const std::string redisKeySpaceFields[] =
    {

    };

    static const unsigned char redisKeySpaceFieldsIntIndexes[] = {};

    template<typename T>
    struct FieldProperty
    {
        FieldProperty(T mass, unsigned char size)
            : mass_(mass), size_(size)
        {

        }
        const T mass_;
        const unsigned char size_;
    };

    typedef FieldProperty<const std::string*> FieldPropertyS;
    typedef FieldProperty<const unsigned char*> FieldPropertyU;

    static const std::pair<FieldPropertyS,FieldPropertyU> redisFields[] =
    {
        std::make_pair(FieldPropertyS(redisServerFields, SIZEOFMASS(redisServerFields)),
            FieldPropertyU(redisServerFieldsIntIndexes, SIZEOFMASS(redisServerFieldsIntIndexes))),
        std::make_pair(FieldPropertyS(redisClientFields, SIZEOFMASS(redisClientFields)),
            FieldPropertyU(redisClientFieldsIntIndexes, SIZEOFMASS(redisClientFieldsIntIndexes))),
        std::make_pair(FieldPropertyS(redisMemoryFields, SIZEOFMASS(redisMemoryFields)),
            FieldPropertyU(redisMemoryFieldsIntIndexes, SIZEOFMASS(redisMemoryFieldsIntIndexes))),
        std::make_pair(FieldPropertyS(redisPersistenceFields, SIZEOFMASS(redisPersistenceFields)),
            FieldPropertyU(redisPersistenceFieldsIntIndexes, SIZEOFMASS(redisPersistenceFieldsIntIndexes))),
        std::make_pair(FieldPropertyS(redisStatsFields, SIZEOFMASS(redisStatsFields)),
            FieldPropertyU(redisStatsFieldsIntIndexes, SIZEOFMASS(redisStatsFieldsIntIndexes))),
        std::make_pair(FieldPropertyS(redisReplicationFields, SIZEOFMASS(redisReplicationFields)),
            FieldPropertyU(redisReplicationFieldsIntIndexes, SIZEOFMASS(redisReplicationFieldsIntIndexes))),
        std::make_pair(FieldPropertyS(redisCpuFields, SIZEOFMASS(redisCpuFields)),
            FieldPropertyU(redisCpuFieldsIntIndexes, SIZEOFMASS(redisCpuFieldsIntIndexes))),
        std::make_pair(FieldPropertyS(redisKeySpaceFields, SIZEOFMASS(redisKeySpaceFields)),
            FieldPropertyU(redisKeySpaceFieldsIntIndexes, SIZEOFMASS(redisKeySpaceFieldsIntIndexes)))
    };

    template<typename T>
    struct FiledByIdex
    {
        common::Value* valueByIndex(unsigned char index) const;
    };

    struct RedisServerInfo
            : public ServerInfo
    {
        struct Server
                : FiledByIdex<Server>
        {
            Server();
            explicit Server(const std::string& server_text);
            common::Value* valueByIndex(unsigned char index) const;

            std::string redis_version_;
            std::string redis_git_sha1_;
            std::string redis_git_dirty_;
            std::string redis_mode_;
            std::string os_;
            uint32_t arch_bits_;
            std::string multiplexing_api_;
            std::string gcc_version_;
            uint32_t process_id_;
            std::string run_id_;
            uint32_t tcp_port_;
            uint32_t uptime_in_seconds_;
            uint32_t uptime_in_days_;
            uint32_t lru_clock_;
        } server_;

        struct Clients
                : FiledByIdex<Clients>
        {
            Clients();
            explicit Clients(const std::string& client_text);
            common::Value* valueByIndex(unsigned char index) const;

            uint32_t connected_clients_;
            uint32_t client_longest_output_list_;
            uint32_t client_biggest_input_buf_;
            uint32_t blocked_clients_;
        } clients_;

        struct Memory
                : FiledByIdex<Server>
        {
            Memory();
            explicit Memory(const std::string& memory_text);
            common::Value* valueByIndex(unsigned char index) const;

            uint32_t used_memory_;
            std::string used_memory_human_;
            uint32_t used_memory_rss_;
            uint32_t used_memory_peak_;
            std::string used_memory_peak_human_;
            uint32_t used_memory_lua_;
            float mem_fragmentation_ratio_;
            std::string mem_allocator_;
        } memory_;

        struct Persistence
                : FiledByIdex<Server>
        {
            Persistence();
            explicit Persistence(const std::string& persistence_text);
            common::Value* valueByIndex(unsigned char index) const;

            uint32_t loading_;
            uint32_t rdb_changes_since_last_save_;
            uint32_t rdb_bgsave_in_progress_;
            uint32_t rdb_last_save_time_;
            std::string rdb_last_bgsave_status_;
            int rdb_last_bgsave_time_sec_;
            int rdb_current_bgsave_time_sec_;
            uint32_t aof_enabled_;
            uint32_t aof_rewrite_in_progress_;
            uint32_t aof_rewrite_scheduled_;
            int aof_last_rewrite_time_sec_;
            int aof_current_rewrite_time_sec_;
            std::string aof_last_bgrewrite_status_;
        } persistence_;

        struct Stats
                : FiledByIdex<Stats>
        {
            Stats();
            explicit Stats(const std::string& stats_text);
            common::Value* valueByIndex(unsigned char index) const;

            uint32_t total_connections_received_;
            uint32_t total_commands_processed_;
            uint32_t instantaneous_ops_per_sec_;
            uint32_t rejected_connections_;
            uint32_t expired_keys_;
            uint32_t evicted_keys_;
            uint32_t keyspace_hits_;
            uint32_t keyspace_misses_;
            uint32_t pubsub_channels_;
            uint32_t pubsub_patterns_;
            uint32_t latest_fork_usec_;
        } stats_;

        struct Replication
                : FiledByIdex<Server>
        {
            Replication();
            explicit Replication(const std::string& replication_text);
            common::Value* valueByIndex(unsigned char index) const;

            std::string role_;
            uint32_t connected_slaves_;
        } replication_;

        struct Cpu
                : FiledByIdex<Cpu>
        {
            Cpu();
            explicit Cpu(const std::string& cpu_text);
            common::Value* valueByIndex(unsigned char index) const;

            float used_cpu_sys_;
            float used_cpu_user_;
            float used_cpu_sys_children_;
            float used_cpu_user_children_;
        } cpu_;

        struct Keyspace
                : FiledByIdex<Keyspace>
        {
            common::Value* valueByIndex(unsigned char index) const;
        } keySp_;

        virtual common::Value* valueByIndexes(unsigned char property, unsigned char field) const;

        RedisServerInfo();
        RedisServerInfo(const Server &serv, const Clients &clients, const Memory &memory,
                   const Persistence &pers, const Stats &stats, const Replication &repl, const Cpu &cpu, const Keyspace &key);
    };

    std::ostream& operator<<(std::ostream& out, const RedisServerInfo& value);

    ServerInfoSPtr makeRedisServerInfo(const std::string &content);
    ServerInfoSPtr makeRedisServerInfo(FastoObject *root);
}
