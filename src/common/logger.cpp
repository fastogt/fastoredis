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
        unicode_ostream& logger::get_cout()
        {
        #ifdef UNICODE
           return std::wcout;
        #else
           return std::cout;
        #endif
        }
        unicode_istream& logger::get_cin()
        {
        #ifdef UNICODE
           return std::wcin;
        #else
           return std::cin;
        #endif
        }
        unicode_ostream &logger::get_cerr()
        {
        #ifdef UNICODE
           return std::wcerr;
        #else
           return std::cerr;
        #endif
        }
        unicode_char logger::get_endl()
        {
           return UTEXT('\n');
        }
        logger::logger()
            : m_outStream(NULL)
        {
        #ifdef NDEBUG
            m_outStream = new unicode_ofstream(get_logger_path().c_str());
            unicode_ofstream *file = dynamic_cast<unicode_ofstream*>(m_outStream);
            if(!file||(file&&!file->is_open()))
            {
                 m_outStream  = &get_cerr();
                 *m_outStream << traits_level<WARNING>::text << UTEXT(" ") << "Output file not open!" <<  UTEXT("\n");
            }
            *m_outStream << UTEXT("LOG STARTED\n");
        #else
            m_outStream = &get_cerr();
        #endif
        }
        logger::~logger(void)
        {
            m_outStream->flush();
            if(m_outStream&&m_outStream!=&get_cerr()){
                unicode_ofstream *file = dynamic_cast<unicode_ofstream*>(m_outStream);
                if(file){
                    file->close();
                }
               delete m_outStream;
            }
        }
    }
}
