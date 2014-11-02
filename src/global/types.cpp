#include "global/types.h"

#include "common/convert2string.h"

namespace fastoredis
{
    Command::Command()
        : message_(), type_(InnerCommand)
    {

    }

    Command::Command(const std::string &mess, c_type t)
        : message_(common::convertToString16(mess)), type_(t)
    {

    }

    Command::Command(const common::string16 &mess, c_type t)
        : message_(mess), type_(t)
    {

    }

    const common::string16 &Command::message() const
    {
        return message_;
    }

    const Command::c_type Command::type() const
    {
        return type_;
    }
}
