#pragma once

#include "core/core_fwd.h"

#include "core/iserver.h"

namespace fastoredis
{
    class ICluster
            : public IServerBase
    {
    public:
        typedef std::vector<IServerSPtr> nodes_type;

        QString name() const;
        nodes_type nodes() const;
        void addServer(IServerSPtr serv);

        IServerSPtr root() const;

    protected:
        ICluster(IServerSPtr root, const std::string& name);

    private:
        const std::string name_;
        const IServerSPtr root_;
        nodes_type nodes_;
    };
}
