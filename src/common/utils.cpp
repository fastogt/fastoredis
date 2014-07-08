#include "common/utils.h"
#ifdef OS_WIN
#include <windows.h>
#include <iphlpapi.h>
#elif defined OS_POSIX
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>

#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#endif

namespace common
{
    namespace utils
    {
        namespace signal
        {
            bool signal(int sig, void (*handler)(int))
            {
                bool result = ::signal(sig, handler)!= SIG_ERR;
                if(!result){
                    unicode_perror("signal");
                }
                return result;
            }

            bool sigaddset(sigset_t &signal_mask,int sig)
            {
                bool result = ::sigaddset(&signal_mask,sig)!= ERROR_RESULT_VALUE;
                if(!result){
                    unicode_perror("sigaddset");
                }
                return result;
            }

            bool sigprocmask(int how, const sigset_t *set,sigset_t *oset)
            {
                bool result = ::sigprocmask(how,set,oset)!= ERROR_RESULT_VALUE;
                if(!result){
                    unicode_perror("sigprocmask");
                }
                return result;
            }

            bool sigemptyset(sigset_t &signal_mask)
            {
                bool result = ::sigemptyset(&signal_mask)!= ERROR_RESULT_VALUE;
                if(!result){
                    unicode_perror("sigemptyset");
                }
                return result;
            }
        }
    }
}
