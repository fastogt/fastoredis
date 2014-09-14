#pragma once

/**/

#include <vector>

#include "common/types.h"
#include "global/global.h"

#define REDIS_SERVER_LABEL "# Server"
#define REDIS_CLIENTS_LABEL "# Clients"
#define REDIS_MEMORY_LABEL "# Memory"
#define REDIS_PERSISTENCE_LABEL "# Persistence"
#define REDIS_STATS_LABEL "# Stats"
#define REDIS_REPLICATION_LABEL "# Replication"
#define REDIS_CPU_LABEL "# CPU"
#define REDIS_KEYSPACE_LABEL "# Keyspace"

#define REDIS_VERSION_LABEL "redis_version"
#define REDIS_GIT_SHA1_LABEL "redis_git_sha1"
#define REDIS_GIT_DIRTY_LABEL "redis_git_dirty"
#define REDIS_MODE_LABEL "redis_mode"
#define REDIS_OS_LABEL "os"
#define REDIS_ARCH_BITS_LABEL "arch_bits"
#define REDIS_MULTIPLEXING_API_LABEL "multiplexing_api"
#define REDIS_GCC_VERSION_LABEL "gcc_version"
#define REDIS_PROCESS_ID_LABEL "process_id"
#define REDIS_RUN_ID_LABEL "run_id"
#define REDIS_TCP_PORT_LABEL "tcp_port"
#define REDIS_UPTIME_IN_SECONDS_LABEL "uptime_in_seconds"
#define REDIS_UPTIME_IN_DAYS "uptime_in_days"
#define REDIS_LRU_CLOCK_LABEL "lru_clock"

#define REDIS_CONNECTED_CLIENTS_LABEL "connected_clients"
#define REDIS_CLIENT_LONGEST_OUTPUT_LIST_LABEL "client_longest_output_list"
#define REDIS_CLIENT_BIGGEST_INPUT_BUF_LABEL "client_biggest_input_buf"
#define REDIS_BLOCKED_CLIENTS_LABEL "blocked_clients"

#define REDIS_USED_MEMORY_LABEL "used_memory"
#define REDIS_USED_MEMORY_HUMAN_LABEL "used_memory_human"
#define REDIS_USED_MEMORY_RSS_LABEL "used_memory_rss"
#define REDIS_USED_MEMORY_PEAK_LABEL "used_memory_peak"
#define REDIS_USED_MEMORY_PEAK_HUMAN_LABEL "used_memory_peak_human"
#define REDIS_USED_MEMORY_LUA_LABEL "used_memory_lua"
#define REDIS_MEM_FRAGMENTATION_RATIO_LABEL "mem_fragmentation_ratio"
#define REDIS_MEM_ALLOCATOR_LABEL "mem_allocator"

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

#define REDIS_TOTAL_CONNECTIONS_RECEIVED_LABEL "total_connections_received"
#define REDIS_TOTAL_COMMANDS_PROCESSED_LABEL "total_commands_processed"
#define REDIS_INSTANTANEOUS_OPS_PER_SEC_LABEL "instantaneous_ops_per_sec"
#define REDIS_REJECTED_CONNECTIONS_LABEL "rejected_connections"
#define REDIS_EXPIRED_KEYS_LABEL "expired_keys"
#define REDIS_EVICTED_KEYS_LABEL "evicted_keys"
#define REDIS_KEYSPACE_HITS_LABEL "keyspace_hits"
#define REDIS_KEYSPACE_MISSES_LABEL "keyspace_misses"
#define REDIS_PUBSUB_CHANNELS_LABEL "pubsub_channels"
#define REDIS_PUBSUB_PATTERNS_LABEL "pubsub_patterns"
#define REDIS_LATEST_FORK_USEC_LABEL "latest_fork_usec"

#define REDIS_ROLE_LABEL "role"
#define REDIS_CONNECTED_SLAVES_LABEL "connected_slaves"

