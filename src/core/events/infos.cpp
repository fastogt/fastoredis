#include "core/events/infos.h"

#include <ostream>

#include "common/convert2string.h"

namespace fastoredis
{
    DataBaseInfo::DataBaseInfo(const common::unicode_string &name, size_t size)
        : name_(name), size_(size)
    {

    }

    ServerInfo::Server::Server::Server()
        : redis_version_(), redis_git_sha1_(), redis_git_dirty_(), redis_mode_(), os_(),
        arch_bits_(0), multiplexing_api_(), gcc_version_() ,process_id_(0),
        run_id_(), tcp_port_(0), uptime_in_seconds_(0), uptime_in_days_(0), lru_clock_(0)
    {

    }

    ServerInfo::Server::Server(const common::unicode_string& server_text)
        : redis_version_(), redis_git_sha1_(), redis_git_dirty_(), redis_mode_(), os_(),
        arch_bits_(0), multiplexing_api_(), gcc_version_() ,process_id_(0),
        run_id_(), tcp_port_(0), uptime_in_seconds_(0), uptime_in_days_(0), lru_clock_(0)
    {
        const common::unicode_string &src = server_text;
        size_t pos = 0;
        size_t start = 0;
        while((pos = src.find(UTEXT("\r\n"), start)) != common::unicode_string::npos){
            common::unicode_string line = src.substr(start, pos-start);
            size_t delem = line.find_first_of(':');
            common::unicode_string field = line.substr(0, delem);
            common::unicode_string value = line.substr(delem + 1);
            if(field == UTEXT("redis_version")){
                redis_version_ = value;
            }
            else if(field == UTEXT("redis_git_sha1")){
                redis_git_sha1_ = value;
            }
            else if(field == UTEXT("redis_git_dirty")){
                redis_git_dirty_ = value;
            }
            else if(field == UTEXT("redis_mode")){
                redis_mode_ = value;
            }
            else if(field == UTEXT("os")){
                os_ = value;
            }
            else if(field == UTEXT("arch_bits")){
                arch_bits_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("multiplexing_api")){
                multiplexing_api_ = value;
            }
            else if(field == UTEXT("gcc_version")){
                gcc_version_ = value;
            }
            else if(field == UTEXT("process_id")){
                process_id_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("run_id")){
                run_id_ = value;
            }
            else if(field == UTEXT("tcp_port")){
                tcp_port_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("uptime_in_seconds")){
                uptime_in_seconds_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("uptime_in_days")){
                uptime_in_days_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("lru_clock")){
                lru_clock_ = common::convertfromString<common::uint32_type>(value);
            }
            start = pos + 2;
        }
    }

    ServerInfo::Clients::Clients()
        : connected_clients_(0), client_longest_output_list_(0),
        client_biggest_input_buf_(0), blocked_clients_(0)
    {
    }

    ServerInfo::Clients::Clients(const common::unicode_string& client_text)
        : connected_clients_(0), client_longest_output_list_(0),
        client_biggest_input_buf_(0), blocked_clients_(0)
    {
        const common::unicode_string &src = client_text;
        size_t pos = 0;
        size_t start = 0;
        while((pos = src.find(UTEXT("\r\n"), start)) != common::unicode_string::npos){
            common::unicode_string line = src.substr(start, pos-start);
            size_t delem = line.find_first_of(':');
            common::unicode_string field = line.substr(0, delem);
            common::unicode_string value = line.substr(delem + 1);
            if(field == UTEXT("connected_clients")){
                connected_clients_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("client_longest_output_list")){
                client_longest_output_list_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("client_biggest_input_buf")){
                client_biggest_input_buf_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("blocked_clients")){
                blocked_clients_ = common::convertfromString<common::uint32_type>(value);
            }
            start = pos + 2;
        }
    }

