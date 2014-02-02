#include "core/Logger.h"

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
        logging::logger::instance().print(mess,level);
        if (notify)
            emit printed(common::utils_qt::toQString(mess), level);
    }

    void Logger::print(const QString &mess, common::logging::LEVEL_LOG level, bool notify)
    {
        print(common::utils_qt::toStdString(mess), level, notify);
    }
}