#define REDIS_USED_CPU_SYS_LABEL "used_cpu_sys"
#define REDIS_USED_CPU_USER_LABEL "used_cpu_user"
#define REDIS_USED_CPU_SYS_CHILDREN_LABEL "used_cpu_sys_children"
#define REDIS_USED_CPU_USER_CHILDREN_LABEL "used_cpu_user_children"

namespace fastoredis
{
    static const common::unicode_string redisHeaders[] = {UTEXT(REDIS_SERVER_LABEL), UTEXT(REDIS_CLIENTS_LABEL), UTEXT(REDIS_MEMORY_LABEL),
                                                      UTEXT(REDIS_PERSISTENCE_LABEL), UTEXT(REDIS_STATS_LABEL),
                                                      UTEXT(REDIS_REPLICATION_LABEL), UTEXT(REDIS_CPU_LABEL), UTEXT(REDIS_KEYSPACE_LABEL)};

    static const common::unicode_string redisServerFields[] =
    {
        UTEXT(REDIS_VERSION_LABEL),
        UTEXT(REDIS_GIT_SHA1_LABEL),
        UTEXT(REDIS_GIT_DIRTY_LABEL),
        UTEXT(REDIS_MODE_LABEL),
        UTEXT(REDIS_OS_LABEL),
        UTEXT(REDIS_ARCH_BITS_LABEL),//
        UTEXT(REDIS_MULTIPLEXING_API_LABEL),
        UTEXT(REDIS_GCC_VERSION_LABEL),
        UTEXT(REDIS_PROCESS_ID_LABEL),//
        UTEXT(REDIS_RUN_ID_LABEL),
        UTEXT(REDIS_TCP_PORT_LABEL),//
        UTEXT(REDIS_UPTIME_IN_SECONDS_LABEL),//
        UTEXT(REDIS_UPTIME_IN_DAYS),//
        UTEXT(REDIS_LRU_CLOCK_LABEL)//
    };

    static const unsigned char redisServerFieldsIntIndexes[] = { 5,8,10,11,12,13 };

    static const common::unicode_string redisClientFields[] =
    {
        UTEXT(REDIS_CONNECTED_CLIENTS_LABEL),//
        UTEXT(REDIS_CLIENT_LONGEST_OUTPUT_LIST_LABEL),//
        UTEXT(REDIS_CLIENT_BIGGEST_INPUT_BUF_LABEL),//
        UTEXT(REDIS_BLOCKED_CLIENTS_LABEL)//
    };

    static const unsigned char redisClientFieldsIntIndexes[] = { 0,1,2,3 };

    static const common::unicode_string redisMemoryFields[] =
    {
        UTEXT(REDIS_USED_MEMORY_LABEL),//
        UTEXT(REDIS_USED_MEMORY_HUMAN_LABEL),
        UTEXT(REDIS_USED_MEMORY_RSS_LABEL),//
        UTEXT(REDIS_USED_MEMORY_PEAK_LABEL),//
        UTEXT(REDIS_USED_MEMORY_PEAK_HUMAN_LABEL),
        UTEXT(REDIS_USED_MEMORY_LUA_LABEL),//
        UTEXT(REDIS_MEM_FRAGMENTATION_RATIO_LABEL),//
        UTEXT(REDIS_MEM_ALLOCATOR_LABEL)
    };

    static const unsigned char redisMemoryFieldsIntIndexes[] = { 0,2,3,5,6 };

    static const common::unicode_string redisPersistenceFields[] =
    {
        UTEXT(REDIS_LOADING_LABEL),//
        UTEXT(REDIS_RDB_CHANGES_SINCE_LAST_SAVE_LABEL),//
        UTEXT(REDIS_RDB_DGSAVE_IN_PROGRESS_LABEL),//
        UTEXT(REDIS_RDB_LAST_SAVE_TIME_LABEL),//
        UTEXT(REDIS_RDB_LAST_DGSAVE_STATUS_LABEL),//
        UTEXT(REDIS_RDB_LAST_DGSAVE_TIME_SEC_LABEL),//
        UTEXT(REDIS_RDB_CURRENT_DGSAVE_TIME_SEC_LABEL),
        UTEXT(REDIS_AOF_ENABLED_LABEL),//
        UTEXT(REDIS_AOF_REWRITE_IN_PROGRESS_LABEL),//
        UTEXT(REDIS_AOF_REWRITE_SHEDULED_LABEL),//
        UTEXT(REDIS_AOF_LAST_REWRITE_TIME_SEC_LABEL),//
        UTEXT(REDIS_AOF_CURRENT_REWRITE_TIME_SEC_LABEL),//
        UTEXT(REDIS_AOF_LAST_DGREWRITE_STATUS_LABEL)
    };

