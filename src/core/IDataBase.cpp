#include "core/IDataBase.h"

namespace fastoredis
{
    IDatabase::IDatabase(IServer *server, const QString &name)
        : parent_(server), name_(name)
    {

    }
}
