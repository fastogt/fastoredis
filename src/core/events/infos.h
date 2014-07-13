#pragma once

/**/

#include <vector>

#include "common/types.h"

namespace fastoredis
{
    struct DataBaseInfo
    {
        DataBaseInfo(const common::unicode_string &name, size_t size);
        common::unicode_string name_;
        size_t size_;
    };

    inline bool operator == (const DataBaseInfo &lhs, const DataBaseInfo &rhs)
    {
        return lhs.name_ == rhs.name_ && lhs.size_ == rhs.size_;
    }

    struct ServerInfo
    {
        struct Server
        {
            Server();
            explicit Server(const common::unicode_string& server_text);

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
        {
            Clients();
            explicit Clients(const common::unicode_string& client_text);

            common::uint32_type connected_clients_;
            common::uint32_type client_longest_output_list_;
            common::uint32_type client_biggest_input_buf_;
            common::uint32_type blocked_clients_;
        } clients_;

        struct Memory
        {
            Memory();
            explicit Memory(const common::unicode_string& memory_text);

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
        {
            Persistence();
            explicit Persistence(const common::unicode_string& persistence_text);

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
        {
            Stats();
            explicit Stats(const common::unicode_string& stats_text);

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
        {
            Replication();
            explicit Replication(const common::unicode_string& replication_text);

            common::unicode_string role_;
            common::uint32_type connected_slaves_;
        } replication_;

        struct Cpu
        {
            Cpu();
            explicit Cpu(const common::unicode_string& cpu_text);

            float used_cpu_sys_;
            float used_cpu_user_;
            float used_cpu_sys_children_;
            float used_cpu_user_children_;
        } cpu_;

        ServerInfo();
        ServerInfo(const Server &serv, const Clients &clients, const Memory &memory, const Persistence &pers, const Stats &stats, const Replication &repl, const Cpu &cpu);
    };

    std::ostream& operator<<(std::ostream& out, const ServerInfo& value);

    typedef std::pair<common::unicode_string, common::unicode_string> PropertyType;

    struct ServerPropertyInfo
    {
        ServerPropertyInfo();
        std::vector<PropertyType> propertyes_;
    };
}
