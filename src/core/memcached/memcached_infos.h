#pragma once

#include "core/types.h"

namespace fastoredis
{
    class MemcachedServerInfo
            : public ServerInfo
    {
    public:
        MemcachedServerInfo();
        virtual common::Value* valueByIndexes(unsigned char property, unsigned char field) const;
    };

    std::ostream& operator << (std::ostream& out, const MemcachedServerInfo& value);

    ServerInfoSPtr makeMemcachedServerInfo(const std::string &content);
    ServerInfoSPtr makeMemcachedServerInfo(FastoObject *root);
}
