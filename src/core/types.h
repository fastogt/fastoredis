#pragma once

#include "global/global.h"

#include "core/connection_types.h"

namespace fastoredis
{
    struct ServerInfo
    {
        connectionTypes type() const;
        virtual common::Value* valueByIndexes(unsigned char property, unsigned char field) const = 0;
        virtual ~ServerInfo();

    protected:
        ServerInfo(connectionTypes type);
        ServerInfo(const ServerInfo& that);
        ServerInfo& operator=(const ServerInfo& that);

    private:
        const connectionTypes type_;
    };

    typedef shared_ptr_t<ServerInfo> ServerInfoSPtr;

    struct ServerInfoSnapShoot
    {
        ServerInfoSnapShoot()
            : msec_(0), info_()
        {

        }

        ServerInfoSnapShoot(common::time64_t msec, ServerInfoSPtr info)
            : msec_(msec), info_(info)
        {

        }

        bool isValid() const
        {
            return msec_ > 0 && info_;
        }

        common::time64_t msec_;
        ServerInfoSPtr info_;
    };

    typedef std::pair<std::string, std::string> PropertyType;

    struct ServerPropertyInfo
    {
        ServerPropertyInfo();
        std::vector<PropertyType> propertyes_;
    };

    ServerPropertyInfo makeServerProperty(FastoObjectArray* array);

    struct DataBaseInfo
    {
        DataBaseInfo(const std::string& name, size_t size, bool isDefault);

        std::string name_;
        size_t size_;
        bool isDefault_;
    };

    inline bool operator == (const DataBaseInfo& lhs, const DataBaseInfo& rhs)
    {
        return lhs.name_ == rhs.name_ && lhs.size_ == rhs.size_;
    }
}
