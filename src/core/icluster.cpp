#include "core/icluster.h"

#include "common/qt/convert_string.h"

#include "core/iserver.h"

namespace fastoredis
{
    ICluster::ICluster(IServerSPtr root, const std::string &name)
        : root_(root), name_(name)
    {

    }

    QString ICluster::name() const
    {
        return common::convertFromString<QString>(name_);
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

    IServerSPtr ICluster::root() const
    {
        return root_;
    }
}
