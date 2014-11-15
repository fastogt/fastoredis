#include "gui/dialogs/connection_dialog.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QEvent>
#include <QLabel>
#include <QFileDialog>

#include "gui/gui_factory.h"
#include "common/qt/convert_string.h"
#include "common/utils.h"

namespace
{
    QString trPrivateKey = QObject::tr("Private Key");
    QString trPassword = QObject::tr("Password");
    QString trHide = QObject::tr("Hide");
    QString trShow = QObject::tr("Show");
}

namespace fastoredis
{
    ConnectionDialog::ConnectionDialog(const IConnectionSettingsBasePtr &connection, QWidget *parent)
        : QDialog(parent),
        connection_(connection)
    {
        setWindowIcon(GuiFactory::instance().serverIcon());
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove help button (?)

        connectionName_ = new QLineEdit;
        connectionName_->setText(common::convertFromString<QString>(connection_->connectionName()));
        typeConnection_ = new QComboBox;

        std::vector<std::string> supt = supportedConnectionTypes();
        for(std::vector<std::string>::const_iterator it = supt.begin(); it != supt.end(); ++it){
            typeConnection_->addItem(common::convertFromString<QString>(*it));
        }
        typeConnection_->setCurrentText(common::convertFromString<QString>(common::convertToString(connection_->connectionType())));
        VERIFY(connect(typeConnection_, SIGNAL(currentTextChanged(const QString&)), this, SLOT(typeConnectionChange(const QString&))));

        logging_ = new QCheckBox;
        logging_->setChecked(connection_->loggingEnabled());

        commandLine_ = new QLineEdit;
        commandLine_->setText(common::convertFromString<QString>(connection_->commandLine()));

        QVBoxLayout *inputLayout = new QVBoxLayout;
        inputLayout->addWidget(connectionName_);
        inputLayout->addWidget(typeConnection_);
        inputLayout->addWidget(logging_);
        inputLayout->addWidget(commandLine_);

        testButton_ = new QPushButton;
        testButton_->setIcon(GuiFactory::instance().messageBoxInformationIcon());
        VERIFY(connect(testButton_, SIGNAL(clicked()), this, SLOT(testConnection())));

        SSHInfo info = connection_->sshInfo();
        useSsh_ = new QCheckBox;
        useSsh_->setChecked(info.isValid());

        sshHostName_ = new QLineEdit;
        sshHostName_->setText(common::convertFromString<QString>(info.hostName_));

        userName_ = new QLineEdit;
        userName_->setText(common::convertFromString<QString>(info.userName_));

        sshPort_ = new QLineEdit;
        sshPort_->setFixedWidth(80);
        QRegExp rx("\\d+");//(0-65554)
        sshPort_->setValidator(new QRegExpValidator(rx, this));
        sshPort_->setText(QString::number(info.port_));

        passwordLabel_ = new QLabel;
        sshPrivateKeyLabel_ = new QLabel;
        sshPassphraseLabel_ = new QLabel;
        sshAddressLabel_ = new QLabel;
        sshUserNameLabel_ = new QLabel;
        sshAuthMethodLabel_ = new QLabel;

        security_ = new QComboBox;
        security_->addItems(QStringList() << trPassword << trPrivateKey);
        if (info.authMethod() == SSHInfo::PUBLICKEY) {
            security_->setCurrentText(trPrivateKey);
        }
        else {
            security_->setCurrentText(trPassword);
        }
        VERIFY(connect(security_, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(securityChange(const QString&))));

        passwordBox_ = new QLineEdit;
        passwordBox_->setText(common::convertFromString<QString>(info.password_));
        passwordBox_->setEchoMode(QLineEdit::Password);
        passwordEchoModeButton_ = new QPushButton(tr("Show"));
        VERIFY(connect(passwordEchoModeButton_, SIGNAL(clicked()), this, SLOT(togglePasswordEchoMode())));

        privateKeyBox_ = new QLineEdit;
        privateKeyBox_->setText(common::convertFromString<QString>(info.privateKey_));

        passphraseBox_ = new QLineEdit;
        passphraseBox_->setText(common::convertFromString<QString>(info.passphrase_));
        passphraseBox_->setEchoMode(QLineEdit::Password);
        passphraseEchoModeButton_ = new QPushButton(tr("Show"));
        VERIFY(connect(passphraseEchoModeButton_, SIGNAL(clicked()), this, SLOT(togglePassphraseEchoMode())));

        useSshWidget_ = new QWidget;

        QHBoxLayout *hostAndPasswordLayout = new QHBoxLayout;
        hostAndPasswordLayout->addWidget(sshHostName_);
        hostAndPasswordLayout->addWidget(new QLabel(":"));
        hostAndPasswordLayout->addWidget(sshPort_);

        QGridLayout *sshWidgetLayout = new QGridLayout;
        sshWidgetLayout->setAlignment(Qt::AlignTop);
        sshWidgetLayout->setColumnStretch(1, 1);
        sshWidgetLayout->addWidget(sshAddressLabel_ , 1, 0);
        sshWidgetLayout->addLayout(hostAndPasswordLayout, 1, 1, 1, 2);
        sshWidgetLayout->addWidget(sshUserNameLabel_, 2, 0);
        sshWidgetLayout->addWidget(userName_, 2, 1, 1, 2);
        sshWidgetLayout->addWidget(sshAuthMethodLabel_, 4, 0);
        sshWidgetLayout->addWidget(security_, 4, 1, 1, 2);
        sshWidgetLayout->addWidget(passwordLabel_, 5, 0);
        sshWidgetLayout->addWidget(passwordBox_, 5, 1);
        sshWidgetLayout->addWidget(passwordEchoModeButton_, 5, 2);
        sshWidgetLayout->addWidget(sshPrivateKeyLabel_, 7, 0);
        sshWidgetLayout->addWidget(privateKeyBox_, 7, 1);
        selectPrivateFileButton_ = new QPushButton("...");
        selectPrivateFileButton_->setFixedSize(20, 20);
        sshWidgetLayout->addWidget(selectPrivateFileButton_, 7, 2);
        sshWidgetLayout->addWidget(sshPassphraseLabel_, 8, 0);
        sshWidgetLayout->addWidget(passphraseBox_, 8, 1);
        sshWidgetLayout->addWidget(passphraseEchoModeButton_, 8, 2);
        useSshWidget_->setLayout(sshWidgetLayout);

        inputLayout->addWidget(useSsh_);

        VERIFY(connect(selectPrivateFileButton_, SIGNAL(clicked()), this, SLOT(setPrivateFile())));
        VERIFY(connect(useSsh_, SIGNAL(stateChanged(int)), this, SLOT(sshSupportStateChange(int))));

        QHBoxLayout *bottomLayout = new QHBoxLayout;
        bottomLayout->addWidget(testButton_, 1, Qt::AlignLeft);
        buttonBox_ = new QDialogButtonBox(this);
        buttonBox_->setOrientation(Qt::Horizontal);
        buttonBox_->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Save);
        VERIFY(connect(buttonBox_, SIGNAL(accepted()), this, SLOT(accept())));
        VERIFY(connect(buttonBox_, SIGNAL(rejected()), this, SLOT(reject())));
        bottomLayout->addWidget(buttonBox_);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addLayout(inputLayout);
        mainLayout->addWidget(useSshWidget_);
        mainLayout->addLayout(bottomLayout);
        mainLayout->setSizeConstraint(QLayout::SetFixedSize);
        setLayout(mainLayout);

