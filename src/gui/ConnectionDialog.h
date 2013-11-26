#pragma once

#include <QDialog>

#include "global/global.h"

namespace fastoredis
{
    class IConnectionSettingsBase;
    class ConnectionDialog : public QDialog
    {
        Q_OBJECT

    public:
        ConnectionDialog(IConnectionSettingsBase *connection, QWidget *parent = 0);

    public Q_SLOTS:
        virtual void accept();

    private Q_SLOTS:
        void testConnection();

    private:
        bool validateAndApply();
        IConnectionSettingsBase *const connection_;
    };
}
