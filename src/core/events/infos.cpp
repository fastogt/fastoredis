#include "core/events/infos.h"

#include <boost/lexical_cast.hpp>

namespace fastoredis
{
    DataBaseInfo::DataBaseInfo(const std::string &name, size_t size)
        : name_(name), size_(size)
    {

    }

    ServerInfo::Server::Server::Server()
        : redis_version_(), redis_git_sha1_(), redis_git_dirty_(), redis_mode_(), os_(),
        arch_bits_(0), multiplexing_api_(), gcc_version_() ,process_id_(0),
        run_id_(), tcp_port_(0), uptime_in_seconds_(0), uptime_in_days_(0), lru_clock_(0)
    {

    }

    ServerInfo::Server::Server(const std::string& server_text)
        : redis_version_(), redis_git_sha1_(), redis_git_dirty_(), redis_mode_(), os_(),
        arch_bits_(0), multiplexing_api_(), gcc_version_() ,process_id_(0),
        run_id_(), tcp_port_(0), uptime_in_seconds_(0), uptime_in_days_(0), lru_clock_(0)
    {
        const std::string &src = server_text;
        size_t pos = 0;
        size_t start = 0;
        while((pos = src.find("\r\n", start)) != std::string::npos){
            std::string line = src.substr(start, pos-start);
            size_t delem = line.find_first_of(':');
            std::string field = line.substr(0, delem);
            std::string value = line.substr(delem + 1);
            if(field == "redis_version"){
                redis_version_ = value;
            }
            else if(field == "redis_git_sha1"){
                redis_git_sha1_ = value;
            }
            else if(field == "redis_git_dirty"){
                redis_git_dirty_ = value;
            }
            else if(field == "redis_mode"){
                redis_mode_ = value;
            }
            else if(field == "os"){
                os_ = value;
            }
            else if(field == "arch_bits"){
                arch_bits_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "multiplexing_api"){
                multiplexing_api_ = value;
            }
            else if(field == "gcc_version"){
                gcc_version_ = value;
            }
            else if(field == "process_id"){
                process_id_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "run_id"){
                run_id_ = value;
            }
            else if(field == "tcp_port"){
                tcp_port_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "uptime_in_seconds"){
                uptime_in_seconds_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "uptime_in_days"){
                uptime_in_days_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "lru_clock"){
                lru_clock_ = boost::lexical_cast<unsigned>(value);
            }
            start = pos + 2;
        }
    }

    ServerInfo::Clients::Clients()
        : connected_clients_(0), client_longest_output_list_(0),
        client_biggest_input_buf_(0), blocked_clients_(0)
    {
    }

    ServerInfo::Clients::Clients(const std::string& client_text)
        : connected_clients_(0), client_longest_output_list_(0),
        client_biggest_input_buf_(0), blocked_clients_(0)
    {
        const std::string &src = client_text;
        size_t pos = 0;
        size_t start = 0;
        while((pos = src.find("\r\n", start)) != std::string::npos){
            std::string line = src.substr(start, pos-start);
            size_t delem = line.find_first_of(':');
            std::string field = line.substr(0, delem);
            std::string value = line.substr(delem + 1);
            if(field == "connected_clients"){
                connected_clients_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "client_longest_output_list"){
                client_longest_output_list_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "client_biggest_input_buf"){
                client_biggest_input_buf_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "blocked_clients"){
                blocked_clients_ = boost::lexical_cast<unsigned>(value);
            }
            start = pos + 2;
        }
    }

    ServerInfo::Memory::Memory()
        : used_memory_(0), used_memory_human_(), used_memory_rss_(0), used_memory_peak_(0),
          used_memory_peak_human_(), used_memory_lua_(0),mem_fragmentation_ratio_(0), mem_allocator_()
    {


    }

