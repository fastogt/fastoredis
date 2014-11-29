#include "gui/fasto_common_item.h"

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
            return QString("%1 %2").arg(item->key()).arg(item->value());
        }

        QString value;
        for(int i = 0; i < item->childrenCount(); ++i){
            value += toJson(dynamic_cast<FastoCommonItem*>(item->child(i)));
        }

        return QString("%1 %2").arg(item->key()).arg(value);
    }
}
