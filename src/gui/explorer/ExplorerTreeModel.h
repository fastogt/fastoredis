#pragma once

#include "gui/base/TreeModel.h"
#include "core/IServer.h"

namespace fastoredis
{
    struct IExplorerTreeItem
            : public TreeItem
    {
        enum eColumn
        {
            eName = 0,
            eCountColumns
        };
        enum eType
        {
            Server,
            Database
        };
        IExplorerTreeItem(TreeItem *parent);
        virtual QString name() const = 0;
        virtual IServerPtr server() const = 0;
        virtual eType type() const = 0;
    };

    struct ExplorerServerItem
            : public IExplorerTreeItem
    {
        ExplorerServerItem(const IServerPtr &server, TreeItem *parent);
        virtual QString name() const;
        virtual IServerPtr server() const;
        virtual eType type() const;
    private:
        IServerPtr server_;
    };

    struct ExplorerDatabaseItem
            : public IExplorerTreeItem
    {
        ExplorerDatabaseItem(const IDatabasePtr &db, ExplorerServerItem *parent);
        ExplorerServerItem *parent() const;
        virtual QString name() const;
        virtual IServerPtr server() const;
        virtual eType type() const;
    private:
        IDatabasePtr db_;
    };

    class ExplorerTreeModel
            : public TreeModel
    {
        Q_OBJECT
    public:
        ExplorerTreeModel(QObject *parent = 0);
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        virtual int columnCount(const QModelIndex &parent) const;

        void addServer(const IServerPtr &server);
        void removeServer(const IServerPtr &server);

        void addDatabase(const IDatabasePtr &db);

        ~ExplorerTreeModel();

    private:
        ExplorerServerItem *findServerItem(IServer *server) const;
    };
}


