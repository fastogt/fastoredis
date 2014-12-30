#include "common/net/socket_tcp.h"

#include "common/net/net.h"

#include <unistd.h>

namespace common
{
    namespace net
    {
        SocketTcp::SocketTcp(const std::string& host, uint16_t port)
            : host_(host, port), fd_(INVALID_DESCRIPTOR)
        {

        }

        SocketTcp::SocketTcp(const hostAndPort& host)
            : host_(host), fd_(INVALID_DESCRIPTOR)
        {

        }

        bool SocketTcp::connect()
        {
            if(fd_ != INVALID_DESCRIPTOR){
                return true;
            }

            int sfd = net::connect(host_);

            fd_ = sfd;
            return fd_ != INVALID_DESCRIPTOR;
        }

        void SocketTcp::close()
        {
            if(fd_ == INVALID_DESCRIPTOR){
                return;
            }

            ::close(fd_);
            fd_ = INVALID_DESCRIPTOR;
        }

        bool SocketTcp::write(const buffer_type& data)
        {
            ssize_t nwritten = net::write_to_socket(fd_, data);
            return nwritten != ERROR_RESULT_VALUE;
        }

        bool SocketTcp::read(buffer_type& outData, uint16_t maxSize)
        {
            ssize_t nread = net::read_from_socket(fd_, outData, maxSize);
            return nread > 0;
        }
    }
}


