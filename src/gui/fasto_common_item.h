#pragma once

/**/

#include <QString>

#include "common/value.h"
#include "gui/base/tree_item.h"

namespace fastoredis
{
    class FastoCommonItem
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
        typedef std::vector<FastoCommonItem*> child_container_type;
        FastoCommonItem(const QString& key, const QString& value, common::Value::Type type, TreeItem* parent, void* internalPointer);

        QString key() const;
        QString value() const;
        void setValue(const QString& val);
        common::Value::Type type() const;

    private:
        QString key_;
        QString value_;
        common::Value::Type type_;
    };

    QString toJson(FastoCommonItem* item);
    QString toRaw(FastoCommonItem* item);
}

