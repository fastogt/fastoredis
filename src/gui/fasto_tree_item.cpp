#include "gui/fasto_tree_item.h"

namespace fastoredis
{
    FastoTreeItem::FastoTreeItem(const QString& key, const QString& value, common::Value::Type type, FastoTreeItem* parent)
        : TreeItem(parent), key_(key), value_(value), type_(type)
    {

    }

    QString FastoTreeItem::key() const
    {
        return key_;
    }

    QString FastoTreeItem::value() const
    {
        return value_;
    }

    common::Value::Type FastoTreeItem::type() const
    {
        return type_;
    }

    QString toJson(FastoTreeItem* item)
    {
        if(!item){
            return "{  }";
        }

        if(!item->childrenCount()){
            return QString("{ %1 %2 }").arg(item->key()).arg(item->value());
        }

        QString value;
        for(int i = 0; i < item->childrenCount(); ++i){
            value += toJson(dynamic_cast<FastoTreeItem*>(item->child(i)));
        }

        return QString("{ %1 %2 }").arg(item->key()).arg(value);
    }
}
