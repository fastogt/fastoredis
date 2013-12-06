#pragma once
#include <boost/scoped_ptr.hpp>
#include <QAbstractItemModel>

#include "global/global.h"

namespace fastoredis
{
    class FastoTreeItem;

    class FastoTreeModel
            : public QAbstractItemModel
    {
        Q_OBJECT
    public:
        typedef QAbstractItemModel base_class;
        FastoTreeModel(QObject *parent = 0);
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        Qt::ItemFlags flags(const QModelIndex &index) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const;

        int rowCount(const QModelIndex &parent=QModelIndex()) const;
        int columnCount(const QModelIndex &parent) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent= QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex& index) const;

        void setRoot(FastoTreeItem *root);
        ~FastoTreeModel();
    private:
        boost::scoped_ptr<FastoTreeItem> _root;
    };
}


