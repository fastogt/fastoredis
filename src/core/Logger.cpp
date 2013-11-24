#include "core/Logger.h"

#include <QDir>
#include <QMetaType>
#include "common/logger.h"
#include "common/qt_helper/converter_patterns.h"

namespace
{
    std::string getLoggerPath()
    {
        static std::string path = common::utils_qt::toStdString(QString("%1/"PROJECT_NAME_LOWERCASE".log").arg(QDir::homePath()));
        return path;
    }
}

namespace fastoredis
{
    Logger::Logger()
    {
        qRegisterMetaType<common::logging::LEVEL_LOG>("common::logging::LEVEL_LOG");
        std::string path = getLoggerPath();
        QFile file(common::utils_qt::toQString(path)); //delete file if it size more than 5mb
        if (file.exists() && file.size() > 5 * 1024 * 1024) {
            file.remove();
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
        print(common::utils_qt::toQString(mess), level, notify);
    }

    void Logger::print(const QString &mess, common::logging::LEVEL_LOG level, bool notify)
    {
        using namespace common;
        logging::logger::instance().print(utils_qt::toStdString(mess),level);
        if (notify)
            emit printed(mess, level);
    }
}
