#pragma once

#include <QDialog>

class QComboBox;
class QLineEdit;
class QDialogButtonBox;
class QPushButton;

#include "core/ConnectionSettings.h"

namespace fastoredis
{
    class IConnectionSettingsBase;
    class ConnectionDialog : public QDialog
    {
        Q_OBJECT

    public:
        ConnectionDialog(const IConnectionSettingsBasePtr &connection, QWidget *parent = 0);

    public Q_SLOTS:
        virtual void accept();

    private Q_SLOTS:
        void testConnection();
        void typeConnectionChange(const QString &value);

    private:
        bool validateAndApply();
        const IConnectionSettingsBasePtr connection_;
        QLineEdit *connectionName_;
        QComboBox *typeConnection_;
        QLineEdit *commandLine_;

        QPushButton *testButton_;
        QDialogButtonBox *buttonBox_;
    };
}
