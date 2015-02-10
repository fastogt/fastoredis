#pragma once

#include "gui/base/tree_model.h"

#include "core/iserver.h"
#include "core/idatabase.h"

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
            Database,
            Key
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

        void loadDatabases();

    private:
        const IServerSPtr server_;
    };

    struct ExplorerDatabaseItem
            : public IExplorerTreeItem
    {
        ExplorerDatabaseItem(DataBaseInfoSPtr db, ExplorerServerItem* parent);
        virtual ~ExplorerDatabaseItem();

        ExplorerServerItem* parent() const;

        virtual QString name() const;
        virtual eType type() const;       
        bool isDefault() const;

        virtual IServerSPtr server() const;
        IDatabaseSPtr db() const;

        void loadContent();
        void setDefault();
        void removeKey(const std::string &key);
        void loadValue(const std::string &key);

        DataBaseInfoSPtr info() const;
    private:
        DataBaseInfoSPtr inf_;
    };

    struct ExplorerKeyItem
            : public IExplorerTreeItem
    {
        ExplorerKeyItem(const std::string& name, ExplorerDatabaseItem* parent);
        virtual ~ExplorerKeyItem();

        ExplorerDatabaseItem* parent() const;
        virtual QString name() const;
        std::string sname() const;
        virtual IServerSPtr server() const;
        virtual eType type() const;

        void remove();
        void loadValue();

    private:
        std::string name_;
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

        void addDatabase(IServer* server, DataBaseInfoSPtr db);
        void removeDatabase(IServer* server, DataBaseInfoSPtr db);
        void setDefaultDb(IServer* server, DataBaseInfoSPtr db);

        void addKey(IServer* server, DataBaseInfoSPtr db, const std::string& key);
        void removeKey(IServer* server, DataBaseInfoSPtr db, const std::string& key);
    private:
        ExplorerServerItem *findServerItem(IServer* server) const;
        ExplorerDatabaseItem *findDatabaseItem(ExplorerServerItem* server, DataBaseInfoSPtr db) const;
        ExplorerKeyItem *findKeyItem(ExplorerDatabaseItem* db, const std::string& key) const;
    };
}