    static const unsigned char redisPersistenceFieldsIntIndexes[] = { 0,1,2,3,5,6,7,8,9,10,11,12 };

    static const common::unicode_string redisStatsFields[] =
    {
        UTEXT(REDIS_TOTAL_CONNECTIONS_RECEIVED_LABEL),//
        UTEXT(REDIS_TOTAL_COMMANDS_PROCESSED_LABEL),//
        UTEXT(REDIS_INSTANTANEOUS_OPS_PER_SEC_LABEL),//
        UTEXT(REDIS_REJECTED_CONNECTIONS_LABEL),//
        UTEXT(REDIS_EXPIRED_KEYS_LABEL),//
        UTEXT(REDIS_EVICTED_KEYS_LABEL),//
        UTEXT(REDIS_KEYSPACE_HITS_LABEL),//
        UTEXT(REDIS_KEYSPACE_MISSES_LABEL),//
        UTEXT(REDIS_PUBSUB_CHANNELS_LABEL),//
        UTEXT(REDIS_PUBSUB_PATTERNS_LABEL),//
        UTEXT(REDIS_LATEST_FORK_USEC_LABEL)//
    };

    static const unsigned char redisStatsFieldsIntIndexes[] = { 0,1,2,3,5,6,7,8,9,10 };

    static const common::unicode_string redisReplicationFields[] =
    {
        UTEXT(REDIS_ROLE_LABEL),
        UTEXT(REDIS_CONNECTED_SLAVES_LABEL)//
    };

    static const unsigned char redisReplicationFieldsIntIndexes[] = { 1 };

    static const common::unicode_string redisCpuFields[] =
    {
        UTEXT(REDIS_USED_CPU_SYS_LABEL),//
        UTEXT(REDIS_USED_CPU_USER_LABEL),//
        UTEXT(REDIS_USED_CPU_SYS_CHILDREN_LABEL),//
        UTEXT(REDIS_USED_CPU_USER_CHILDREN_LABEL)//
    };

    static const unsigned char redisCpuFieldsIntIndexes[] = { 0,1,2,3 };

    static const common::unicode_string redisKeySpaceFields[] =
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

    typedef FieldProperty<const common::unicode_string*> FieldPropertyS;
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

    struct DataBaseInfo
    {
        DataBaseInfo(const common::unicode_string& name, size_t size);
        common::unicode_string name_;
        size_t size_;
    };

    inline bool operator == (const DataBaseInfo &lhs, const DataBaseInfo &rhs)
    {
        return lhs.name_ == rhs.name_ && lhs.size_ == rhs.size_;
    }

    template<typename T>
    struct FiledByIdex
    {
        common::Value* valueByIndex(unsigned char index) const;
    };

    struct ServerInfo
    {
        struct Server
                : FiledByIdex<Server>
        {
            Server();
            explicit Server(const common::unicode_string& server_text);
            common::Value* valueByIndex(unsigned char index) const;

            common::unicode_string redis_version_;
            common::unicode_string redis_git_sha1_;
            common::unicode_string redis_git_dirty_;
            common::unicode_string redis_mode_;
            common::unicode_string os_;
            common::uint32_type arch_bits_;
            common::unicode_string multiplexing_api_;
            common::unicode_string gcc_version_;
            common::uint32_type process_id_;
            common::unicode_string run_id_;
            common::uint32_type tcp_port_;
            common::uint32_type uptime_in_seconds_;
            common::uint32_type uptime_in_days_;
            common::uint32_type lru_clock_;
        } server_;

