#include "common/net/net.h"

#ifdef OS_POSIX
#include <netdb.h>
#else
#include <winsock2.h>
#include <wspiapi.h>
#endif

#ifdef OS_MACOSX
#include <sys/uio.h>
#endif

#include <fcntl.h>
#include <sys/stat.h>

#include "common/logger.h"

#ifdef OS_WINDOWS

#define BUF_SIZE 8192

namespace
{
    ssize_t sendfile(int out_fd, int in_fd, off_t *offset, size_t count)
    {
        off_t orig;
        char buf[BUF_SIZE];
        size_t numRead, numSent, totSent;

        if (offset != NULL) {

            /* Save current file offset and set offset to value in '*offset' */

            orig = lseek(in_fd, 0, SEEK_CUR);
            if (orig == -1)
                return ERROR_RESULT_VALUE;
            if (lseek(in_fd, *offset, SEEK_SET) == -1)
                return ERROR_RESULT_VALUE;
        }

        totSent = 0;

        while (count > 0) {
            numRead = read(in_fd, buf, BUF_SIZE);
            if (numRead == -1)
                return ERROR_RESULT_VALUE;
            if (numRead == 0)
                break;                      /* EOF */
#ifdef OS_WIN
            numSent = send(out_fd, buf, numRead, 0);
#else
            numSent = write(out_fd, buf, numRead);
#endif
            if (numSent == -1)
                return ERROR_RESULT_VALUE;

            if (numSent == 0){
                return ERROR_RESULT_VALUE;
            }

            count -= numSent;
            totSent += numSent;
        }

        if (offset != NULL) {

            /* Return updated file offset in '*offset', and reset the file offset
               to the value it had when we were called. */

            *offset = lseek(in_fd, 0, SEEK_CUR);
            if (*offset == -1)
                return ERROR_RESULT_VALUE;
            if (lseek(in_fd, orig, SEEK_SET) == -1)
                return ERROR_RESULT_VALUE;
        }

        return totSent;
    }
}
#endif

namespace common
{
    namespace net
    {
        hostAndPort::hostAndPort()
            : host_(), port_(0)
        {

        }

        hostAndPort::hostAndPort(const std::string& host, uint16_t port)
            : host_(host), port_(port)
        {

        }

        bool hostAndPort::isValid() const
        {
            return !host_.empty() && port_ != 0;
        }

        int connect(const net::hostAndPort& from)
        {
            if(!from.isValid()){
                return INVALID_DESCRIPTOR;
            }

            std::string host = from.host_;
            uint16_t port = from.port_;

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
                    return INVALID_DESCRIPTOR;
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
               return INVALID_DESCRIPTOR;
            }

            freeaddrinfo(result);           /* No longer needed */

            return sfd;
        }

        ssize_t write_to_socket(int fd, const buffer_type& data)
        {
            if(fd == INVALID_DESCRIPTOR){
                return ERROR_RESULT_VALUE;
            }

            if(data.empty()){
                return ERROR_RESULT_VALUE;
            }

            #ifdef OS_WIN
                ssize_t nwritten = send(fd, (const char*)data.c_str(), data.size(), 0);
            #else
                ssize_t nwritten = ::write(fd, data.c_str(), data.size());
            #endif
            return nwritten;
        }

        ssize_t read_from_socket(int fd, buffer_type& outData, uint16_t maxSize)
        {
            if(fd == INVALID_DESCRIPTOR){
                return ERROR_RESULT_VALUE;
            }

            byte_type* data = (byte_type*)calloc(maxSize, sizeof(byte_type));
            if(!data){
                return ERROR_RESULT_VALUE;
            }

            #ifdef OS_WIN
                ssize_t nread = recv(fd, (char*)data, maxSize, 0);
            #else
                ssize_t nread = ::read(fd, data, maxSize);
            #endif

            if(nread > 0){
                outData = buffer_type(data, nread);
            }

            free(data);

            return nread;
        }

        int sendFile(const std::string& path, const net::hostAndPort& to)
        {
            if(path.empty()){
                return ERROR_RESULT_VALUE;
            }

            int sock = connect(to);
            if(sock == INVALID_DESCRIPTOR){
                return ERROR_RESULT_VALUE;
            }

            int fd = open(path.c_str(), O_RDONLY);
            if (fd == INVALID_DESCRIPTOR) {
                DEBUG_MSG_PERROR("open", errno);
                return ERROR_RESULT_VALUE;
            }

            struct stat stat_buf;
            fstat(fd, &stat_buf);

            off_t offset = 0;
#ifdef OS_MACOSX
            int res = sendfile(sock, fd, offset, &stat_buf.st_size, NULL, 0);
#else
            int res = sendfile(sock, fd, &offset, stat_buf.st_size);
#endif
            if(res == ERROR_RESULT_VALUE){
                DEBUG_MSG_PERROR("sendfile", errno);
            }

            close(sock);
            return res;
        }
    }

    std::string convertToString(const net::hostAndPort& host)
    {
        static const uint16_t size_buff = 512;
        char buff[size_buff] = {0};
        sprintf(buff, "%s:%d", host.host_.c_str(), host.port_);
        return buff;
    }

    template<>
    net::hostAndPort convertFromString(const std::string& host)
    {
        net::hostAndPort res;
        size_t del = host.find_first_of(':');
        if(del != std::string::npos){
            res.host_ = host.substr(0, del);
            res.port_ = convertFromString<uint16_t>(host.substr(del + 1));
        }

        return res;
    }
}
