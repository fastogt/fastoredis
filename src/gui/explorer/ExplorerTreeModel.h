#pragma once

#include "gui/base/TreeModel.h"
#include "core/IServer.h"

namespace fastoredis
{
    struct ExplorerTreeItem
            : public TreeItem
    {
        typedef TreeItem base_class;
        enum eColumn
        {
            eName = 0,
            eStatus,
            eCountColumns
        };
        ExplorerTreeItem(ExplorerTreeItem *parent)
            : base_class(parent)
        {

        }
        IServerPtr server_;
    };

    class ExplorerTreeModel
            : public TreeModel
    {
        Q_OBJECT
    public:
        typedef TreeModel base_class;
        ExplorerTreeModel(QObject *parent = 0);
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        virtual int columnCount(const QModelIndex &parent) const;

        void addServer(const IServerPtr &server);
        void removeServer(const IServerPtr &server);

        ~ExplorerTreeModel();
    };
}


