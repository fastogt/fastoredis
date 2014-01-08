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

    std::string IDatabase::name() const
    {
        return info_.name_;
    }

    IDatabase::~IDatabase()
    {

    }
}
