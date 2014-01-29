#include "core/CommandLogger.h"

#include "common/qt/converter_patterns.h"

namespace fastoredis
{
    CommandLogger::CommandLogger()
    {
    }

    void CommandLogger::print(const char *mess)
    {
        print(std::string(mess));
    }

    void CommandLogger::print(const std::string &mess)
    {
        emit printed(common::utils_qt::toQString(mess));
    }

    void CommandLogger::print(const QString &mess)
    {
        print(common::utils_qt::toStdString(mess));
    }
}
