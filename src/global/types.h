#pragma once

#include <string>

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
        const std::string message() const;
        const c_type type() const;
    private:
        std::string message_;
        c_type type_;
    };
}
