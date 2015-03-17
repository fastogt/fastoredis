#pragma once

#include "core/types.h"

namespace fastoredis
{
    typedef common::shared_ptr<DataBaseInfo> DataBaseInfoSPtr;

    class IDatabase;
    typedef common::shared_ptr<IDatabase> IDatabaseSPtr;

    class IServer;
    typedef common::shared_ptr<IServer> IServerSPtr;

    class IDriver;
    typedef common::shared_ptr<IDriver> IDriverSPtr;

    class Cluster
    {
    public:
        typedef std::vector<IServerSPtr> nodes_type;

        explicit Cluster(const std::string& name, const nodes_type& nodes = nodes_type());

        std::string name() const;

        IServerSPtr root() const;
        nodes_type nodes() const;
        void addServer(IServerSPtr serv);

    private:
        std::string name_;
        nodes_type nodes_;
    };
}
