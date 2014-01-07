#pragma once

#include "core/Infos.h"

namespace fastoredis
{
    class IServer;

    class IDatabase
    {
    public:
        DataBaseInfo info() const;
        IServer *const server() const;

    protected:
        IDatabase(IServer *server, const DataBaseInfo &info);
        virtual ~IDatabase();

    private:
        IServer *const server_;
        DataBaseInfo info_;
    };
}
