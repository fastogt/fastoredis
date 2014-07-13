#include "gui/dialogs/preferences_dialog.h"

#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
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
        std::vector<std::string> allV = allSupportedViews();
        for(int i=0; i<allV.size(); ++i){
            defaultViewComboBox_->addItem(common::convertfromString<QString>(allV[i]));
        }
        stylesLayout->addWidget(defaultViewComboBox_);
        stylesLayout->addWidget(stylesComboBox_);

        syncTabs_ = new QCheckBox;

        layout->addLayout(langLayout);
        layout->addLayout(stylesLayout);
        layout->addWidget(syncTabs_);

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
        languagesComboBox_->setCurrentText(common::convertfromString<QString>(SettingsManager::instance().currentLanguage()));
        stylesComboBox_->setCurrentText(common::convertfromString<QString>(SettingsManager::instance().currentStyle()));
        defaultViewComboBox_->setCurrentText(common::convertfromString<QString>(common::convert2string(SettingsManager::instance().defaultView())));
        syncTabs_->setChecked(SettingsManager::instance().syncTabs());
    }

    void PreferencesDialog::accept()
    {
        QString newLang = translations::applyLanguage(languagesComboBox_->currentText());
        SettingsManager::instance().setCurrentLanguage(common::convert2string(newLang));

        applyStyle(stylesComboBox_->currentText());
        SettingsManager::instance().setCurrentStyle(common::convert2string(stylesComboBox_->currentText()));

        SettingsManager::instance().setDefaultView(common::convertfromString<fastoredis::supportedViews>(common::convert2string(defaultViewComboBox_->currentText())));

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
        syncTabs_->setText("Sync tabs");
    }
}
