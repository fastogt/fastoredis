#include "common/net/net.h"

namespace common
{
    std::string convertToString(const net::hostAndPort& host)
    {
        static const uint16_t size_buff = 512;
        char buff[size_buff] = {0};
        sprintf(buff, "%s:%d", host.first.c_str(), host.second);
        return buff;
    }

    template<>
    net::hostAndPort convertFromString(const std::string& host)
    {
        net::hostAndPort res;
        size_t del = host.find_first_of(':');
        if(del != std::string::npos){
            res.first = host.substr(0, del);
            res.second = convertFromString<uint16_t>(host.substr(del + 1));
        }

        return res;
    }
}
