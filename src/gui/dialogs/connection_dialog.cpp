#include "gui/dialogs/connection_dialog.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QLineEdit>
#include <QEvent>

#include "gui/gui_factory.h"
#include "common/qt/convert_string.h"
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
        connectionName_->setText(convertfromString<QString>(connection_->connectionName()));

        typeConnection_ = new QComboBox;
        std::vector<unicode_string> supt = supportedConnectionTypes();
        for(std::vector<unicode_string>::const_iterator it = supt.begin(); it != supt.end(); ++it){
            typeConnection_->addItem(convertfromString<QString>(*it));
        }
        typeConnection_->setCurrentText(convertfromString<QString>(common::convert2string(connection_->connectionType())));
        VERIFY(connect(typeConnection_, SIGNAL(currentTextChanged(const QString&)), this, SLOT(typeConnectionChange(const QString&))));

        commandLine_ = new QLineEdit;
        commandLine_->setText(convertfromString<QString>(connection_->commandLine()));

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
        connectionTypes currentType = common::convertfromString<connectionTypes>(common::convert2string(value));
        bool isValidType = currentType != badConnectionType();
        connectionName_->setEnabled(isValidType);
        testButton_->setEnabled(isValidType);
        commandLine_->setEnabled(isValidType);
        buttonBox_->button(QDialogButtonBox::Save)->setEnabled(isValidType);
    }

    void ConnectionDialog::accept()
    {
        if(validateAndApply()){
            connection_->setConnectionName(common::convert2string(connectionName_->text()));
            connection_->setCommandLine(common::convert2string(commandLine_->text()));
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
