#include "core/events/Infos.h"

namespace fastoredis
{
    DataBaseInfo::DataBaseInfo(const std::string &name, size_t size)
        : name_(name), size_(size)
    {

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
