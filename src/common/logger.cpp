#include "common/logger.h"

#include "common/file_system.h"
#include <unistd.h>
#include <fstream>

namespace
{
    using namespace common;
    const std::string &get_logger_path()
    {
        static std::string result = file_system::prepare_path("~/"PROJECT_NAME_LOWERCASE".log");
        return result;
    }

    typedef std::basic_streambuf<char16> string16_streambuf;
}

namespace common
{
    namespace logging
    {
        string16_ostream g_cout(string16_streambuf());
        string16_istream g_cin(string16_streambuf());
        string16_ostream g_cerr(string16_streambuf());

        logger::logger()
            : outStream_(NULL)
        {
        #ifdef NDEBUG
            outStream_ = new string16_ofstream(get_logger_path().c_str());
            string16_ofstream *file = dynamic_cast<string16_ofstream*>(outStream_);
            if(!file||(file&&!file->is_open()))
            {
                 outStream_  = &cerr();
                 *outStream_ << traits_level<WARNING>::text << UTEXT(" ") << "Output file not open!" <<  UTEXT("\n");
            }            
        #else
            outStream_ = &cerr;
        #endif
            *outStream_ << UTEXT("LOG STARTED\n");
        }

        logger::~logger(void)
        {
            outStream_->flush();
            if(outStream_ && outStream_ != &cerr){
                string16_ofstream *file = dynamic_cast<string16_ofstream*>(outStream_);
                if(file){
                    file->close();
                }
               delete outStream_;
            }
        }

        void logger::printTradeSafe(LEVEL_LOG level, const std::string &data)
        {
            multi_threading::unique_lock<locker_type> lock(lock_);
            //*outStream_ << log_level_to_text(level) << UTEXT(" ") << data;
        }
    }

    void DEBUG_MSG_PERROR(const char *function)
    {
        const char* strer = strerror(errno);
        return DEBUG_MSG_FORMAT<256>(logging::L_ERROR, "function: %s, %s\n", function, strer);
    }
}
