#pragma once

#include "gui/base/TreeModel.h"
#include "core/IServer.h"

namespace fastoredis
{
    struct ExplorerServerItem
            : public TreeItem
    {
        typedef TreeItem base_class;
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
        ExplorerServerItem(const IServerPtr &server, ExplorerServerItem *parent);
        virtual QString name() const;
        virtual IServerPtr server() const;
        virtual eType type() const;
    protected:
        IServerPtr server_;
    };

    struct ExplorerDatabaseItem
            : public ExplorerServerItem
    {
        typedef ExplorerServerItem base_class;
        ExplorerDatabaseItem(ExplorerServerItem *par, const QString &name);
        virtual QString name() const;
        virtual IServerPtr server() const;
        virtual eType type() const;
    private:
        QString name_;
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
        void addDatabase(IServer *server, const QString &name);

        ~ExplorerTreeModel();
    };
}


