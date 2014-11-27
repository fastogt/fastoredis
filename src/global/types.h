#pragma once

/**/

#include "common/types.h"

namespace fastoredis
{
    class Command
    {
    public:
        enum CommandType
        {
            UserCommand,
            InnerCommand
        };
        Command();
        Command(const std::string &mess, CommandType t = InnerCommand);
        const std::string& message() const;
        CommandType type() const;

    private:
        const std::string message_;
        const CommandType type_;
    };
}
