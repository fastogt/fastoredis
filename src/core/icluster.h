#pragma once

#include "core/core_fwd.h"

#include "core/events/events.h"

namespace fastoredis
{
    class ICluster
    {
    public:
        typedef std::vector<IServerSPtr> nodes_type;

        explicit ICluster(const std::string& name, const nodes_type& nodes = nodes_type());

        std::string name() const;

        IServerSPtr root() const;
        nodes_type nodes() const;
        void addServer(IServerSPtr serv);

    private:
        std::string name_;
        nodes_type nodes_;
    };
}
