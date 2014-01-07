#pragma once

#include "core/IDataBase.h"

namespace fastoredis
{
    class RedisDatabase
            : public IDatabase
    {
    public:
        RedisDatabase(IServer *server, const DataBaseInfo &info);
    };
}
