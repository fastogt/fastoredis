#pragma once

#include <QDialog>

class QComboBox;
class QLineEdit;
class QDialogButtonBox;
class QPushButton;
class QCheckBox;
class QLabel;

#include "core/connection_settings.h"

namespace fastoredis
{
    class ConnectionDialog
            : public QDialog
    {
        Q_OBJECT

    public:
        ConnectionDialog(QWidget* parent, IConnectionSettingsBase* connection = NULL); //get ownerships connection
        IConnectionSettingsBaseSPtr connection() const;

    public Q_SLOTS:
        virtual void accept();

    private Q_SLOTS:
        void typeConnectionChange(const QString& value);
        void securityChange(const QString& val);
        void sshSupportStateChange(int value);
        void togglePasswordEchoMode();
        void togglePassphraseEchoMode();
        void setPrivateFile();
        void testConnection();

    protected:
        virtual void changeEvent(QEvent* );

    private:
        void retranslateUi();
        bool validateAndApply();
        SSHInfo::SupportedAuthenticationMetods selectedAuthMethod() const;

        IConnectionSettingsBaseSPtr connection_;
        QLineEdit* connectionName_;
        QComboBox* typeConnection_;
        QCheckBox* logging_;
        QLineEdit* commandLine_;

        QDialogButtonBox *buttonBox_;

        QCheckBox* useSsh_;

        QWidget* useSshWidget_;
        QLineEdit* sshHostName_;
        QLineEdit* sshPort_;

        QLabel* sshAddressLabel_;
        QLabel* sshPassphraseLabel_;
        QLabel* sshUserNameLabel_;
        QLineEdit* userName_;
        QLabel* sshAuthMethodLabel_;
        QLabel* passwordLabel_;
        QLabel* sshPrivateKeyLabel_;

        QComboBox* security_;
        QLineEdit* passwordBox_;
        QPushButton* passwordEchoModeButton_;
        QLineEdit* privateKeyBox_;
        QLineEdit* passphraseBox_;
        QPushButton* passphraseEchoModeButton_;
        QPushButton* selectPrivateFileButton_;
    };
}
