#pragma once
#include "global/global.h"

#include <QAbstractItemModel>

namespace fastoredis
{
    class FastoTreeModel
            : public QAbstractItemModel
    {
        Q_OBJECT
    public:
        typedef QAbstractItemModel base_class;
        FastoTreeModel(const FastoObjectPtr &root, QObject *parent = 0);

    private:
        const FastoObjectPtr _root;
    };
}
