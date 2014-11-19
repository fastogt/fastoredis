#pragma once

#include "common/net/net.h"

namespace common
{
    namespace net
    {
        class SocketTcp
        {
        public:
            SocketTcp(const std::string& host, uint16_t port);
            explicit SocketTcp(const hostAndPort& host);
            bool connect() WARN_UNUSED_RESULT;
            void close();

            bool write(const buffer_type& data) WARN_UNUSED_RESULT;
            bool read(buffer_type &outData, uint16_t max_size) WARN_UNUSED_RESULT;

        private:
            const hostAndPort host_;
            int fd_;
        };
    }
}
