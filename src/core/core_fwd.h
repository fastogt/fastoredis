#pragma once

#include "core/types.h"

namespace fastoredis
{
    typedef shared_ptr_t<DataBaseInfo> DataBaseInfoSPtr;

    class IDatabase;
    typedef shared_ptr_t<IDatabase> IDatabaseSPtr;

    class IServer;
    typedef shared_ptr_t<IServer> IServerSPtr;

    class IDriver;
    typedef shared_ptr_t<IDriver> IDriverSPtr;   
}
