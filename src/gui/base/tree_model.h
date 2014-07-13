#pragma once

/**/

#include <boost/scoped_ptr.hpp>

#include <QAbstractItemModel>

#include "gui/base/tree_item.h"

namespace fastoredis
{
    class TreeModel
            : public QAbstractItemModel
    {
        Q_OBJECT
    public:
        TreeModel(QObject *parent = 0);

        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent= QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex& index) const;

        virtual ~TreeModel();
    protected:
        boost::scoped_ptr<TreeItem> _root;
    };
}


