#include "core/Logger.h"

#include <QMetaType>
#include <QFile>
#include "common/logger.h"
#include "common/qt/converter_patterns.h"
#include "common/file_system.h"

namespace
{
    std::string getLoggerPath()
    {
        static std::string path = common::file_system::prepare_path("~/"PROJECT_NAME_LOWERCASE".log");
        return path;
    }
    const uint maxFileSize = 5 * 1024 * 1024;
}

namespace fastoredis
{
    Logger::Logger()
    {
        qRegisterMetaType<common::logging::LEVEL_LOG>("common::logging::LEVEL_LOG");
        std::string path = getLoggerPath();
        QFile file(common::utils_qt::toQString(path));
        if (file.exists() && file.size() > maxFileSize) {
            file.remove(); //delete file if it size more than 5mb
        }
    }

    Logger::~Logger()
    {   
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
