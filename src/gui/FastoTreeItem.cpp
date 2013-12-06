#include "gui/FastoTreeItem.h"

namespace fastoredis
{
    FastoTreeItem::FastoTreeItem(const QString &key, const QString &value, fastoType type, FastoTreeItem *parent)
        : parent_(parent), key_(key), value_(value), type_(type)
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

    void FastoTreeItem::addChildren(FastoTreeItem *child)
    {
        childrens_.push_back(child);
    }

    int FastoTreeItem::childrenCount() const
    {
        return childrens_.size();
    }

    FastoTreeItem *FastoTreeItem::child(unsigned pos) const
    {
        return childrens_[pos];
    }

    int FastoTreeItem::indexOf(FastoTreeItem *item) const
    {
        for (unsigned i=0; i < childrens_.size(); ++i) {
            if (item == childrens_[i]) {
                return i;
            }
        }
        return -1;
    }

    FastoTreeItem *FastoTreeItem::parent() const
    {
        return parent_;
    }

    FastoTreeItem::~FastoTreeItem()
    {
        for(unsigned i=0; i < childrens_.size(); ++i){
            delete childrens_[i];
        }
    }
}
