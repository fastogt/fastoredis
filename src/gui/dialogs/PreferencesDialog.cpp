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
#include "core/SettingsManager.h"
#include "common/qt_helper/converter_patterns.h"

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

        QHBoxLayout *stylesLayout = new QHBoxLayout(this);
        QLabel *stylesLabel = new QLabel("Styles:");
        stylesLayout->addWidget(stylesLabel);
        _stylesComboBox = new QComboBox();
        _stylesComboBox->addItems(detail::getSupportedStyles());
        _defaultViewComboBox = new QComboBox;
        std::vector<std::string> allV = detail::allSupportedViews();
        for(int i=0; i<allV.size(); ++i){
            _defaultViewComboBox->addItem(common::utils_qt::toQString(allV[i]));
        }
        stylesLayout->addWidget(_defaultViewComboBox);
        stylesLayout->addWidget(_stylesComboBox);
        layout->addLayout(stylesLayout);   

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
        _stylesComboBox->setCurrentText(common::utils_qt::toQString(SettingsManager::instance().currentStyle()));
        _defaultViewComboBox->setCurrentText(common::utils_qt::toQString(detail::toStdString(SettingsManager::instance().defaultView())));
    }

    void PreferencesDialog::accept()
    {
        detail::applyStyle(_stylesComboBox->currentText());
        SettingsManager::instance().setCurrentStyle(common::utils_qt::toStdString(_stylesComboBox->currentText()));

        SettingsManager::instance().setDefaultView(detail::toSupportedViews(common::utils_qt::toStdString(_defaultViewComboBox->currentText())));

        return BaseClass::accept();
    }
}
