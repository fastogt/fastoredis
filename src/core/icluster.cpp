#include "core/icluster.h"

#include "core/iserver.h"

namespace fastoredis
{
    ICluster::ICluster(const std::string &name, const nodes_type& nodes)
        : name_(name), nodes_(nodes)
    {

    }

    IServerSPtr ICluster::root() const
    {
        for(int i = 0; i < nodes_.size(); ++i){
            IServerSPtr ser = nodes_[i];
            if(ser && ser->isRoot()){
                return ser;
            }
        }

        return IServerSPtr();
    }

    std::string ICluster::name() const
    {
        return name_;
    }

    ICluster::nodes_type ICluster::nodes() const
    {
        return nodes_;
    }

    void ICluster::addServer(IServerSPtr serv)
    {
        if(serv){
            nodes_.push_back(serv);
        }
    }
}
