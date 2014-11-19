#pragma once

/**/

#include <QDialog>

#include "core/connection_settings.h"

class QTreeWidget;

namespace fastoredis
{
    class ConnectionListWidgetItem;

    class ConnectionsDialog
            : public QDialog
    {
        Q_OBJECT

    public:
        ConnectionsDialog(QWidget *parent = 0);
        IConnectionSettingsBasePtr selectedConnection() const;
        virtual void accept();

    private Q_SLOTS:
        void add();
        void remove();
        void edit();
        void connectionSelectChange();

    protected:
        virtual void changeEvent(QEvent* );

    private:
        void retranslateUi();
        void add(const fastoredis::IConnectionSettingsBasePtr& con);
        QTreeWidget* listWidget_;
        QPushButton* acButton_;
    };
}
