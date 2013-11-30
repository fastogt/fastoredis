#pragma once

#include <QDialog>
#include <QTreeWidget>

#include "core/ConnectionSettings.h"

namespace fastoredis
{
    class ConnectionListWidgetItem;

    class ConnectionsDialog
            : public QDialog
    {
        Q_OBJECT

    public:
        typedef QDialog base_class;
        typedef std::vector<ConnectionListWidgetItem *> ConnectionListItemContainerType;
        ConnectionsDialog(QWidget *parent = 0);
        IConnectionSettingsBasePtr selectedConnection() const;
        virtual void accept();

    private Q_SLOTS:
        void add();
        void remove();
        void edit();

    private:
        void add(const fastoredis::IConnectionSettingsBasePtr &con);
        QTreeWidget *_listWidget;
        ConnectionListItemContainerType _connectionItems;
    };
}
