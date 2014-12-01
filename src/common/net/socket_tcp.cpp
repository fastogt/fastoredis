#include "common/net/socket_tcp.h"

#ifdef OS_POSIX
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#else
#include <winsock2.h>
#include <wspiapi.h>
#endif

#include <errno.h>
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

            std::string host = host_.first;
            uint16_t port = host_.second;

            if(host.empty()){
                return false;
            }

            if(port <= 0){
                return false;
            }

            struct addrinfo hints, *result, *rp;
            char _port[6];
            snprintf(_port, 6, "%d", port);
            memset(&hints, 0, sizeof(hints));
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
            hints.ai_protocol = 0;          /* Any protocol */
            hints.ai_canonname = NULL;
            hints.ai_addr = NULL;
            hints.ai_next = NULL;

            /* Try with IPv6 if no IPv4 address was found. We do it in this order since
             * in a client you can't afford to test if you have IPv6 connectivity
             * as this would add latency to every connect. Otherwise a more sensible
             * route could be: Use IPv6 if both addresses are available and there is IPv6
             * connectivity. */

            int rv;
            if ((rv = getaddrinfo(host.c_str(), _port, &hints, &result)) != 0) {
                 hints.ai_family = AF_INET6;
                 if ((rv = getaddrinfo(host.c_str(), _port, &hints, &result)) != 0) {
                    return false;
                }
            }

            int sfd;
            for (rp = result; rp != NULL; rp = rp->ai_next) {
               sfd = socket(rp->ai_family, rp->ai_socktype,
                            rp->ai_protocol);
               if (sfd == -1)
                   continue;

               if (::connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
                   break;                  /* Success */

               ::close(sfd);
            }

            if (rp == NULL) {               /* No address succeeded */
               return false;
            }

            freeaddrinfo(result);           /* No longer needed */
            fd_ = sfd;
            return true;
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
            if(fd_ == INVALID_DESCRIPTOR){
                return false;
            }

            if(data.empty()){
                return false;
            }

            #ifdef OS_WIN
                ssize_t nwritten = send(fd_, (const char*)data.c_str(), data.size(),0);
            #else
                ssize_t nwritten = ::write(fd_, data.c_str(), data.size());
            #endif
            return nwritten != ERROR_RESULT_VALUE;
        }

        bool SocketTcp::read(buffer_type& outData, uint16_t maxSize)
        {
            if(fd_ == INVALID_DESCRIPTOR){
                return false;
            }

            byte_type* data = (byte_type*)calloc(maxSize, sizeof(byte_type));
            if(!data){
                return false;
            }

            #ifdef OS_WIN
                ssize_t nread = recv(fd_, (char*)data, maxSize, 0);
            #else
                ssize_t nread = ::read(fd_, data, maxSize);
            #endif

            if(nread > 0){
                outData = buffer_type(data, nread);
            }

            free(data);

            return nread > 0;
        }
    }
}


