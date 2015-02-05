#include "common/logger.h"

#include <inttypes.h>

#include "common/file_system.h"
#include "common/multi_threading/types.h"
#include "common/time.h"

namespace
{
    FILE* logger_fd = stderr;
    common::multi_threading::mutex_t logger_lock;
    std::once_flag logger_startup_once;

    static void logger_startup_function(void)
    {
    #ifdef NDEBUG
        std::string result = common::file_system::prepare_path("~/" PROJECT_NAME_LOWERCASE ".log");
        FILE* logger_fd_file = fopen(result.c_str(), "ab+");
        if(logger_fd_file){
            logger_fd = logger_fd_file;
        }
    #endif
        fprintf(logger_fd, PROJECT_NAME_TITLE " log started at: %" PRId64 "\n", common::time::current_mstime());
        fflush(logger_fd);
    }
}

namespace common
{
    namespace logging
    {
        typedef void LogHandler(LEVEL_LOG level, const char* message);

        void DefaultLogHandler(LEVEL_LOG level, const char* message)
        {
            // We use fprintf() instead of cerr because we want this to work at static
            // initialization time.
            fprintf(logger_fd, PROJECT_NAME_TITLE " [%s] %s\n", log_level_to_text(level), message);
            fflush(logger_fd);  // Needed on MSVC.
        }

        void NullLogHandler(LEVEL_LOG level, const std::string& message)
        {
          // Nothing.
        }

        static LogHandler* log_handler_ = &DefaultLogHandler;

        void printTradeSafe(LEVEL_LOG level, const char *data)
        {
            std::call_once(logger_startup_once, logger_startup_function);

            multi_threading::unique_lock<multi_threading::mutex_t> lock(logger_lock);
            log_handler_(level, data);
        }
    }

    void DEBUG_MSG_PERROR(const char *function, int err)
    {
        const char* strer = strerror(err);
        return DEBUG_MSG_FORMAT<256>(logging::L_ERROR, "function: %s, %s", function, strer);
    }
}
