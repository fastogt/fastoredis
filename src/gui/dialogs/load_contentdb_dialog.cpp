#include "gui/dialogs/load_contentdb_dialog.h"

#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QMessageBox>
#include <QLabel>

#include "gui/gui_factory.h"
#include "translations/global.h"

namespace fastoredis
{
    LoadContentDbDialog::LoadContentDbDialog(const QString &title, connectionTypes type, QWidget* parent)
        : QDialog(parent), type_(type)
    {
        setWindowIcon(GuiFactory::instance().icon(type_));
        setWindowTitle(title);
        QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
        VERIFY(connect(buttonBox, &QDialogButtonBox::accepted, this, &LoadContentDbDialog::accept));
        VERIFY(connect(buttonBox, &QDialogButtonBox::rejected, this, &LoadContentDbDialog::reject));

        QVBoxLayout *mainLayout = new QVBoxLayout;

        QHBoxLayout* countLayout = new QHBoxLayout;
        countLayout->addWidget(new QLabel(tr("Keys count:")));
        countTextEdit_ = new QLineEdit;
        countTextEdit_->setFixedWidth(80);
        QRegExp rx("\\d+");//(0-65554)
        countTextEdit_->setValidator(new QRegExpValidator(rx, this));
        countTextEdit_->setText(QString::number(100));
        countLayout->addWidget(countTextEdit_);
        mainLayout->addLayout(countLayout);

        QHBoxLayout* patternLayout = new QHBoxLayout;
        patternLayout->addWidget(new QLabel(tr("Pattern:")));
        patternEdit_ = new QLineEdit;
        patternEdit_->setFixedWidth(80);
        patternEdit_->setText("*");
        patternLayout->addWidget(patternEdit_);
        mainLayout->addLayout(patternLayout);

        mainLayout->addWidget(buttonBox);
        setLayout(mainLayout);
    }

    void LoadContentDbDialog::accept()
    {
        using namespace translations;
        QString pattern = patternEdit_->text();
        if(pattern.isEmpty()){
            QMessageBox::warning(this, trError, QObject::tr("Invalid pattern!"));
            countTextEdit_->setFocus();
            return;
        }

        uint32_t count = countTextEdit_->text().toUInt();
        if(count == 0){
            QMessageBox::warning(this, trError, QObject::tr("Invalid keys count!"));
            countTextEdit_->setFocus();
            return;
        }

        QDialog::accept();
    }

    uint32_t LoadContentDbDialog::count() const
    {
        return countTextEdit_->text().toUInt();
    }

    QString LoadContentDbDialog::pattern() const
    {
        return patternEdit_->text();
    }
}
