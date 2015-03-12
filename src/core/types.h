#pragma once

#include "global/global.h"

#include "core/connection_types.h"

namespace fastoredis
{
    template<connectionTypes ct>
    struct DBTraits
    {
        static const common::Value::Type supportedTypes[];
    };

    struct NKey
    {
        explicit NKey(const std::string& key, common::Value::Type type = common::Value::TYPE_NULL);

        std::string key_;
        common::Value::Type type_;
    };

    inline bool operator == (const NKey& lhs, const NKey& rhs)
    {
        return lhs.key_ == rhs.key_ && lhs.type_ == rhs.type_;
    }

    struct NValue
    {
        explicit NValue(const std::string& value, common::Value::Type type = common::Value::TYPE_NULL);

        std::string value_;
        common::Value::Type type_;
    };

    inline bool operator == (const NValue& lhs, const NValue& rhs)
    {
        return lhs.value_ == rhs.value_ && lhs.type_ == rhs.type_;
    }

    struct NDbValue
    {
        NDbValue(const NKey& key, const NValue& value);

        NKey key_;
        NValue value_;
    };

    class ServerInfo
    {
    public:
        ServerInfo(connectionTypes type);
        virtual ~ServerInfo();

        connectionTypes type() const;
        virtual std::string toString() const = 0;
        virtual common::Value* valueByIndexes(unsigned char property, unsigned char field) const = 0;        

    protected:
        DISALLOW_COPY_AND_ASSIGN(ServerInfo);

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

    typedef common::shared_ptr<ServerInfo> ServerInfoSPtr;

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
        typedef std::vector<NKey> keys_cont_type;
        connectionTypes type() const;
        std::string name() const;
        size_t size() const;

        bool isDefault() const;
        void setIsDefault(bool isDef);

        virtual DataBaseInfo* clone() const = 0;
        virtual ~DataBaseInfo();

        keys_cont_type keys() const;
        void setKeys(const keys_cont_type& keys);

    protected:
        DataBaseInfo(const std::string& name, size_t size, bool isDefault, connectionTypes type);

    private:
        std::string name_;
        size_t size_;
        bool isDefault_;
        keys_cont_type keys_;

        const connectionTypes type_;
    };

    inline bool operator == (const DataBaseInfo& lhs, const DataBaseInfo& rhs)
    {
        return lhs.name() == rhs.name() && lhs.size() == rhs.size() && lhs.isDefault() == rhs.isDefault() && lhs.type() == rhs.type();
    }

    class CommandKey
    {
    public:
        enum cmdtype
        {
            C_NONE,
            C_DELETE,
            C_LOAD,
            C_CREATE
        };

        cmdtype type() const;
        NKey key() const;

        virtual ~CommandKey();

    protected:
        CommandKey(const NKey& key, cmdtype type);

    private:
        cmdtype type_;
        NKey key_;
    };

    class CommandDeleteKey
            : public CommandKey
    {
    public:
        CommandDeleteKey(const NKey& key);
    };

    class CommandLoadKey
            : public CommandKey
    {
    public:
        CommandLoadKey(const NKey& key);
    };

    class CommandCreateKey
            : public CommandKey
    {
    public:
        CommandCreateKey(const NKey& key, FastoObjectIPtr value);
        FastoObjectIPtr value() const;

    private:
        FastoObjectIPtr value_;
    };

    typedef common::shared_ptr<CommandKey> CommandKeySPtr;
}
