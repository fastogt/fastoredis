#pragma once

#include <boost/scoped_ptr.hpp>
#include <QAbstractItemModel>

#include "gui/base/TreeItem.h"

namespace fastoredis
{
    class TreeModel
            : public QAbstractItemModel
    {
        Q_OBJECT
    public:
        TreeModel(QObject *parent = 0);

        int rowCount(const QModelIndex &parent=QModelIndex()) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent= QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex& index) const;

        ~TreeModel();
    protected:
        boost::scoped_ptr<TreeItem> _root;
    };
}


