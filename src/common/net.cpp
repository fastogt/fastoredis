#include "common/net.h"

#include "common/macros.h"
#include <boost/lexical_cast.hpp>

namespace common
{
    namespace net
    {
        std::string toStdString(const hostAndPort &host)
        {
            unicode_char buff[512] = {0};
            const char* h = host.first.c_str();
            if(h){
                sprintf(buff,"%s:%u", h, host.second);
            }
            return buff;
        }

        hostAndPort toHostAndPort(const std::string &host)
        {
            hostAndPort res;
            size_t del = host.find_first_of(':');
            if(del != std::string::npos){
                res.first = host.substr(0, del);
                res.second = boost::lexical_cast<unsigned>(host.substr(del + 1));
            }
            return res;
        }
    }
}
