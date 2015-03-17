#include "core/core_fwd.h"

#include "core/iserver.h"

namespace fastoredis
{
    Cluster::Cluster(const std::string &name, const nodes_type& nodes)
        : name_(name), nodes_(nodes)
    {

    }

    IServerSPtr Cluster::root() const
    {
        for(int i = 0; i < nodes_.size(); ++i){
            IServerSPtr ser = nodes_[i];
            if(ser && ser->isRoot()){
                return ser;
            }
        }

        return IServerSPtr();
    }

    std::string Cluster::name() const
    {
        return name_;
    }

    Cluster::nodes_type Cluster::nodes() const
    {
        return nodes_;
    }

    void Cluster::addServer(IServerSPtr serv)
    {
        if(serv){
            nodes_.push_back(serv);
        }
    }
}