        struct Clients
                : FiledByIdex<Clients>
        {
            Clients();
            explicit Clients(const common::unicode_string& client_text);
            common::Value* valueByIndex(unsigned char index) const;

            common::uint32_type connected_clients_;
            common::uint32_type client_longest_output_list_;
            common::uint32_type client_biggest_input_buf_;
            common::uint32_type blocked_clients_;
        } clients_;

        struct Memory
                : FiledByIdex<Server>
        {
            Memory();
            explicit Memory(const common::unicode_string& memory_text);
            common::Value* valueByIndex(unsigned char index) const;

            common::uint32_type used_memory_;
            common::unicode_string used_memory_human_;
            common::uint32_type used_memory_rss_;
            common::uint32_type used_memory_peak_;
            common::unicode_string used_memory_peak_human_;
            common::uint32_type used_memory_lua_;
            float mem_fragmentation_ratio_;
            common::unicode_string mem_allocator_;
        } memory_;

        struct Persistence
                : FiledByIdex<Server>
        {
            Persistence();
            explicit Persistence(const common::unicode_string& persistence_text);
            common::Value* valueByIndex(unsigned char index) const;

            common::uint32_type loading_;
            common::uint32_type rdb_changes_since_last_save_;
            common::uint32_type rdb_bgsave_in_progress_;
            common::uint32_type rdb_last_save_time_;
            common::unicode_string rdb_last_bgsave_status_;
            int rdb_last_bgsave_time_sec_;
            int rdb_current_bgsave_time_sec_;
            common::uint32_type aof_enabled_;
            common::uint32_type aof_rewrite_in_progress_;
            common::uint32_type aof_rewrite_scheduled_;
            int aof_last_rewrite_time_sec_;
            int aof_current_rewrite_time_sec_;
            common::unicode_string aof_last_bgrewrite_status_;
        } persistence_;

        struct Stats
                : FiledByIdex<Stats>
        {
            Stats();
            explicit Stats(const common::unicode_string& stats_text);
            common::Value* valueByIndex(unsigned char index) const;

            common::uint32_type total_connections_received_;
            common::uint32_type total_commands_processed_;
            common::uint32_type instantaneous_ops_per_sec_;
            common::uint32_type rejected_connections_;
            common::uint32_type expired_keys_;
            common::uint32_type evicted_keys_;
            common::uint32_type keyspace_hits_;
            common::uint32_type keyspace_misses_;
            common::uint32_type pubsub_channels_;
            common::uint32_type pubsub_patterns_;
            common::uint32_type latest_fork_usec_;
        } stats_;

        struct Replication
                : FiledByIdex<Server>
        {
            Replication();
            explicit Replication(const common::unicode_string& replication_text);
            common::Value* valueByIndex(unsigned char index) const;

            common::unicode_string role_;
            common::uint32_type connected_slaves_;
        } replication_;

        struct Cpu
                : FiledByIdex<Cpu>
        {
            Cpu();
            explicit Cpu(const common::unicode_string& cpu_text);
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

        common::Value* valueByIndexes(unsigned char property, unsigned char field) const;

        ServerInfo();
        ServerInfo(const Server &serv, const Clients &clients, const Memory &memory,
                   const Persistence &pers, const Stats &stats, const Replication &repl, const Cpu &cpu, const Keyspace &key);
    };

    std::ostream& operator<<(std::ostream& out, const ServerInfo& value);

    typedef std::pair<common::unicode_string, common::unicode_string> PropertyType;

    struct ServerPropertyInfo
    {
        ServerPropertyInfo();
        std::vector<PropertyType> propertyes_;
    };

    ServerInfo makeServerInfo(const common::unicode_string& content);
    ServerInfo makeServerInfo(const FastoObjectPtr& root);
    ServerPropertyInfo makeServerProperty(const FastoObjectPtr& root);
}
