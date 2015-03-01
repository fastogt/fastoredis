#include "core/types.h"

namespace fastoredis
{
    NKey::NKey()
        : key_(), type_(common::Value::TYPE_NULL)
    {

    }

    NKey::NKey(const std::string& key, common::Value::Type type)
        : key_(key), type_(type)
    {
    }

    NValue::NValue()
        : value_(), type_(common::Value::TYPE_NULL)
    {

    }

    NValue::NValue(const std::string& value, common::Value::Type type)
        : value_(value), type_(type)
    {

    }

    NDbValue::NDbValue()
    {

    }

    NDbValue::NDbValue(const NKey& key, const NValue& value)
        : key_(key), value_(value)
    {

    }

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

    Field::Field(const std::string& name, common::Value::Type type)
        : name_(name), type_(type)
    {

    }

    bool Field::isIntegral() const
    {
        return common::Value::isIntegral(type_);
    }

    ServerInfoSnapShoot::ServerInfoSnapShoot()
        : msec_(0), info_()
    {

    }

    ServerInfoSnapShoot::ServerInfoSnapShoot(common::time64_t msec, ServerInfoSPtr info)
        : msec_(msec), info_(info)
    {

    }

    bool ServerInfoSnapShoot::isValid() const
    {
        return msec_ > 0 && info_;
    }

    ServerPropertyInfo::ServerPropertyInfo()
    {

    }

    ServerPropertyInfo makeServerProperty(FastoObjectArray* array)
    {
        ServerPropertyInfo inf;

        common::ArrayValue* ar = array->array();
        if(ar){
            for(int i = 0; i < ar->getSize(); i+=2){
                std::string c1;
                std::string c2;
                bool res = ar->getString(i, &c1);
                DCHECK(res);
                res = ar->getString(i+1, &c2);
                DCHECK(res);
                inf.propertyes_.push_back(std::make_pair(c1, c2));
            }
        }
        return inf;
    }

    DataBaseInfo::DataBaseInfo(const std::string& name, size_t size, bool isDefault, connectionTypes type)
        : name_(name), size_(size), isDefault_(isDefault), type_(type)
    {

    }

    DataBaseInfo::~DataBaseInfo()
    {

    }

    connectionTypes DataBaseInfo::type() const
    {
        return type_;
    }

    std::string DataBaseInfo::name() const
    {
        return name_;
    }

    size_t DataBaseInfo::size() const
    {
        return size_;
    }

    bool DataBaseInfo::isDefault() const
    {
        return isDefault_;
    }

    void DataBaseInfo::setIsDefault(bool isDef)
    {
        isDefault_ = isDef;
    }

    void DataBaseInfo::setKeys(const keys_cont_type& keys)
    {
        keys_ = keys;
    }

    DataBaseInfo::keys_cont_type DataBaseInfo::keys() const
    {
        return keys_;
    }

    CommandKey::CommandKey(const std::string& key, cmdtype type)
        : key_(key), type_(type), execCommand_()
    {

    }

    CommandKey::cmdtype CommandKey::type() const
    {
        return type_;
    }

    std::string CommandKey::key() const
    {
        return key_;
    }

    common::Value::Type CommandKey::itype() const
    {
        return itype_;
    }

    std::string CommandKey::execCommand() const
    {
        return execCommand_;
    }

    void CommandKey::setExecCommand(const std::string& execCommand)
    {
        execCommand_ = execCommand;
    }
}
