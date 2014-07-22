#include "common/logger.h"

#include "common/file_system.h"
#include <unistd.h>
#include <fstream>

namespace
{
    using namespace common;
    const unicode_string &get_logger_path()
    {
        static unicode_string result = file_system::prepare_path(UTEXT("~/"PROJECT_NAME_LOWERCASE".log"));
        return result;
    }
}

namespace common
{
    namespace logging
    {
        unicode_ostream& logger::cout()
        {
        #ifdef UNICODE
           return std::wcout;
        #else
           return std::cout;
        #endif
        }

        unicode_istream& logger::cin()
        {
        #ifdef UNICODE
           return std::wcin;
        #else
           return std::cin;
        #endif
        }

        unicode_ostream &logger::cerr()
        {
        #ifdef UNICODE
           return std::wcerr;
        #else
           return std::cerr;
        #endif
        }

        unicode_char logger::endl()
        {
           return UTEXT('\n');
        }

        logger::logger()
            : outStream_(NULL)
        {
        #ifdef NDEBUG
            outStream_ = new unicode_ofstream(get_logger_path().c_str());
            unicode_ofstream *file = dynamic_cast<unicode_ofstream*>(outStream_);
            if(!file||(file&&!file->is_open()))
            {
                 outStream_  = &cerr();
                 *outStream_ << traits_level<WARNING>::text << UTEXT(" ") << "Output file not open!" <<  UTEXT("\n");
            }            
        #else
            outStream_ = &cerr();
        #endif
            *outStream_ << UTEXT("LOG STARTED\n");
        }

        logger::~logger(void)
        {
            outStream_->flush();
            if(outStream_ && outStream_ != &cerr()){
                unicode_ofstream *file = dynamic_cast<unicode_ofstream*>(outStream_);
                if(file){
                    file->close();
                }
               delete outStream_;
            }
        }

        void logger::printTradeSafe(LEVEL_LOG level, const unicode_string& data)
        {
            multi_threading::guard_wraper<locker_type> lock(lock_);
            *outStream_ << log_level_to_text(level) << UTEXT(" ") << data;
        }        
    }

    void DEBUG_MSG_PERROR(const unicode_char* function)
    {
        const char* strer = strerror(errno);
        return DEBUG_MSG_FORMAT<256>(logging::L_ERROR, UTEXT("function: %s, %s\n"), function, strer);
    }
}
