#include "core/IDataBase.h"

namespace fastoredis
{
    IDatabase::IDatabase(const IServerPtr &server, const QString &name)
        : server_(server), name_(name)
    {

    }

    IServerPtr IDatabase::server() const
    {
        return server_;
    }

    QString IDatabase::name() const
    {
        return name_;
    }

    IDatabase::~IDatabase()
    {

    }
}
