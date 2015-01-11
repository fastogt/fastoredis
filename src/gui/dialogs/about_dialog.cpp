#include "gui/dialogs/about_dialog.h"

#include <QPushButton>
#include <QLabel>
#include <QDialogButtonBox>
#include <QGridLayout>

#include "gui/gui_factory.h"

namespace
{
    const QString description = QObject::tr(
#ifdef RELEASE_CANDIDATE
        "<h3>" PROJECT_NAME_TITLE " " PROJECT_VERSION "<br/>Revision:" PROJECT_GIT_VERSION "</h3>"
#else
        "<h3>" PROJECT_NAME_TITLE " " PROJECT_VERSION " beta" STRINGIZE(PROJECT_VERSION_TWEAK) "<br/>Revision:" PROJECT_GIT_VERSION "</h3>"
#endif
        "Shell-centric Redis management tool."
        "<br/>"
        "<br/>"
        "Visit " PROJECT_NAME_TITLE " website: <a href=\"http://" PROJECT_DOMAIN "\">" PROJECT_DOMAIN "</a> <br/>"
        "<br/>"
        "<a href=\"https://" PROJECT_GITHUB_FORK "\">Fork</a> project or <a href=\"https://" PROJECT_GITHUB_ISSUES "\">submit</a> issues/proposals on GitHub.  <br/>"
        "<br/>"
        "Copyright 2014 <a href=\"http://" PROJECT_COMPANYNAME_DOMAIN "\">" PROJECT_COMPANYNAME "</a>. All rights reserved.<br/>"
        "<br/>"
        "The program is provided AS IS with NO WARRANTY OF ANY KIND, "
        "INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A "
        "PARTICULAR PURPOSE.<br/>");
}

namespace fastoredis
{
    AboutDialog::AboutDialog(QWidget* parent)
        : QDialog(parent)
    {
        setWindowIcon(GuiFactory::instance().mainWindowIcon());

        setWindowTitle(PROJECT_NAME_TITLE);
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
        QGridLayout *layout = new QGridLayout(this);
        layout->setSizeConstraint(QLayout::SetFixedSize);

        QLabel *copyRightLabel = new QLabel(description);
        copyRightLabel->setWordWrap(true);
        copyRightLabel->setOpenExternalLinks(true);
        copyRightLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
        QPushButton *closeButton = buttonBox->button(QDialogButtonBox::Close);
        buttonBox->addButton(closeButton, QDialogButtonBox::ButtonRole(QDialogButtonBox::RejectRole | QDialogButtonBox::AcceptRole));
        VERIFY(connect(buttonBox , SIGNAL(rejected()), this, SLOT(reject())));

        QIcon icon = GuiFactory::instance().mainWindowIcon();
        QPixmap iconPixmap = icon.pixmap(48, 48);

        QLabel *logoLabel = new QLabel;
        logoLabel->setPixmap(iconPixmap);
        layout->addWidget(logoLabel , 0, 0, 1, 1);
        layout->addWidget(copyRightLabel, 0, 1, 4, 4);
        layout->addWidget(buttonBox, 4, 0, 1, 5);
    }
}
