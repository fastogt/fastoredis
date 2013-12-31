#pragma once

#include "core/IDataBase.h"

namespace fastoredis
{
    class RedisDatabase
            : public IDatabase
    {
    public:
        RedisDatabase(const IServerPtr &server, const QString &name);
    };
}
