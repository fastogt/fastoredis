#pragma once

#include <string>

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

        ServerInfo();
        ServerInfo(const Server &serv);
    };
}
