#include "core/types.h"

namespace fastoredis
{
    DbValue::DbValue()
    {

    }

    DbValue::DbValue(const std::string& key, const std::string& value)
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
                ar->getString(i, &c1);
                ar->getString(i+1, &c2);
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

    void DataBaseInfo::setKeys(const std::vector<std::string>& keys)
    {
        keys_ = keys;
    }

    std::vector<std::string> DataBaseInfo::keys() const
    {
        return keys_;
    }
}
