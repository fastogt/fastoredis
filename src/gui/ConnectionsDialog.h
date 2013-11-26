#pragma once

#include <QDialog>
#include <QTreeWidget>

namespace fastoredis
{
    class ConnectionListWidgetItem;
    class IConnectionSettingsBase;

    class ConnectionsDialog
            : public QDialog
    {
        Q_OBJECT

    public:
        typedef QDialog base_class;
        typedef std::vector<ConnectionListWidgetItem *> ConnectionListItemContainerType;
        ConnectionsDialog(QWidget *parent = 0);
        IConnectionSettingsBase *selectedConnection() const;
        virtual void accept();

    private:
        void add(IConnectionSettingsBase *con);
        QTreeWidget *_listWidget;
        ConnectionListItemContainerType _connectionItems;
    };
}
