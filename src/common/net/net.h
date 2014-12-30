#pragma once

#include "common/macros.h"
#include "common/convert2string.h"

namespace common
{
    namespace net
    {
        struct hostAndPort
        {
            std::string host_;
            uint16_t port_;

            hostAndPort();
            hostAndPort(const std::string& host, uint16_t port);
            bool isValid() const;
        };

        int connect(const hostAndPort& from) WARN_UNUSED_RESULT;
        ssize_t write_to_socket(int fd, const buffer_type& data) WARN_UNUSED_RESULT;
        ssize_t read_from_socket(int fd, buffer_type& outData, uint16_t maxSize) WARN_UNUSED_RESULT;
        int sendFile(const std::string& path, const hostAndPort& to) WARN_UNUSED_RESULT;
    }

    std::string convertToString(const net::hostAndPort& from);
}
