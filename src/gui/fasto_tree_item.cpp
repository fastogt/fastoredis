#include "gui/fasto_tree_item.h"

namespace fastoredis
{
    FastoTreeItem::FastoTreeItem(const QString &key, const QString &value, common::Value::Type type, FastoTreeItem *parent)
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
}
