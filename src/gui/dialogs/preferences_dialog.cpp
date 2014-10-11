#include "gui/dialogs/preferences_dialog.h"

#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QEvent>

#include "gui/app_style.h"
#include "gui/gui_factory.h"
#include "translations/translations.h"
#include "core/settings_manager.h"
#include "core/servers_manager.h"
#include "common/qt/convert_string.h"

namespace fastoredis
{
    PreferencesDialog::PreferencesDialog(QWidget *parent)
        : QDialog(parent)
    {
        setWindowIcon(GuiFactory::instance().mainWindowIcon());

        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
        setFixedSize(height,width);

        QVBoxLayout *layout = new QVBoxLayout(this);

        QHBoxLayout *langLayout = new QHBoxLayout;
        langLabel_ = new QLabel;
        langLayout->addWidget(langLabel_);
        languagesComboBox_  = new QComboBox();
        languagesComboBox_->addItems(translations::getSupportedLanguages());
        langLayout->addWidget(languagesComboBox_);

        QHBoxLayout *stylesLayout = new QHBoxLayout;
        stylesLabel_ = new QLabel;
        stylesLayout->addWidget(stylesLabel_);

        stylesComboBox_ = new QComboBox();
        stylesComboBox_->addItems(getSupportedStyles());
        defaultViewComboBox_ = new QComboBox;
        std::vector<common::string16> allV = allSupportedViews();
        for(int i=0; i<allV.size(); ++i){
            defaultViewComboBox_->addItem(common::convertFromString16<QString>(allV[i]));
        }
        stylesLayout->addWidget(defaultViewComboBox_);
        stylesLayout->addWidget(stylesComboBox_);

        syncTabs_ = new QCheckBox;
        logDirPath_ = new QLineEdit;
        QHBoxLayout *logLayout = new QHBoxLayout;
        logDirLabel_ = new QLabel;
        logLayout->addWidget(logDirLabel_);
        logLayout->addWidget(logDirPath_);

        layout->addLayout(langLayout);
        layout->addLayout(stylesLayout);
        layout->addWidget(syncTabs_);
        layout->addLayout(logLayout);

        QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Save);
        VERIFY(connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept())));
        VERIFY(connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject())));
        layout->addWidget(buttonBox);
        setLayout(layout);

        syncWithSettings();
        retranslateUi();
    }

    void PreferencesDialog::syncWithSettings()
    {
        languagesComboBox_->setCurrentText(common::convertFromString<QString>(SettingsManager::instance().currentLanguage()));
        stylesComboBox_->setCurrentText(common::convertFromString<QString>(SettingsManager::instance().currentStyle()));
        defaultViewComboBox_->setCurrentText(common::convertFromString16<QString>(common::convertToString16(SettingsManager::instance().defaultView())));
        syncTabs_->setChecked(SettingsManager::instance().syncTabs());
        logDirPath_->setText(common::convertFromString<QString>(SettingsManager::instance().loggingDirectory()));
    }

    void PreferencesDialog::accept()
    {
        QString newLang = translations::applyLanguage(languagesComboBox_->currentText());
        SettingsManager::instance().setCurrentLanguage(common::convertToString(newLang));

        applyStyle(stylesComboBox_->currentText());
        SettingsManager::instance().setCurrentStyle(common::convertToString(stylesComboBox_->currentText()));

        SettingsManager::instance().setDefaultView(common::convertFromString16<fastoredis::supportedViews>(common::convertToString16(defaultViewComboBox_->currentText())));

        ServersManager::instance().setSyncServers(syncTabs_->isChecked());
        SettingsManager::instance().setSyncTabs(syncTabs_->isChecked());

        return QDialog::accept();
    }

    void PreferencesDialog::changeEvent(QEvent *e)
    {
        if(e->type() == QEvent::LanguageChange){
            retranslateUi();
        }
        QDialog::changeEvent(e);
    }

    void PreferencesDialog::retranslateUi()
    {
        setWindowTitle(tr("Preferences "PROJECT_NAME_TITLE));
        langLabel_->setText(tr("Language:"));
        stylesLabel_->setText(tr("Styles:"));
        syncTabs_->setText(tr("Sync tabs"));
        logDirLabel_->setText(tr("Logging directory:"));
    }
}
