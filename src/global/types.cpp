#include "global/types.h"

namespace fastoredis
{
    Command::Command()
        : message_(), type_(InnerCommand)
    {

    }

    Command::Command(const std::string &mess, c_type t)
        : message_(mess), type_(t)
    {

    }

    const std::string &Command::message() const
    {
        return message_;
    }

    const Command::c_type Command::type() const
    {
        return type_;
    }
}