    ServerInfo::Memory::Memory(const std::string& memory_text)
        : used_memory_(0), used_memory_human_(), used_memory_rss_(0), used_memory_peak_(0),
          used_memory_peak_human_(), used_memory_lua_(0),mem_fragmentation_ratio_(0), mem_allocator_()
    {
        const std::string &src = memory_text;
        size_t pos = 0;
        size_t start = 0;
        while((pos = src.find("\r\n", start)) != std::string::npos){
            std::string line = src.substr(start, pos-start);
            size_t delem = line.find_first_of(':');
            std::string field = line.substr(0, delem);
            std::string value = line.substr(delem + 1);
            if(field == "used_memory"){
                used_memory_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "used_memory_human"){
                used_memory_human_ = value;
            }
            else if(field == "used_memory_rss"){
                used_memory_rss_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "used_memory_peak"){
                used_memory_peak_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "used_memory_peak_human"){
                used_memory_peak_human_ = value;
            }
            else if(field == "used_memory_lua"){
                used_memory_lua_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "mem_fragmentation_ratio"){
                mem_fragmentation_ratio_ = boost::lexical_cast<float>(value);
            }
            else if(field == "mem_allocator"){
                mem_allocator_ = value;
            }
            start = pos + 2;
        }
    }

    ServerInfo::Persistence::Persistence()
        : loading_(0), rdb_changes_since_last_save_(0), rdb_bgsave_in_progress_(0),
          rdb_last_save_time_(0), rdb_last_bgsave_status_(),
          rdb_last_bgsave_time_sec_(0), rdb_current_bgsave_time_sec_(0),
          aof_enabled_(0), aof_rewrite_in_progress_(0), aof_rewrite_scheduled_(0),
          aof_last_rewrite_time_sec_(0), aof_current_rewrite_time_sec_(0), aof_last_bgrewrite_status_()
    {

    }

    ServerInfo::Persistence::Persistence(const std::string& persistence_text)
        : loading_(0), rdb_changes_since_last_save_(0), rdb_bgsave_in_progress_(0),
          rdb_last_save_time_(0), rdb_last_bgsave_status_(),
          rdb_last_bgsave_time_sec_(0), rdb_current_bgsave_time_sec_(0),
          aof_enabled_(0), aof_rewrite_in_progress_(0), aof_rewrite_scheduled_(0),
          aof_last_rewrite_time_sec_(0), aof_current_rewrite_time_sec_(0), aof_last_bgrewrite_status_()
    {
        const std::string &src = persistence_text;
        size_t pos = 0;
        size_t start = 0;
        while((pos = src.find("\r\n", start)) != std::string::npos){
            std::string line = src.substr(start, pos-start);
            size_t delem = line.find_first_of(':');
            std::string field = line.substr(0, delem);
            std::string value = line.substr(delem + 1);
            if(field == "loading"){
                loading_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "rdb_changes_since_last_save"){
                rdb_changes_since_last_save_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "rdb_bgsave_in_progress"){
                rdb_bgsave_in_progress_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "rdb_last_save_time"){
                rdb_last_save_time_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "rdb_last_bgsave_status"){
                rdb_last_bgsave_status_ = value;
            }
            else if(field == "rdb_last_bgsave_time_sec"){
                rdb_last_bgsave_time_sec_ = boost::lexical_cast<int>(value);
            }
            else if(field == "rdb_current_bgsave_time_sec"){
                rdb_current_bgsave_time_sec_ = boost::lexical_cast<int>(value);
            }
            else if(field == "aof_enabled"){
                aof_enabled_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "aof_rewrite_in_progress"){
                aof_rewrite_in_progress_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "aof_rewrite_scheduled"){
                aof_rewrite_scheduled_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "aof_last_rewrite_time_sec"){
                aof_last_rewrite_time_sec_ = boost::lexical_cast<int>(value);
            }
            else if(field == "aof_current_rewrite_time_sec"){
                aof_current_rewrite_time_sec_ = boost::lexical_cast<int>(value);
            }
            else if(field == "aof_last_bgrewrite_status"){
                aof_last_bgrewrite_status_ = value;
            }
            start = pos + 2;
        }
    }

    ServerInfo::Stats::Stats()
        : total_connections_received_(0), total_commands_processed_(0),
          instantaneous_ops_per_sec_(0), rejected_connections_(0),
          expired_keys_(0), evicted_keys_(0), keyspace_hits_(0),
          keyspace_misses_(0), pubsub_channels_(0),
          pubsub_patterns_(0), latest_fork_usec_(0)
    {

    }

