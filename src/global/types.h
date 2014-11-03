#pragma once

/**/

#include "common/types.h"

namespace fastoredis
{
    class Command
    {
    public:
        enum c_type
        {
            UserCommand,
            InnerCommand
        };
        Command();
        Command(const std::string &mess, c_type t = InnerCommand);
        Command(const common::string16 &mess, c_type t = InnerCommand);
        const common::string16& message() const;
        const c_type type() const;

    private:
        const common::string16 message_;
        c_type type_;
    };
}