    ServerInfo::Memory::Memory()
        : used_memory_(0), used_memory_human_(), used_memory_rss_(0), used_memory_peak_(0),
          used_memory_peak_human_(), used_memory_lua_(0),mem_fragmentation_ratio_(0), mem_allocator_()
    {


    }

    ServerInfo::Memory::Memory(const common::unicode_string& memory_text)
        : used_memory_(0), used_memory_human_(), used_memory_rss_(0), used_memory_peak_(0),
          used_memory_peak_human_(), used_memory_lua_(0),mem_fragmentation_ratio_(0), mem_allocator_()
    {
        const common::unicode_string &src = memory_text;
        size_t pos = 0;
        size_t start = 0;
        while((pos = src.find(UTEXT("\r\n"), start)) != common::unicode_string::npos){
            common::unicode_string line = src.substr(start, pos-start);
            size_t delem = line.find_first_of(':');
            common::unicode_string field = line.substr(0, delem);
            common::unicode_string value = line.substr(delem + 1);
            if(field == UTEXT("used_memory")){
                used_memory_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("used_memory_human")){
                used_memory_human_ = value;
            }
            else if(field == UTEXT("used_memory_rss")){
                used_memory_rss_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("used_memory_peak")){
                used_memory_peak_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("used_memory_peak_human")){
                used_memory_peak_human_ = value;
            }
            else if(field == UTEXT("used_memory_lua")){
                used_memory_lua_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("mem_fragmentation_ratio")){
                mem_fragmentation_ratio_ = common::convertfromString<float>(value);
            }
            else if(field == UTEXT("mem_allocator")){
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

    ServerInfo::Persistence::Persistence(const common::unicode_string& persistence_text)
        : loading_(0), rdb_changes_since_last_save_(0), rdb_bgsave_in_progress_(0),
          rdb_last_save_time_(0), rdb_last_bgsave_status_(),
          rdb_last_bgsave_time_sec_(0), rdb_current_bgsave_time_sec_(0),
          aof_enabled_(0), aof_rewrite_in_progress_(0), aof_rewrite_scheduled_(0),
          aof_last_rewrite_time_sec_(0), aof_current_rewrite_time_sec_(0), aof_last_bgrewrite_status_()
    {
        const common::unicode_string &src = persistence_text;
        size_t pos = 0;
        size_t start = 0;
        while((pos = src.find(UTEXT("\r\n"), start)) != common::unicode_string::npos){
            common::unicode_string line = src.substr(start, pos-start);
            size_t delem = line.find_first_of(':');
            common::unicode_string field = line.substr(0, delem);
            common::unicode_string value = line.substr(delem + 1);
            if(field == UTEXT("loading")){
                loading_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("rdb_changes_since_last_save")){
                rdb_changes_since_last_save_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("rdb_bgsave_in_progress")){
                rdb_bgsave_in_progress_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("rdb_last_save_time")){
                rdb_last_save_time_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("rdb_last_bgsave_status")){
                rdb_last_bgsave_status_ = value;
            }
            else if(field == UTEXT("rdb_last_bgsave_time_sec")){
                rdb_last_bgsave_time_sec_ = common::convertfromString<int>(value);
            }
            else if(field == UTEXT("rdb_current_bgsave_time_sec")){
                rdb_current_bgsave_time_sec_ = common::convertfromString<int>(value);
            }
            else if(field == UTEXT("aof_enabled")){
                aof_enabled_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("aof_rewrite_in_progress")){
                aof_rewrite_in_progress_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("aof_rewrite_scheduled")){
                aof_rewrite_scheduled_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("aof_last_rewrite_time_sec")){
                aof_last_rewrite_time_sec_ = common::convertfromString<int>(value);
            }
            else if(field == UTEXT("aof_current_rewrite_time_sec")){
                aof_current_rewrite_time_sec_ = common::convertfromString<int>(value);
            }
            else if(field == UTEXT("aof_last_bgrewrite_status")){
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

    ServerInfo::Stats::Stats(const common::unicode_string& stats_text)
        : total_connections_received_(0), total_commands_processed_(0),
          instantaneous_ops_per_sec_(0), rejected_connections_(0),
          expired_keys_(0), evicted_keys_(0), keyspace_hits_(0),
          keyspace_misses_(0), pubsub_channels_(0),
          pubsub_patterns_(0), latest_fork_usec_(0)
    {
        const common::unicode_string &src = stats_text;
        size_t pos = 0;
        size_t start = 0;
        while((pos = src.find(UTEXT("\r\n"), start)) != common::unicode_string::npos){
            common::unicode_string line = src.substr(start, pos-start);
            size_t delem = line.find_first_of(':');
            common::unicode_string field = line.substr(0, delem);
            common::unicode_string value = line.substr(delem + 1);
            if(field == UTEXT("total_connections_received")){
                total_connections_received_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("total_commands_processed")){
                total_commands_processed_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("instantaneous_ops_per_sec")){
                instantaneous_ops_per_sec_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("rejected_connections")){
                rejected_connections_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("expired_keys")){
                expired_keys_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("evicted_keys")){
                evicted_keys_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("keyspace_hits")){
                keyspace_hits_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("keyspace_misses")){
                keyspace_misses_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("pubsub_channels")){
                pubsub_channels_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("pubsub_patterns")){
                pubsub_patterns_ = common::convertfromString<common::uint32_type>(value);
            }
            else if(field == UTEXT("latest_fork_usec")){
                latest_fork_usec_ = common::convertfromString<common::uint32_type>(value);
            }
            start = pos + 2;
        }
    }

    ServerInfo::Replication::Replication()
        : role_(), connected_slaves_(0)
    {

    }

    ServerInfo::Replication::Replication(const common::unicode_string& replication_text)
        : role_(), connected_slaves_(0)
    {
        const common::unicode_string &src = replication_text;
        size_t pos = 0;
        size_t start = 0;

        while((pos = src.find(UTEXT("\r\n"), start)) != common::unicode_string::npos){
            common::unicode_string line = src.substr(start, pos-start);
            size_t delem = line.find_first_of(':');
            common::unicode_string field = line.substr(0, delem);
            common::unicode_string value = line.substr(delem + 1);
            if(field == UTEXT("role")){
                role_ = value;
            }
            else if(field == UTEXT("connected_slaves")){
                connected_slaves_ = common::convertfromString<common::uint32_type>(value);
            }
            start = pos + 2;
        }
    }

    ServerInfo::Cpu::Cpu()
        : used_cpu_sys_(0), used_cpu_user_(0), used_cpu_sys_children_(0), used_cpu_user_children_(0)
    {

    }

    ServerInfo::Cpu::Cpu(const common::unicode_string& cpu_text)
        : used_cpu_sys_(0), used_cpu_user_(0), used_cpu_sys_children_(0), used_cpu_user_children_(0)
    {
        const common::unicode_string &src = cpu_text;
        size_t pos = 0;
        size_t start = 0;
        while((pos = src.find(UTEXT("\r\n"), start)) != common::unicode_string::npos){
            common::unicode_string line = src.substr(start, pos-start);
            size_t delem = line.find_first_of(':');
            common::unicode_string field = line.substr(0, delem);
            common::unicode_string value = line.substr(delem + 1);
            if(field == UTEXT("used_cpu_sys")){
                used_cpu_sys_ = common::convertfromString<float>(value);
            }
            else if(field == UTEXT("used_cpu_user")){
                used_cpu_user_ = common::convertfromString<float>(value);
            }
            else if(field == UTEXT("used_cpu_sys_children")){
                used_cpu_sys_children_ = common::convertfromString<float>(value);
            }
            else if(field == UTEXT("used_cpu_user_children")){
                used_cpu_user_children_ = common::convertfromString<float>(value);
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

    std::ostream& operator<<(std::ostream& out, const ServerInfo::Server& value)
    {
        return out << UTEXT("redis_version:") << value.redis_version_ << UTEXT("\r\n")
                   << UTEXT("redis_git_sha1:") << value.redis_git_sha1_ << UTEXT("\r\n")
                   << UTEXT("redis_git_dirty:") << value.redis_git_dirty_ << UTEXT("\r\n")
                   << UTEXT("redis_mode:") << value.redis_mode_ << UTEXT("\r\n")
                   << UTEXT("os:") << value.os_ << UTEXT("\r\n")
                   << UTEXT("arch_bits:") << value.arch_bits_ << UTEXT("\r\n")
                   << UTEXT("multiplexing_api:") << value.multiplexing_api_ << UTEXT("\r\n")
                   << UTEXT("gcc_version:") << value.gcc_version_ << UTEXT("\r\n")
                   << UTEXT("process_id:") << value.process_id_ << UTEXT("\r\n")
                   << UTEXT("run_id:") << value.run_id_ << UTEXT("\r\n")
                   << UTEXT("tcp_port:") << value.tcp_port_ << UTEXT("\r\n")
                   << UTEXT("uptime_in_seconds:") << value.uptime_in_seconds_ << UTEXT("\r\n")
                   << UTEXT("uptime_in_days:") << value.uptime_in_days_ << UTEXT("\r\n")
                   << UTEXT("lru_clock:") << value.lru_clock_ << UTEXT("\r\n");
    }

    std::ostream& operator<<(std::ostream& out, const ServerInfo::Clients& value)
    {
        return out << UTEXT("connected_clients:") << value.connected_clients_ << UTEXT("\r\n")
                   << UTEXT("client_longest_output_list:") << value.client_longest_output_list_ << UTEXT("\r\n")
                   << UTEXT("client_biggest_input_buf:") << value.client_biggest_input_buf_ << UTEXT("\r\n")
                   << UTEXT("blocked_clients:") << value.blocked_clients_ << UTEXT("\r\n");
    }

    std::ostream& operator<<(std::ostream& out, const ServerInfo::Memory& value)
    {
        return out << UTEXT("used_memory:") << value.used_memory_ << UTEXT("\r\n")
                   << UTEXT("used_memory_human:") << value.used_memory_human_ << UTEXT("\r\n")
                   << UTEXT("used_memory_rss:") << value.used_memory_rss_ << UTEXT("\r\n")
                   << UTEXT("used_memory_peak:") << value.used_memory_peak_ << UTEXT("\r\n")
                   << UTEXT("used_memory_peak_human:") << value.used_memory_peak_human_ << UTEXT("\r\n")
                   << UTEXT("used_memory_lua:") << value.used_memory_lua_ << UTEXT("\r\n")
                   << UTEXT("mem_fragmentation_ratio:") << value.mem_fragmentation_ratio_ << UTEXT("\r\n")
                   << UTEXT("mem_allocator:") << value.mem_allocator_ << UTEXT("\r\n");
    }

    std::ostream& operator<<(std::ostream& out, const ServerInfo::Persistence& value)
    {
        return out << UTEXT("loading:") << value.loading_ << UTEXT("\r\n")
                   << UTEXT("rdb_changes_since_last_save:") << value.rdb_changes_since_last_save_ << UTEXT("\r\n")
                   << UTEXT("rdb_bgsave_in_progress:") << value.rdb_bgsave_in_progress_ << UTEXT("\r\n")
                   << UTEXT("rdb_last_save_time:") << value.rdb_last_save_time_ << UTEXT("\r\n")
                   << UTEXT("rdb_last_bgsave_status:") << value.rdb_last_bgsave_status_ << UTEXT("\r\n")
                   << UTEXT("rdb_last_bgsave_time_sec:") << value.rdb_last_bgsave_time_sec_ << UTEXT("\r\n")
                   << UTEXT("rdb_current_bgsave_time_sec:") << value.rdb_current_bgsave_time_sec_ << UTEXT("\r\n")
                   << UTEXT("aof_enabled:") << value.aof_enabled_ << UTEXT("\r\n")
                   << UTEXT("aof_rewrite_in_progress:") << value.aof_rewrite_in_progress_ << UTEXT("\r\n")
                   << UTEXT("aof_rewrite_scheduled:") << value.aof_rewrite_scheduled_ << UTEXT("\r\n")
                   << UTEXT("aof_last_rewrite_time_sec:") << value.aof_last_rewrite_time_sec_ << UTEXT("\r\n")
                   << UTEXT("aof_current_rewrite_time_sec:") << value.aof_current_rewrite_time_sec_ << UTEXT("\r\n")
                   << UTEXT("aof_last_bgrewrite_status:") << value.aof_last_bgrewrite_status_ << UTEXT("\r\n");
    }

    std::ostream& operator<<(std::ostream& out, const ServerInfo::Stats& value)
    {
        return out << UTEXT("total_connections_received:") << value.total_connections_received_ << UTEXT("\r\n")
                   << UTEXT("total_commands_processed:") << value.total_commands_processed_ << UTEXT("\r\n")
                   << UTEXT("instantaneous_ops_per_sec:") << value.instantaneous_ops_per_sec_ << UTEXT("\r\n")
                   << UTEXT("rejected_connections:") << value.rejected_connections_ << UTEXT("\r\n")
                   << UTEXT("expired_keys:") << value.expired_keys_ << UTEXT("\r\n")
                   << UTEXT("evicted_keys:") << value.evicted_keys_ << UTEXT("\r\n")
                   << UTEXT("keyspace_hits:") << value.keyspace_hits_ << UTEXT("\r\n")
                   << UTEXT("keyspace_misses:") << value.keyspace_misses_ << UTEXT("\r\n")
                   << UTEXT("pubsub_channels:") << value.pubsub_channels_ << UTEXT("\r\n")
                   << UTEXT("pubsub_patterns:") << value.pubsub_patterns_ << UTEXT("\r\n")
                   << UTEXT("latest_fork_usec:") << value.latest_fork_usec_ << UTEXT("\r\n");
    }

    std::ostream& operator<<(std::ostream& out, const ServerInfo::Replication& value)
    {
        return out << UTEXT("role:") << value.role_ << UTEXT("\r\n")
                   << UTEXT("connected_slaves:") << value.connected_slaves_ << UTEXT("\r\n");
    }

    std::ostream& operator<<(std::ostream& out, const ServerInfo::Cpu& value)
    {
        return out << UTEXT("used_cpu_sys:") << value.used_cpu_sys_ << UTEXT("\r\n")
                   << UTEXT("used_cpu_user:") << value.used_cpu_user_ << UTEXT("\r\n")
                   << UTEXT("used_cpu_sys_children:") << value.used_cpu_sys_children_ << UTEXT("\r\n")
                   << UTEXT("used_cpu_user_children:") << value.used_cpu_user_children_ << UTEXT("\r\n");
    }

    std::ostream& operator<<(std::ostream& out, const ServerInfo& value)
    {
        //"# Server", "# Clients", "# Memory", "# Persistence", "# Stats", "# Replication", "# CPU", "# Keyspace"
        return out << UTEXT("# Server\r\n") << value.server_ << UTEXT("# Clients\r\n") << value.clients_ << UTEXT("# Memory\r\n") << value.memory_
                   << UTEXT("# Persistence\r\n") << value.persistence_ << UTEXT("# Stats\r\n") << value.stats_
                   << UTEXT("# Replication\r\n") << value.replication_ << UTEXT("# CPU\r\n") << value.cpu_;
    }

    ServerPropertyInfo::ServerPropertyInfo()
    {

    }
}
