#include "global/types.h"

namespace fastoredis
{
    Command::Command()
        : message_(), type_(InnerCommand)
    {

    }

    Command::Command(const std::string &mess, CommandType t)
        : message_(mess), type_(t)
    {

    }

    const std::string &Command::message() const
    {
        return message_;
    }

    Command::CommandType Command::type() const
    {
        return type_;
    }
}
