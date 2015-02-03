#include "global/types.h"

namespace fastoredis
{
    Command::Command()
        : message_(), type_(common::Value::C_INNER)
    {

    }

    Command::Command(const std::string &mess, common::Value::CommandType commandT)
        : message_(mess), type_(commandT)
    {

    }

    const std::string &Command::message() const
    {
        return message_;
    }

    common::Value::CommandType Command::type() const
    {
        return type_;
    }
}
