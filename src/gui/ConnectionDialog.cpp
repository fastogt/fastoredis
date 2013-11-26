#include "gui/ConnectionDialog.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QDialogButtonBox>

#include "gui/GuiFactory.h"
#include "common/macros.h"

namespace fastoredis
{
    ConnectionDialog::ConnectionDialog(IConnectionSettingsBase *connection, QWidget *parent)
        : QDialog(parent),
        connection_(connection)
    {
        setWindowTitle("Connection Settings");
        setWindowIcon(GuiFactory::instance().serverIcon());
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove help button (?)

        QPushButton *testButton = new QPushButton("&Test");
        testButton->setIcon(GuiFactory::instance().messageBoxInformationIcon());
        VERIFY(connect(testButton, SIGNAL(clicked()), this, SLOT(testConnection())));

        QHBoxLayout *bottomLayout = new QHBoxLayout;
        bottomLayout->addWidget(testButton, 1, Qt::AlignLeft);
        QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Save);
        VERIFY(connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept())));
        VERIFY(connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject())));
        bottomLayout->addWidget(buttonBox);


        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addLayout(bottomLayout);
        setLayout(mainLayout);
    }

    void ConnectionDialog::accept()
    {
        if(validateAndApply()){
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
