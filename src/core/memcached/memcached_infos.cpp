#include "core/memcached/memcached_infos.h"

#include <ostream>

namespace fastoredis
{   
    MemcachedServerInfo::MemcachedServerInfo()
        : ServerInfo(MEMCACHED)
    {

    }

    common::Value* MemcachedServerInfo::valueByIndexes(unsigned char property, unsigned char field) const
    {
        return NULL;
    }

    std::ostream& operator<<(std::ostream& out, const MemcachedServerInfo& value)
    {
        return out;
    }

    ServerInfoSPtr makeMemcachedServerInfo(const std::string &content)
    {
        if(content.empty()){
            return ServerInfoSPtr();
        }

        MemcachedServerInfo* result = new MemcachedServerInfo;
        ServerInfoSPtr sresult(result);

        return sresult;
    }

    ServerInfoSPtr makeMemcachedServerInfo(FastoObject* root)
    {
        const std::string content = common::convertToString(root);
        return makeMemcachedServerInfo(content);
    }

    MemcachedBaseInfo::MemcachedBaseInfo(const std::string& name, size_t size, bool isDefault)
        : DataBaseInfo(name, size, isDefault, MEMCACHED)
    {

    }

    DataBaseInfo* MemcachedBaseInfo::clone() const
    {
        return new MemcachedBaseInfo(*this);
    }
}
