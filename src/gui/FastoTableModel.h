#pragma once

#include <QAbstractProxyModel>
#include "global/global.h"

namespace fastoredis
{
    class FastoTableModel
		: public QAbstractProxyModel
    {
        Q_OBJECT

    public:
        typedef QAbstractProxyModel base_class;
        typedef std::vector<QString> ColumnsValuesType;

        explicit FastoTableModel(QObject *parent = 0);
        QVariant data(const QModelIndex &index, int role) const;

        int rowCount(const QModelIndex &parent=QModelIndex()) const;
        int columnCount(const QModelIndex &parent) const;

        QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;
        QModelIndex index( int row, int col, const QModelIndex& index ) const;
        virtual QModelIndex mapFromSource( const QModelIndex & sourceIndex ) const;
        virtual QModelIndex mapToSource( const QModelIndex &proxyIndex ) const;
        virtual QModelIndex parent( const QModelIndex& index ) const;
    };
}
