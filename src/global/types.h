#pragma once

#include "common/value.h"

namespace fastoredis
{
    class Command
    {
    public:
        Command();
        Command(const std::string &mess, common::Value::CommandType commandT);
        const std::string& message() const;
        common::Value::CommandType type() const;

    private:
        const std::string message_;
        const common::Value::CommandType type_;
    };
}