        //update controls
        sshSupportStateChange(useSsh_->checkState());
        securityChange(security_->currentText());
        typeConnectionChange(typeConnection_->currentText());
        //syncWithConnection();
        retranslateUi();
    }

    void ConnectionDialog::syncWithConnection()
    {
        connectionName_->setText(common::convertFromString<QString>(connection_->connectionName()));
        typeConnection_->setCurrentText(common::convertFromString<QString>(common::convertToString(connection_->connectionType())));
        commandLine_->setText(common::convertFromString<QString>(connection_->commandLine()));
        logging_->setChecked(connection_->loggingEnabled());

        SSHInfo info = connection_->sshInfo();
        useSsh_->setChecked(info.isValid());
        sshHostName_->setText(common::convertFromString<QString>(info.hostName_));
        userName_->setText(common::convertFromString<QString>(info.userName_));
        sshPort_->setText(QString::number(info.port_));

        passwordBox_->setText(common::convertFromString<QString>(info.password_));
        privateKeyBox_->setText(common::convertFromString<QString>(info.privateKey_));
        passphraseBox_->setText(common::convertFromString<QString>(info.passphrase_));

        if (info.authMethod() == SSHInfo::PUBLICKEY) {
            security_->setCurrentText("Private Key");
        }
        else {
            security_->setCurrentText("Password");
        }
    }

    void ConnectionDialog::accept()
    {
        if(validateAndApply()){
            connection_->setConnectionName(common::convertToString(connectionName_->text()));
            connection_->setCommandLine(common::convertToString(commandLine_->text()));
            connection_->setLoggingEnabled(logging_->isChecked());

            SSHInfo info = connection_->sshInfo();
            info.hostName_ = common::convertToString(sshHostName_->text());
            info.userName_ = common::convertToString(userName_->text());
            info.port_ = sshPort_->text().toInt();
            info.password_ = common::convertToString(passwordBox_->text());
            info.publicKey_ = "";
            info.privateKey_ = common::convertToString(privateKeyBox_->text());
            info.passphrase_ = common::convertToString(passphraseBox_->text());
            if (useSsh_->isChecked()){
                info.currentMethod_ = selectedAuthMethod();
            }
            else{
                info.currentMethod_ = SSHInfo::UNKNOWN;
            }
            connection_->setSshInfo(info);

            QDialog::accept();
        }
    }

    SSHInfo::SupportedAuthenticationMetods ConnectionDialog::selectedAuthMethod() const
    {
        if (security_->currentText() == trPrivateKey){
            return SSHInfo::PUBLICKEY;
        }

        return SSHInfo::PASSWORD;
    }

    void ConnectionDialog::sshSupportStateChange(int value)
    {
        useSshWidget_->setVisible(value);
        sshHostName_->setEnabled(value);
        userName_->setEnabled(value);
        sshPort_->setEnabled(value);
        security_->setEnabled(value);
        sshPrivateKeyLabel_->setEnabled(value);
        privateKeyBox_->setEnabled(value);
        selectPrivateFileButton_->setEnabled(value);
        sshAddressLabel_->setEnabled(value);
        sshUserNameLabel_->setEnabled(value);
        sshAuthMethodLabel_->setEnabled(value);
        sshPassphraseLabel_->setEnabled(value);
        passphraseBox_->setEnabled(value);
        passwordBox_->setEnabled(value);
        passwordLabel_->setEnabled(value);
    }

    void ConnectionDialog::securityChange(const QString&)
    {
        bool isKey = selectedAuthMethod() == SSHInfo::PUBLICKEY;
        sshPrivateKeyLabel_->setVisible(isKey);
        privateKeyBox_->setVisible(isKey);
        selectPrivateFileButton_->setVisible(isKey);
        sshPassphraseLabel_->setVisible(isKey);
        passphraseBox_->setVisible(isKey);
        passphraseEchoModeButton_->setVisible(isKey);
        passwordBox_->setVisible(!isKey);
        passwordLabel_->setVisible(!isKey);
        passwordEchoModeButton_->setVisible(!isKey);
    }

    void ConnectionDialog::setPrivateFile()
    {
        QString filepath = QFileDialog::getOpenFileName(this, tr("Select private key file"),
        privateKeyBox_->text(), tr("Private key files (*.*)"));
        if (filepath.isNull())
            return;

        privateKeyBox_->setText(filepath);
    }

    void ConnectionDialog::togglePasswordEchoMode()
    {
        bool isPassword = passwordBox_->echoMode() == QLineEdit::Password;
        passwordBox_->setEchoMode(isPassword ? QLineEdit::Normal: QLineEdit::Password);
        passwordEchoModeButton_->setText(isPassword ? trHide: trShow);
    }

    void ConnectionDialog::togglePassphraseEchoMode()
    {
        bool isPassword = passphraseBox_->echoMode() == QLineEdit::Password;
        passphraseBox_->setEchoMode(isPassword ? QLineEdit::Normal: QLineEdit::Password);
        passphraseEchoModeButton_->setText(isPassword ? trHide: trShow);
    }

    void ConnectionDialog::typeConnectionChange(const QString &value)
    {
        connectionTypes currentType = common::convertFromString<connectionTypes>(common::convertToString(value));
        bool isValidType = currentType != badConnectionType();
        connectionName_->setEnabled(isValidType);
        testButton_->setEnabled(isValidType);
        commandLine_->setEnabled(isValidType);
        buttonBox_->button(QDialogButtonBox::Save)->setEnabled(isValidType);
        commandLine_->setToolTip(useHelpText(currentType));

        useSsh_->setEnabled(isValidType);
    }

    bool ConnectionDialog::validateAndApply()
    {
        return true;
    }

    void ConnectionDialog::testConnection()
    {
        if(validateAndApply()){
           // ConnectionDiagnosticDialog diag(_connection,this);
           // diag.exec();
        }
    }

    void ConnectionDialog::changeEvent(QEvent *e)
    {
        if(e->type() == QEvent::LanguageChange){
            retranslateUi();
        }
        QDialog::changeEvent(e);
    }

    void ConnectionDialog::retranslateUi()
    {
        setWindowTitle(tr("Connection Settings"));
        logging_->setText(tr("Logging enabled"));
        testButton_->setText(tr("&Test"));
        useSsh_->setText(tr("Use SSH tunnel"));
        passwordLabel_->setText(tr("User Password:"));
        sshPrivateKeyLabel_->setText(tr("Private key:"));
        sshPassphraseLabel_->setText(tr("Passphrase:"));
        sshAddressLabel_->setText(tr("SSH Address:"));
        sshUserNameLabel_->setText(tr("SSH User Name:"));
        sshAuthMethodLabel_->setText(tr("SSH Auth Method:"));
    }
}
