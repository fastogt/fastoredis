#pragma once

#include "gui/base/tree_model.h"

#include "core/iserver.h"

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

        IExplorerTreeItem(TreeItem* parent);
        virtual ~IExplorerTreeItem();

        virtual QString name() const = 0;
        virtual IServerSPtr server() const = 0;
        virtual eType type() const = 0;
    };

    struct ExplorerServerItem
            : public IExplorerTreeItem
    {
        ExplorerServerItem(IServerSPtr server, TreeItem* parent);
        virtual ~ExplorerServerItem();

        virtual QString name() const;
        virtual IServerSPtr server() const;
        virtual eType type() const;

    private:
        const IServerSPtr server_;
    };

    struct ExplorerDatabaseItem
            : public IExplorerTreeItem
    {
        ExplorerDatabaseItem(const DataBaseInfo& db, ExplorerServerItem* parent);
        virtual ~ExplorerDatabaseItem();

        ExplorerServerItem* parent() const;
        virtual QString name() const;
        virtual IServerSPtr server() const;
        virtual eType type() const;
        void loadContent();
        void setDefault();

        bool isDefault() const;

        DataBaseInfo db() const;
        void setDb(const DataBaseInfo& db);

    private:
        DataBaseInfo db_;
    };

    class ExplorerTreeModel
            : public TreeModel
    {
        Q_OBJECT
    public:
        ExplorerTreeModel(QObject* parent = 0);
        virtual ~ExplorerTreeModel();

        virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
        virtual Qt::ItemFlags flags(const QModelIndex& index) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        virtual int columnCount(const QModelIndex &parent) const;

        void addServer(IServerSPtr server);
        void removeServer(IServerSPtr server);

        void addDatabase(IServer *server, const DataBaseInfo& db);
        void setDefaultDatabase(IServer* server, const DataBaseInfo& db);

    private:
        ExplorerServerItem *findServerItem(IServer* server) const;
        ExplorerDatabaseItem *findDatabaseItem(ExplorerServerItem* server, const DataBaseInfo& db) const;
    };
}


