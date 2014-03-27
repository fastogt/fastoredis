#pragma once

#include <string>
#include <vector>

namespace fastoredis
{
    struct DataBaseInfo
    {
        DataBaseInfo(const std::string &name, size_t size);
        std::string name_;
        size_t size_;
    };

    inline bool operator ==(const DataBaseInfo &lhs, const DataBaseInfo &rhs)
    {
        return lhs.name_ == rhs.name_ && lhs.size_ == rhs.size_;
    }

    struct ServerInfo
    {
        struct Server
        {
            Server();
            explicit Server(const std::string& server_text);

            std::string redis_version_;
            std::string redis_git_sha1_;
            std::string redis_git_dirty_;
            std::string redis_mode_;
            std::string os_;
            unsigned arch_bits_;
            std::string multiplexing_api_;
            std::string gcc_version_;
            unsigned process_id_;
            std::string run_id_;
            unsigned tcp_port_;
            unsigned uptime_in_seconds_;
            unsigned uptime_in_days_;
            unsigned lru_clock_;
        } server_;

        struct Clients
        {
            Clients();
            explicit Clients(const std::string& client_text);

            unsigned connected_clients_;
            unsigned client_longest_output_list_;
            unsigned client_biggest_input_buf_;
            unsigned blocked_clients_;
        } clients_;

        struct Memory
        {
            Memory();
            explicit Memory(const std::string& memory_text);

            unsigned used_memory_;
            std::string used_memory_human_;
            unsigned used_memory_rss_;
            unsigned used_memory_peak_;
            std::string used_memory_peak_human_;
            unsigned used_memory_lua_;
            float mem_fragmentation_ratio_;
            std::string mem_allocator_;
        } memory_;

        struct Persistence
        {
            Persistence();
            explicit Persistence(const std::string& persistence_text);

            unsigned loading_;
            unsigned rdb_changes_since_last_save_;
            unsigned rdb_bgsave_in_progress_;
            unsigned rdb_last_save_time_;
            std::string rdb_last_bgsave_status_;
            int rdb_last_bgsave_time_sec_;
            int rdb_current_bgsave_time_sec_;
            unsigned aof_enabled_;
            unsigned aof_rewrite_in_progress_;
            unsigned aof_rewrite_scheduled_;
            int aof_last_rewrite_time_sec_;
            int aof_current_rewrite_time_sec_;
            std::string aof_last_bgrewrite_status_;
        } persistence_;

        struct Stats
        {
            Stats();
            explicit Stats(const std::string& stats_text);

            unsigned total_connections_received_;
            unsigned total_commands_processed_;
            unsigned instantaneous_ops_per_sec_;
            unsigned rejected_connections_;
            unsigned expired_keys_;
            unsigned evicted_keys_;
            unsigned keyspace_hits_;
            unsigned keyspace_misses_;
            unsigned pubsub_channels_;
            unsigned pubsub_patterns_;
            unsigned latest_fork_usec_;
        } stats_;

        struct Replication
        {
            Replication();
            explicit Replication(const std::string& replication_text);

            std::string role_;
            unsigned connected_slaves_;
        } replication_;

        struct Cpu
        {
            Cpu();
            explicit Cpu(const std::string& cpu_text);

            float used_cpu_sys_;
            float used_cpu_user_;
            float used_cpu_sys_children_;
            float used_cpu_user_children_;
        } cpu_;

        ServerInfo();
        ServerInfo(const Server &serv, const Clients &clients, const Memory &memory, const Persistence &pers, const Stats &stats, const Replication &repl, const Cpu &cpu);
    };

    typedef std::pair<std::string, std::string> PropertyType;

    struct ServerPropertyInfo
    {
        ServerPropertyInfo();
        std::vector<PropertyType> propertyes_;
    };
}
