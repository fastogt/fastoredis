#include "core/types.h"

#include "common/convert2string.h"

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
            const common::string16 c1 = childrens[i]->toString16();
            const common::string16 c2 = childrens[i+1]->toString16();
            inf.propertyes_.push_back(std::make_pair(common::convertToString(c1), common::convertToString(c2)));
        }
        return inf;
    }

    DataBaseInfo::DataBaseInfo(const common::string16 &name, size_t size)
        : name_(name), size_(size)
    {

    }
}
