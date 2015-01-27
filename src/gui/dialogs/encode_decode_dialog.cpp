#include "gui/dialogs/encode_decode_dialog.h"

#include <QPushButton>
#include <QLabel>
#include <QDialogButtonBox>
#include <QVBoxLayout>

#include "gui/gui_factory.h"

#include "translations/global.h"

namespace
{

}

namespace fastoredis
{
    EncodeDecodeDialog::EncodeDecodeDialog(QWidget* parent)
        : QDialog(parent)
    {
        setWindowIcon(GuiFactory::instance().encodeDecodeIcon());

        setWindowTitle(translations::trEncodeDecodeDialog);
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
        QVBoxLayout* layout = new QVBoxLayout;

        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
        QPushButton *closeButton = buttonBox->button(QDialogButtonBox::Close);
        buttonBox->addButton(closeButton, QDialogButtonBox::ButtonRole(QDialogButtonBox::RejectRole | QDialogButtonBox::AcceptRole));
        VERIFY(connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject())));

        setMinimumSize(QSize(width, height));
        setLayout(layout);
    }
}
