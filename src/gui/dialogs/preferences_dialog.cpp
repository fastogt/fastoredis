#include "gui/dialogs/preferences_dialog.h"

#include <QDialogButtonBox>
#include <QLineEdit>
#include <QFileDialog>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>

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

//      ui settings
        generalBox_ = new QGroupBox;

        QHBoxLayout *styleswLayout = new QHBoxLayout;
        stylesLabel_ = new QLabel;
        stylesComboBox_ = new QComboBox;
        stylesComboBox_->addItems(getSupportedStyles());
        styleswLayout->addWidget(stylesLabel_);
        styleswLayout->addWidget(stylesComboBox_);

        QHBoxLayout *langLayout = new QHBoxLayout;
        langLabel_ = new QLabel;
        langLayout->addWidget(langLabel_);
        languagesComboBox_  = new QComboBox;
        languagesComboBox_->addItems(translations::getSupportedLanguages());
        langLayout->addWidget(languagesComboBox_);

        QVBoxLayout *generalLayout = new QVBoxLayout;
        autoCheckUpdates_ = new QCheckBox;
        generalLayout->addWidget(autoCheckUpdates_);
        generalLayout->addLayout(styleswLayout);
        generalLayout->addLayout(langLayout);

        generalBox_->setLayout(generalLayout);

//      servers settings
        serverSettingsBox_ = new QGroupBox;

        QHBoxLayout* defaultViewLayaut = new QHBoxLayout;
        defaultViewLabel_ = new QLabel;
        defaultViewComboBox_ = new QComboBox;
        std::vector<std::string> allV = allSupportedViews();
        for(int i = 0; i < allV.size(); ++i){
            defaultViewComboBox_->addItem(common::convertFromString<QString>(allV[i]));
        }
        defaultViewLayaut->addWidget(defaultViewLabel_);
        defaultViewLayaut->addWidget(defaultViewComboBox_);

        syncTabs_ = new QCheckBox;
        logDirPath_ = new QLineEdit;
        QHBoxLayout *logLayout = new QHBoxLayout;
        logDirLabel_ = new QLabel;
        logLayout->addWidget(logDirLabel_);
        logLayout->addWidget(logDirPath_);

        QVBoxLayout *serverSettingsLayout = new QVBoxLayout;
        serverSettingsLayout->addLayout(defaultViewLayaut);
        serverSettingsLayout->addWidget(syncTabs_);
        serverSettingsLayout->addLayout(logLayout);
        serverSettingsBox_->setLayout(serverSettingsLayout);

//      main layout
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(generalBox_);
        layout->addWidget(serverSettingsBox_);

        QDialogButtonBox *buttonBox = new QDialogButtonBox;
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Save);
        VERIFY(connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept())));
        VERIFY(connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject())));
        layout->addWidget(buttonBox);
        setMinimumSize(QSize(min_width, min_height));
        setLayout(layout);

        syncWithSettings();
        retranslateUi();
    }

    void PreferencesDialog::syncWithSettings()
    {
        autoCheckUpdates_->setChecked(SettingsManager::instance().autoCheckUpdates());
        languagesComboBox_->setCurrentText(SettingsManager::instance().currentLanguage());
        stylesComboBox_->setCurrentText(SettingsManager::instance().currentStyle());
        defaultViewComboBox_->setCurrentText(common::convertFromString<QString>(common::convertToString(SettingsManager::instance().defaultView())));
        syncTabs_->setChecked(SettingsManager::instance().syncTabs());
        logDirPath_->setText(SettingsManager::instance().loggingDirectory());
    }

    void PreferencesDialog::accept()
    {
        SettingsManager::instance().setAutoCheckUpdates(autoCheckUpdates_->isChecked());

        QString newLang = translations::applyLanguage(languagesComboBox_->currentText());
        SettingsManager::instance().setCurrentLanguage(newLang);

        applyStyle(stylesComboBox_->currentText());
        SettingsManager::instance().setCurrentStyle(stylesComboBox_->currentText());

        const std::string defCombo = common::convertToString(defaultViewComboBox_->currentText());
        const fastoredis::supportedViews v = common::convertFromString<fastoredis::supportedViews>(defCombo);
        SettingsManager::instance().setDefaultView(v);

        ServersManager::instance().setSyncServers(syncTabs_->isChecked());
        SettingsManager::instance().setSyncTabs(syncTabs_->isChecked());
        SettingsManager::instance().setLoggingDirectory(logDirPath_->text());

        return QDialog::accept();
    }

    void PreferencesDialog::changeEvent(QEvent* e)
    {
        if(e->type() == QEvent::LanguageChange){
            retranslateUi();
        }
        QDialog::changeEvent(e);
    }

    void PreferencesDialog::retranslateUi()
    {
        setWindowTitle(tr("Preferences " PROJECT_NAME_TITLE));

        generalBox_->setTitle(tr("General settings"));
        autoCheckUpdates_->setText(tr("Automatically check for updates"));
        langLabel_->setText(tr("Language:"));
        stylesLabel_->setText(tr("Supported UI styles:"));

        serverSettingsBox_->setTitle(tr("Servers global settings"));
        defaultViewLabel_->setText(tr("Default views:"));
        syncTabs_->setText(tr("Sync tabs"));
        logDirLabel_->setText(tr("Logging directory:"));
    }
}
