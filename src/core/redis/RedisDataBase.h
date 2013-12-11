#pragma once

#include "core/IDataBase.h"

namespace fastoredis
{
    class RedisDatabase
            : public IDatabase
    {
    public:
        typedef IDatabase base_class;
        RedisDatabase(IServer *server, const QString &name);
    };
}
