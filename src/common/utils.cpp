#include "common/utils.h"
#ifdef OS_WIN
#include <windows.h>
#include <iphlpapi.h>
#endif
#ifdef OS_POSIX
#include <signal.h>
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
    }
}
#endif
namespace common
{
    namespace utils
    {
        namespace net
        {
            namespace details
            {
                std::string get_mac_address_host_impl(IPAddr destination, IPAddr source)
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

                            result += tmp;
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

                    return result;
                }
            }
            std::string get_mac_address_host(const std::string &host)
            {
                hostent * record = gethostbyname(host.c_str());
                std::string result;
                if(record == NULL)
                {
                    return result;
                }
                in_addr * address = (in_addr * )record->h_addr;
                const char* ip_address = inet_ntoa(* address);
                IPAddr DestIp = inet_addr(ip_address);
                result = details::get_mac_address_host_impl(DestIp, INADDR_ANY);
                return result;
            }
        }
    }
}
