#pragma once

#include <boost/shared_ptr.hpp>

#include "core/Infos.h"

namespace fastoredis
{
    class IServer;

    class IDatabase
    {
    public:
        std::string name() const;
        IServer *const server() const;

    protected:
        IDatabase(IServer *server, const DataBaseInfo &info);
        virtual ~IDatabase();

    private:
        IServer *const server_;
        DataBaseInfo info_;
    };

    typedef boost::shared_ptr<IDatabase> IDatabasePtr;
}
