#include "core/logger.h"

#include <QMetaType>

#include "common/logger.h"
#include "common/qt/convert_string.h"

namespace fastoredis
{
    Logger::Logger()
    {
        qRegisterMetaType<common::logging::LEVEL_LOG>("common::logging::LEVEL_LOG");
    }

    void Logger::print(const char *mess, common::logging::LEVEL_LOG level, bool notify)
    {
        print(std::string(mess), level, notify);
    }

    void Logger::print(const std::string &mess, common::logging::LEVEL_LOG level, bool notify)
    {
        using namespace common;
        DEBUG_MSG_FORMAT<1024>(level, "%s", mess);
        if (notify){
            emit printed(convertFromString16<QString>(mess), level);
        }
    }

    void Logger::print(const common::string16 &mess, common::logging::LEVEL_LOG level, bool notify)
    {
        print(common::convertToString(mess), level, notify);
    }

    void Logger::print(const QString &mess, common::logging::LEVEL_LOG level, bool notify)
    {
        print(common::convertToString16(mess), level, notify);
    }
}
