#include "core/Infos.h"

namespace fastoredis
{
    DataBaseInfo::DataBaseInfo(const std::string &name, size_t size)
        : name_(name), size_(size)
    {

    }

    ServerInfo::ServerInfo()
    {

    }

    ServerInfo::ServerInfo(const Server &serv) : server_(serv)
    {

    }

    ServerPropertyInfo::ServerPropertyInfo()
    {

    }
}
