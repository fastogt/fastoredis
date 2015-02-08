#pragma once

#include "global/global.h"

#include "core/connection_types.h"

namespace fastoredis
{
    struct DbValue
    {
        DbValue();
        DbValue(const std::string& key, const std::string& value);

        std::string key_;
        std::string value_;
    };

    struct ServerInfo
    {
        connectionTypes type() const;
        virtual std::string toString() const = 0;
        virtual common::Value* valueByIndexes(unsigned char property, unsigned char field) const = 0;
        virtual ~ServerInfo();

    protected:
        ServerInfo(connectionTypes type);
        ServerInfo(const ServerInfo& that);
        ServerInfo& operator=(const ServerInfo& that);

    private:
        const connectionTypes type_;
    };

    struct FieldByIndex
    {
        virtual common::Value* valueByIndex(unsigned char index) const = 0;
    };

    struct Field
    {
        Field(const std::string& name, common::Value::Type type);

        bool isIntegral() const;
        std::string name_;
        common::Value::Type type_;
    };

    typedef shared_ptr_t<ServerInfo> ServerInfoSPtr;

    struct ServerInfoSnapShoot
    {
        ServerInfoSnapShoot();
        ServerInfoSnapShoot(common::time64_t msec, ServerInfoSPtr info);
        bool isValid() const;

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

    class DataBaseInfo
    {
    public:
        connectionTypes type() const;
        std::string name() const;
        size_t size() const;

        bool isDefault() const;
        void setIsDefault(bool isDef);

        virtual DataBaseInfo* clone() const = 0;
        virtual ~DataBaseInfo();

        std::vector<std::string> keys() const;
        void setKeys(const std::vector<std::string>& keys);

    protected:
        DataBaseInfo(const std::string& name, size_t size, bool isDefault, connectionTypes type);

    private:
        std::string name_;
        size_t size_;
        bool isDefault_;
        std::vector<std::string> keys_;

        const connectionTypes type_;
    };

    inline bool operator == (const DataBaseInfo& lhs, const DataBaseInfo& rhs)
    {
        return lhs.name() == rhs.name() && lhs.size() == rhs.size() && lhs.isDefault() == rhs.isDefault() && lhs.type() == rhs.type();
    }
}
