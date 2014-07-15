#include "common/utils.h"

#ifdef OS_WIN

#elif defined OS_LINUX
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
#include "common/logger.h"
#endif

namespace common
{
    namespace utils
    {
#ifdef OS_LINUX
        namespace signal
        {
            bool signal(int sig, void (*handler)(int))
            {
                bool result = ::signal(sig, handler)!= SIG_ERR;
                if(!result){
                    DEBUG_MSG_PERROR("signal");
                }
                return result;
            }

            bool sigaddset(sigset_t &signal_m, int sig)
            {
                bool result = ::sigaddset(&signal_m,sig)!= ERROR_RESULT_VALUE;
                if(!result){
                    DEBUG_MSG_PERROR("sigaddset");
                }
                return result;
            }

            bool sigprocmask(int how, const sigset_t *set,sigset_t *oset)
            {
                bool result = ::sigprocmask(how,set,oset)!= ERROR_RESULT_VALUE;
                if(!result){
                    DEBUG_MSG_PERROR("sigprocmask");
                }
                return result;
            }

            bool sigemptyset(sigset_t &signal_m)
            {
                bool result = ::sigemptyset(&signal_m)!= ERROR_RESULT_VALUE;
                if(!result){
                    DEBUG_MSG_PERROR("sigemptyset");
                }
                return result;
            }
        }
#endif
    }
}
