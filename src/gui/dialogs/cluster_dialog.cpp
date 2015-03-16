#include "gui/dialogs/cluster_dialog.h"

#include <QDialogButtonBox>
#include <QEvent>
#include <QFileDialog>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QMessageBox>

#include "gui/dialogs/connection_diagnostic_dialog.h"

#include "common/qt/convert_string.h"

#include "gui/gui_factory.h"

#include "translations/global.h"

namespace
{
    const QString defaultNameConnection = "New Cluster Connection";
}

namespace fastoredis
{
    ClusterDialog::ClusterDialog(QWidget* parent, IConnectionSettingsBase* connection)
        : QDialog(parent), connection_(connection)
    {
        using namespace translations;

        setWindowIcon(GuiFactory::instance().serverIcon());
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove help button (?)

        connectionName_ = new QLineEdit;
        QString conName = defaultNameConnection;
        if(connection_){
            conName = common::convertFromString<QString>(connection_->connectionName());
        }
        connectionName_->setText(conName);
        typeConnection_ = new QComboBox;

        for(int i = 0; i < connnectionType.size(); ++i){
            typeConnection_->addItem(common::convertFromString<QString>(connnectionType[i]));
        }

        if(connection_){
            typeConnection_->setCurrentText(common::convertFromString<QString>(common::convertToString(connection_->connectionType())));
        }

        VERIFY(connect(typeConnection_, &QComboBox::currentTextChanged, this, &ClusterDialog::typeConnectionChange));

        logging_ = new QCheckBox;
        if(connection_){
            logging_->setChecked(connection_->loggingEnabled());
        }
        else{
            logging_->setChecked(false);
        }

        QVBoxLayout *inputLayout = new QVBoxLayout;
        inputLayout->addWidget(connectionName_);
        inputLayout->addWidget(typeConnection_);
        inputLayout->addWidget(logging_);

        QPushButton *testButton = new QPushButton("&Test");
        testButton->setIcon(GuiFactory::instance().messageBoxInformationIcon());
        VERIFY(connect(testButton, &QPushButton::clicked, this, &ClusterDialog::testConnection));

        QHBoxLayout *bottomLayout = new QHBoxLayout;
        bottomLayout->addWidget(testButton, 1, Qt::AlignLeft);
        buttonBox_ = new QDialogButtonBox(this);
        buttonBox_->setOrientation(Qt::Horizontal);
        buttonBox_->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Save);
        VERIFY(connect(buttonBox_, &QDialogButtonBox::accepted, this, &ClusterDialog::accept));
        VERIFY(connect(buttonBox_, &QDialogButtonBox::rejected, this, &ClusterDialog::reject));
        bottomLayout->addWidget(buttonBox_);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addLayout(inputLayout);
        mainLayout->addLayout(bottomLayout);
        mainLayout->setSizeConstraint(QLayout::SetFixedSize);
        setLayout(mainLayout);

        //update controls
        typeConnectionChange(typeConnection_->currentText());
        retranslateUi();
    }

    IConnectionSettingsBaseSPtr ClusterDialog::connection() const
    {
        return connection_;
    }

    void ClusterDialog::accept()
    {
        if(validateAndApply()){
            QDialog::accept();
        }
    }

    void ClusterDialog::typeConnectionChange(const QString& value)
    {
        connectionTypes currentType = common::convertFromString<connectionTypes>(common::convertToString(value));
        bool isValidType = currentType == REDIS;
        connectionName_->setEnabled(isValidType);
        buttonBox_->button(QDialogButtonBox::Save)->setEnabled(isValidType);
    }

    void ClusterDialog::testConnection()
    {
        if(validateAndApply()){
            ConnectionDiagnosticDialog diag(this, connection_);
            diag.exec();
        }
    }

    void ClusterDialog::changeEvent(QEvent* e)
    {
        if(e->type() == QEvent::LanguageChange){
            retranslateUi();
        }
        QDialog::changeEvent(e);
    }

    void ClusterDialog::retranslateUi()
    {
        logging_->setText(tr("Logging enabled"));
    }

    bool ClusterDialog::validateAndApply()
    {
        connectionTypes currentType = common::convertFromString<connectionTypes>(common::convertToString(typeConnection_->currentText()));
        bool isValidType = currentType != DBUNKNOWN;
        if(isValidType){
            std::string conName = common::convertToString(connectionName_->text());
            IConnectionSettingsBase* newConnection = IConnectionSettingsBase::createFromType(currentType, conName);
            if(newConnection){
                connection_.reset(newConnection);
                connection_->setLoggingEnabled(logging_->isChecked());
            }
            return true;
        }
        else{
            using namespace translations;
            QMessageBox::critical(this, trError, QObject::tr("Invalid database type!"));
            return false;
        }
    }
}
