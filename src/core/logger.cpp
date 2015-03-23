#include "core/logger.h"

#include <QMetaType>

#include "common/logger.h"
#include "common/qt/convert_string.h"

#define MAX_DEBUG_MSG_SIZE 1024

namespace fastoredis
{
    Logger::Logger()
    {
        qRegisterMetaType<common::logging::LEVEL_LOG>("common::logging::LEVEL_LOG");
    }

    void Logger::print(const char* mess, common::logging::LEVEL_LOG level, bool notify)
    {
        using namespace common;
        DEBUG_MSG_FORMAT<MAX_DEBUG_MSG_SIZE>(level, "%s", mess);
        if (notify){
            const QString& qmess = convertFromString<QString>(mess);
            emit printed(qmess, level);
        }
    }

    void Logger::print(const QString& mess, common::logging::LEVEL_LOG level, bool notify)
    {
        using namespace common;
        const std::string& smess = convertToString(mess);
        DEBUG_MSG_FORMAT<MAX_DEBUG_MSG_SIZE>(level, "%s", smess);
        if (notify){
            emit printed(mess, level);
        }
    }

    void Logger::print(const std::string& mess, common::logging::LEVEL_LOG level, bool notify)
    {
        using namespace common;
        DEBUG_MSG_FORMAT<MAX_DEBUG_MSG_SIZE>(level, "%s", mess);
        if (notify){
            const QString& qmess = convertFromString<QString>(mess);
            emit printed(qmess, level);
        }
    }
}
