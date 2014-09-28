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
        Command(const unicode_string &mess, c_type t = InnerCommand);
        const unicode_string message() const;
        const c_type type() const;

    private:
        const unicode_string message_;
        c_type type_;
    };
}
