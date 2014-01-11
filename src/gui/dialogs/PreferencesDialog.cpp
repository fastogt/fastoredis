#include "gui/dialogs/PreferencesDialog.h"

#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>

#include "gui/AppStyle.h"
#include "gui/GuiFactory.h"
#include "translations/Translations.h"
#include "core/SettingsManager.h"
#include "core/ServersManager.h"
#include "common/qt/converter_patterns.h"

namespace fastoredis
{
    PreferencesDialog::PreferencesDialog(QWidget *parent)
        : BaseClass(parent)
    {
        setWindowIcon(GuiFactory::instance().mainWindowIcon());

        setWindowTitle("Preferences "PROJECT_NAME_TITLE);
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
        setFixedSize(height,width);

        QVBoxLayout *layout = new QVBoxLayout(this);

        QHBoxLayout *langLayout = new QHBoxLayout;
        QLabel *langLabel = new QLabel("Language:");
        langLayout->addWidget(langLabel);
        _languagesComboBox  = new QComboBox();
        _languagesComboBox->addItems(translations::getSupportedLanguages());
        langLayout->addWidget(_languagesComboBox);

        QHBoxLayout *stylesLayout = new QHBoxLayout;
        QLabel *stylesLabel = new QLabel("Styles:");
        stylesLayout->addWidget(stylesLabel);

        _stylesComboBox = new QComboBox();
        _stylesComboBox->addItems(getSupportedStyles());
        _defaultViewComboBox = new QComboBox;
        std::vector<std::string> allV = allSupportedViews();
        for(int i=0; i<allV.size(); ++i){
            _defaultViewComboBox->addItem(common::utils_qt::toQString(allV[i]));
        }
        stylesLayout->addWidget(_defaultViewComboBox);
        stylesLayout->addWidget(_stylesComboBox);

        _syncTabs = new QCheckBox;
        _syncTabs->setText("Sync tabs");

        layout->addLayout(langLayout);
        layout->addLayout(stylesLayout);
        layout->addWidget(_syncTabs);

        QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Save);
        VERIFY(connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept())));
        VERIFY(connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject())));
        layout->addWidget(buttonBox);
        setLayout(layout);

        syncWithSettings();
    }

    void PreferencesDialog::syncWithSettings()
    {
        _languagesComboBox->setCurrentText(common::utils_qt::toQString(SettingsManager::instance().currentLanguage()));
        _stylesComboBox->setCurrentText(common::utils_qt::toQString(SettingsManager::instance().currentStyle()));
        _defaultViewComboBox->setCurrentText(common::utils_qt::toQString(toStdString(SettingsManager::instance().defaultView())));
        _syncTabs->setChecked(SettingsManager::instance().syncTabs());
    }

    void PreferencesDialog::accept()
    {
        QString newLang = translations::applyLanguage(_languagesComboBox->currentText());
        SettingsManager::instance().setCurrentLanguage(common::utils_qt::toStdString(newLang));

        applyStyle(_stylesComboBox->currentText());
        SettingsManager::instance().setCurrentStyle(common::utils_qt::toStdString(_stylesComboBox->currentText()));

        SettingsManager::instance().setDefaultView(toSupportedViews(common::utils_qt::toStdString(_defaultViewComboBox->currentText())));

        ServersManager::instance().setSyncServers(_syncTabs->isChecked());
        SettingsManager::instance().setSyncTabs(_syncTabs->isChecked());

        return BaseClass::accept();
    }
}
