#include "common/utils.h"
#ifdef OS_WIN
#include <windows.h>
#include <iphlpapi.h>
#elif defined OS_POSIX
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <net/if.h>
#endif

#ifdef OS_WIN
namespace common
{
    namespace utils
    {
        namespace net
        {
            namespace details
            {
                std::string get_mac_address_host_impl(IPAddr destination, IPAddr source, std::string &out_mac_address)
                {
                    std::string result;
                    ULONG MacAddr[2];       /* for 6-byte hardware addresses */
                    ULONG PhysAddrLen = 6;  /* default to length of six bytes */

                    DWORD dwRetVal = SendARP(destination, source, reinterpret_cast<PULONG>(&MacAddr), &PhysAddrLen);

                    if (dwRetVal == NO_ERROR) {
                        BYTE *bPhysAddr = (BYTE *)&MacAddr;
                        for (int i = 0; i < PhysAddrLen; i++) {
                            char tmp[4] = {0};
                            if (i == (PhysAddrLen - 1))
                                sprintf(tmp, "%.2X", bPhysAddr[i]);
                            else
                                sprintf(tmp, "%.2X-", bPhysAddr[i]);

                            out_mac_address += tmp;
                        }
                    }
                    else if(dwRetVal == ERROR_BAD_NET_NAME){

                    }
                    else if(dwRetVal == ERROR_BUFFER_OVERFLOW){

                    }
                    else if(dwRetVal == ERROR_GEN_FAILURE){

                    }
                    else if(dwRetVal == ERROR_INVALID_PARAMETER){

                    }
                    else if(dwRetVal == ERROR_INVALID_USER_BUFFER){

                    }
                    else if(dwRetVal == ERROR_NOT_FOUND){

                    }
                    else if(dwRetVal == ERROR_NOT_SUPPORTED){

                    }

                    return true;
                }
            }

            bool get_mac_address_host(const std::string &host, std::string &out_mac_address)
            {
                hostent * record = gethostbyname(host.c_str());
                if(record == NULL)
                {
                    return false;
                }
                in_addr * address = (in_addr * )record->h_addr;
                const char* ip_address = inet_ntoa(* address);
                IPAddr DestIp = inet_addr(ip_address);
                return details::get_mac_address_host_impl(DestIp, INADDR_ANY, out_mac_address);
            }
        }
    }
}
#elif defined OS_POSIX
namespace common
{
    namespace utils
    {
        namespace signal
        {
            bool signal(int sig, void (*handler)(int))
            {
                bool result = ::signal(sig, handler)!= SIG_ERR;
                if(!result)
                {
                    unicode_perror("signal");
                }
                return result;
            }
            bool sigaddset(sigset_t &signal_mask,int sig)
            {
                bool result = ::sigaddset(&signal_mask,sig)!= ERROR_RESULT_VALUE;
                if(!result)
                {
                    unicode_perror("sigaddset");
                }
                return result;
            }
            bool sigprocmask(int how, const sigset_t *set,sigset_t *oset)
            {
                bool result = ::sigprocmask(how,set,oset)!= ERROR_RESULT_VALUE;
                if(!result)
                {
                    unicode_perror("sigprocmask");
                }
                return result;
            }
            bool sigemptyset(sigset_t &signal_mask)
            {
                bool result = ::sigemptyset(&signal_mask)!= ERROR_RESULT_VALUE;
                if(!result)
                {
                    unicode_perror("sigemptyset");
                }
                return result;
            }
        }
        namespace net
        {
            int print_pkt(char *buf, int len)
            {
                int j = 0;
                for(j = 0; j < len; j++ )
                {
                if((j%16) == 0 && j != 0 )
                    printf("\n");
                    printf("%02x ",*(buf+j)& 0xFF );
                }
                printf("\n");
                return 0;
            }

            bool get_mac_address_host(const std::string &host, std::string &out_mac_address)
            {
                // Socket to send ARP packet
                int list_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP));

                if( list_sock == ERROR_RESULT_VALUE)
                {
                    perror("socket");
                    return false;
                }
                struct sockaddr_in udp_sin;
                bzero (&udp_sin, sizeof(udp_sin));
                udp_sin.sin_family = AF_INET;
                udp_sin.sin_addr.s_addr = inet_addr(out_mac_address.c_str());
                udp_sin.sin_port = htons(5232);

                int i = sendto(list_sock, "TEST", 5, 0, (struct sockaddr *)&udp_sin, sizeof(udp_sin));
                if(i == ERROR_RESULT_VALUE){
                    perror("sendto");
                }
                return true;
            }
        }
    }
}
#endif
