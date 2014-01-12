#include "gui/dialogs/ConnectionDialog.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QLineEdit>
#include <QEvent>

#include "gui/GuiFactory.h"
#include "common/qt/converter_patterns.h"
#include "common/utils.h"

namespace fastoredis
{
    ConnectionDialog::ConnectionDialog(const IConnectionSettingsBasePtr &connection, QWidget *parent)
        : QDialog(parent),
        connection_(connection)
    {
        using namespace common;
        setWindowIcon(GuiFactory::instance().serverIcon());
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove help button (?)

        connectionName_ = new QLineEdit;
        connectionName_->setText(utils_qt::toQString(connection_->connectionName()));

        typeConnection_ = new QComboBox;
        std::vector<unicode_string> supt = supportedConnectionTypes();
        for(std::vector<unicode_string>::const_iterator it = supt.begin(); it != supt.end(); ++it){
            typeConnection_->addItem(utils_qt::toQString(*it));
        }
        typeConnection_->setCurrentText(utils_qt::toQString(toStdString(connection_->connectionType())));
        VERIFY(connect(typeConnection_, SIGNAL(currentTextChanged(const QString&)), this, SLOT(typeConnectionChange(const QString&))));

        commandLine_ = new QLineEdit;
        commandLine_->setText(utils_qt::toQString(connection_->commandLine()));

        QVBoxLayout *inputLayout = new QVBoxLayout;
        inputLayout->addWidget(connectionName_);
        inputLayout->addWidget(typeConnection_);
        inputLayout->addWidget(commandLine_);

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
        retranslateUi();
    }

    void ConnectionDialog::typeConnectionChange(const QString &value)
    {
        connectionTypes currentType = toConnectionType(common::utils_qt::toStdString(value));
        bool isValidType = currentType != badConnectionType();
        connectionName_->setEnabled(isValidType);
        testButton_->setEnabled(isValidType);
        commandLine_->setEnabled(isValidType);
        buttonBox_->button(QDialogButtonBox::Save)->setEnabled(isValidType);
    }

    void ConnectionDialog::accept()
    {
        if(validateAndApply()){
            connection_->setConnectionName(common::utils_qt::toStdString(connectionName_->text()));
            connection_->setCommandLine(common::utils_qt::toStdString(commandLine_->text()));
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
    }
}
