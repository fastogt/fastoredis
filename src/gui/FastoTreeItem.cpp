#include "gui/FastoTreeItem.h"

namespace fastoredis
{
    FastoTreeItem::FastoTreeItem(const QString &key, const QString &value, fastoType type, FastoTreeItem *parent)
        : base_class(parent), key_(key), value_(value), type_(type)
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

    fastoType FastoTreeItem::type() const
    {
        return type_;
    }
}
