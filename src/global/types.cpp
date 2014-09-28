#include "global/types.h"

namespace fastoredis
{
    Command::Command()
        : message_(), type_(InnerCommand)
    {

    }

    Command::Command(const unicode_string &mess, c_type t)
        : message_(mess), type_(t)
    {

    }

    const unicode_string Command::message() const
    {
        return message_;
    }

    const Command::c_type Command::type() const
    {
        return type_;
    }
}