    ServerInfo::Stats::Stats(const std::string& stats_text)
        : total_connections_received_(0), total_commands_processed_(0),
          instantaneous_ops_per_sec_(0), rejected_connections_(0),
          expired_keys_(0), evicted_keys_(0), keyspace_hits_(0),
          keyspace_misses_(0), pubsub_channels_(0),
          pubsub_patterns_(0), latest_fork_usec_(0)
    {
        const std::string &src = stats_text;
        size_t pos = 0;
        size_t start = 0;
        while((pos = src.find("\r\n", start)) != std::string::npos){
            std::string line = src.substr(start, pos-start);
            size_t delem = line.find_first_of(':');
            std::string field = line.substr(0, delem);
            std::string value = line.substr(delem + 1);
            if(field == "total_connections_received"){
                total_connections_received_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "total_commands_processed"){
                total_commands_processed_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "instantaneous_ops_per_sec"){
                instantaneous_ops_per_sec_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "rejected_connections"){
                rejected_connections_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "expired_keys"){
                expired_keys_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "evicted_keys"){
                evicted_keys_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "keyspace_hits"){
                keyspace_hits_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "keyspace_misses"){
                keyspace_misses_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "pubsub_channels"){
                pubsub_channels_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "pubsub_patterns"){
                pubsub_patterns_ = boost::lexical_cast<unsigned>(value);
            }
            else if(field == "latest_fork_usec"){
                latest_fork_usec_ = boost::lexical_cast<unsigned>(value);
            }
            start = pos + 2;
        }
    }

    ServerInfo::Cpu::Cpu()
        : used_cpu_sys_(0), used_cpu_user_(0), used_cpu_sys_children_(0), used_cpu_user_children_(0)
    {

    }

    ServerInfo::Cpu::Cpu(const std::string& cpu_text)
        : used_cpu_sys_(0), used_cpu_user_(0), used_cpu_sys_children_(0), used_cpu_user_children_(0)
    {
        const std::string &src = cpu_text;
        size_t pos = 0;
        size_t start = 0;
        while((pos = src.find("\r\n", start)) != std::string::npos){
            std::string line = src.substr(start, pos-start);
            size_t delem = line.find_first_of(':');
            std::string field = line.substr(0, delem);
            std::string value = line.substr(delem + 1);
            if(field == "used_cpu_sys"){
                used_cpu_sys_ = boost::lexical_cast<float>(value);
            }
            else if(field == "used_cpu_user"){
                used_cpu_user_ = boost::lexical_cast<float>(value);
            }
            else if(field == "used_cpu_sys_children"){
                used_cpu_sys_children_ = boost::lexical_cast<float>(value);
            }
            else if(field == "used_cpu_user_children"){
                used_cpu_user_children_ = boost::lexical_cast<float>(value);
            }
            start = pos + 2;
        }
    }

    ServerInfo::Replication::Replication()
        : role_(), connected_slaves_(0)
    {

    }

    ServerInfo::Replication::Replication(const std::string& replication_text)
        : role_(), connected_slaves_(0)
    {
        const std::string &src = replication_text;
        size_t pos = 0;
        size_t start = 0;

        while((pos = src.find("\r\n", start)) != std::string::npos){
            std::string line = src.substr(start, pos-start);
            size_t delem = line.find_first_of(':');
            std::string field = line.substr(0, delem);
            std::string value = line.substr(delem + 1);
            if(field == "role"){
                role_ = value;
            }
            else if(field == "connected_slaves"){
                connected_slaves_ = boost::lexical_cast<unsigned>(value);
            }
            start = pos + 2;
        }
    }

    ServerInfo::ServerInfo()
    {

    }

    ServerInfo::ServerInfo(const Server &serv, const Clients &clients, const Memory &memory,
                           const Persistence &pers, const Stats &stats, const Replication &repl, const Cpu &cpu)
        : server_(serv), clients_(clients), memory_(memory), persistence_(pers), stats_(stats), replication_(repl), cpu_(cpu)
    {

    }

    ServerPropertyInfo::ServerPropertyInfo()
    {

    }
}
