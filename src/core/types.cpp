#include "core/types.h"

namespace fastoredis
{
    ServerInfo::~ServerInfo()
    {

    }

    connectionTypes ServerInfo::type() const
    {
        return type_;
    }

    ServerInfo::ServerInfo(connectionTypes type)
        : type_(type)
    {

    }

    ServerPropertyInfo::ServerPropertyInfo()
    {

    }

    ServerPropertyInfo makeServerProperty(const FastoObjectPtr &root)
    {
        ServerPropertyInfo inf;
        FastoObject::child_container_type childrens = root->childrens();
        for(int i = 0; i < childrens.size(); i+=2){
            const std::string c1 = childrens[i]->toString();
            const std::string c2 = childrens[i+1]->toString();
            inf.propertyes_.push_back(std::make_pair(c1, c2));
        }
        return inf;
    }

    DataBaseInfo::DataBaseInfo(const std::string &name, size_t size)
        : name_(name), size_(size)
    {

    }
}
