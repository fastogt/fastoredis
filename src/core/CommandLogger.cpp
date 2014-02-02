#include "core/CommandLogger.h"

#include "common/qt/convert_string.h"

namespace fastoredis
{
    CommandLogger::CommandLogger()
    {
        qRegisterMetaType<Command>("fastoredis::Command");
    }

    void CommandLogger::print(const Command &command)
    {
        emit printed(command);
    }
}
