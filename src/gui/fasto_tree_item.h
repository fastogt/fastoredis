#pragma once

/**/

#include <QString>

#include "common/value.h"
#include "gui/base/tree_item.h"

namespace fastoredis
{
    class FastoTreeItem
            : public TreeItem
    {
    public:
        enum eColumn
        {
            eKey = 0,
            eValue = 1,
            eType = 2,
            eCountColumns = 3
        };
        typedef std::vector<FastoTreeItem*> child_container_type;
        FastoTreeItem(const QString &key, const QString &value, common::Value::Type type, FastoTreeItem *parent = 0);

        QString key() const;
        QString value() const;
        common::Value::Type type() const;

    private:
        QString key_;
        QString value_;
        common::Value::Type type_;
    };
}

