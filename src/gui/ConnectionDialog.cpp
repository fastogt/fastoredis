#include "gui/ConnectionDialog.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QLineEdit>

#include "gui/GuiFactory.h"
#include "common/qt_helper/converter_patterns.h"
#include "common/utils.h"

namespace fastoredis
{
    ConnectionDialog::ConnectionDialog(const IConnectionSettingsBasePtr &connection, QWidget *parent)
        : QDialog(parent),
        connection_(connection)
    {
        using namespace common;
        setWindowTitle("Connection Settings");
        setWindowIcon(GuiFactory::instance().serverIcon());
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove help button (?)

        connectionName_ = new QLineEdit;
        connectionName_->setText(utils_qt::toQString(connection_->connectionName()));

        typeConnection_ = new QComboBox;
        std::vector<unicode_string> supt = detail::supportedConnectionTypes();
        for(std::vector<unicode_string>::const_iterator it = supt.begin(); it != supt.end(); ++it){
            typeConnection_->addItem(utils_qt::toQString(*it));
        }
        typeConnection_->setCurrentText(utils_qt::toQString(detail::toStdString(connection_->connectionType())));
        VERIFY(connect(typeConnection_, SIGNAL(currentTextChanged(const QString&)), this, SLOT(typeConnectionChange(const QString&))));

        hostAndPort h = connection_->host();
        host_ = new QLineEdit;
        host_->setText(utils_qt::toQString(h.first));
        port_ = new QLineEdit;
        QRegExp rx("\\d+");//(0-65554)
        port_->setValidator(new QRegExpValidator(rx, this));
        port_->setText(utils_qt::toQString(utils::converter::make_conversion<std::string>(h.second)));

        QHBoxLayout *hostAndPortLayout = new QHBoxLayout;
        hostAndPortLayout->addWidget(host_);
        hostAndPortLayout->addWidget(port_);

        QVBoxLayout *inputLayout = new QVBoxLayout;
        inputLayout->addWidget(connectionName_);
        inputLayout->addWidget(typeConnection_);
        inputLayout->addLayout(hostAndPortLayout);



        testButton_ = new QPushButton("&Test");
        testButton_->setIcon(GuiFactory::instance().messageBoxInformationIcon());
        VERIFY(connect(testButton_, SIGNAL(clicked()), this, SLOT(testConnection())));

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
        mainLayout->addLayout(bottomLayout);
        setLayout(mainLayout);

        //update controls
        typeConnectionChange(typeConnection_->currentText());
    }

    void ConnectionDialog::typeConnectionChange(const QString &value)
    {
        IConnectionSettingsBase::connectionTypes currentType = detail::toConnectionType(common::utils_qt::toStdString(value));
        bool isValidType = currentType != IConnectionSettingsBase::badConnectionType();
        connectionName_->setEnabled(isValidType);
        host_->setEnabled(isValidType);
        port_->setEnabled(isValidType);
        testButton_->setEnabled(isValidType);
        buttonBox_->button(QDialogButtonBox::Save)->setEnabled(isValidType);
    }

    void ConnectionDialog::accept()
    {
        if(validateAndApply()){
            connection_->setConnectionName(common::utils_qt::toStdString(connectionName_->text()));
            hostAndPort h;
            h.first = common::utils_qt::toStdString(host_->text());
            h.second = port_->text().toInt();
            connection_->setHost(h);
            QDialog::accept();
        }
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
}
