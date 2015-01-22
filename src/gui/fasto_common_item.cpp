#include "gui/fasto_common_item.h"

#include "common/qt/convert_string.h"

#include "core/lua_engine.h"

namespace fastoredis
{
    FastoCommonItem::FastoCommonItem(const QString& key, const QString& value, common::Value::Type type, TreeItem *parent, void* internalPointer)
        : TreeItem(parent, internalPointer), key_(key), value_(value), type_(type)
    {

    }

    QString FastoCommonItem::key() const
    {
        return key_;
    }

    QString FastoCommonItem::value() const
    {
        return value_;
    }

    void FastoCommonItem::setValue(const QString& val)
    {
        value_ = val;
    }

    common::Value::Type FastoCommonItem::type() const
    {
        return type_;
    }

    QString toJson(FastoCommonItem* item)
    {
        if(!item){
            return "{  }";
        }

        if(!item->childrenCount()){
            return QString("{ %1 %2 }").arg(item->key()).arg(item->value());
        }

        QString value;
        for(int i = 0; i < item->childrenCount(); ++i){
            value += toJson(dynamic_cast<FastoCommonItem*>(item->child(i)));
        }

        return QString("{ %1 %2 }").arg(item->key()).arg(value);
    }

    QString toRaw(FastoCommonItem* item)
    {
        if(!item){
            return "";
        }

        if(!item->childrenCount()){
            return item->value();
        }

        QString value;
        for(int i = 0; i < item->childrenCount(); ++i){
            value += toRaw(dynamic_cast<FastoCommonItem*>(item->child(i)));
        }

        return value;
    }

    QString toMsgPack(FastoCommonItem* item)
    {
        if(!item){
            return "";
        }

        if(!item->childrenCount()){
            std::string sval = common::convertToString(item->value());
            std::string upack = LuaEngine::instance().mpUnPack(sval);
            return common::convertFromString<QString>(upack);
        }

        QString value;
        for(int i = 0; i < item->childrenCount(); ++i){
            value += toMsgPack(dynamic_cast<FastoCommonItem*>(item->child(i)));
        }

        return value;
    }

    QString toHex(FastoCommonItem* item)
    {
        if(!item){
            return "";
        }

        if(!item->childrenCount()){
            QString val = item->value();
            std::string sval = common::convertToString(val);
            std::string hex = common::HexEncode(sval);
            return common::convertFromString<QString>(hex);
        }

        QString value;
        for(int i = 0; i < item->childrenCount(); ++i){
            value += toHex(dynamic_cast<FastoCommonItem*>(item->child(i)));
        }

        return value;
    }

    QString toCsv(FastoCommonItem* item, const QString& delemitr)
    {
        if(!item){
            return "";
        }

        if(!item->childrenCount()){
            return item->value().replace(delemitr, ",");
        }

        QString value;
        for(int i = 0; i < item->childrenCount(); ++i){
            value += toCsv(dynamic_cast<FastoCommonItem*>(item->child(i)), delemitr);
            if(i != item->childrenCount() - 1){
                value += ",";
            }
        }

        return value;
    }
}
