#include "core/IDataBase.h"

namespace fastoredis
{
    IDatabase::IDatabase(IServer *server, const DataBaseInfo &info)
        : server_(server), info_(info)
    {

    }

    IServer * const IDatabase::server() const
    {
        return server_;
    }

    DataBaseInfo IDatabase::info() const
    {
        return info_;
    }

    IDatabase::~IDatabase()
    {

    }
}
