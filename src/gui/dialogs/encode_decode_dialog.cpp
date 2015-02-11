#include "gui/dialogs/encode_decode_dialog.h"

#include <QPushButton>
#include <QLabel>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QSplitter>
#include <QToolButton>
#include <QComboBox>
#include <QRadioButton>
#include <QEvent>

#include "gui/gui_factory.h"
#include "gui/fasto_editor.h"

#include "core/iedcoder.h"
#include "common/qt/convert_string.h"

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
        VERIFY(connect(buttonBox, &QDialogButtonBox::rejected, this, &EncodeDecodeDialog::reject));

        QToolButton* decode = new QToolButton;
        decode->setIcon(GuiFactory::instance().executeIcon());
        VERIFY(connect(decode, &QToolButton::clicked, this, &EncodeDecodeDialog::decode));

        decoders_ = new QComboBox;
        std::vector<std::string> sup = supportedEDcoderTypes();
        for(int i = 0; i < sup.size(); ++i){
            decoders_->addItem(common::convertFromString<QString>(sup[i]));
        }

        QHBoxLayout *toolBarLayout = new QHBoxLayout;
        toolBarLayout->setContentsMargins(0, 0, 0, 0);
        toolBarLayout->addWidget(decode);
        toolBarLayout->addWidget(decoders_);

        encodeButton_ = new QRadioButton;
        decodeButton_ = new QRadioButton;
        toolBarLayout->addWidget(encodeButton_);
        toolBarLayout->addWidget(decodeButton_);
        encodeButton_->setChecked(true);

        QSplitter *splitter = new QSplitter;
        splitter->setOrientation(Qt::Horizontal);
        splitter->setHandleWidth(1);
        splitter->setContentsMargins(0, 0, 0, 0);
        toolBarLayout->addWidget(splitter);

        input_ = new FastoEditor;
        output_ = new FastoEditor;

        layout->addWidget(input_);       
        layout->addLayout(toolBarLayout);        
        layout->addWidget(output_);

        setMinimumSize(QSize(width, height));
        setLayout(layout);

        retranslateUi();
    }

    void EncodeDecodeDialog::changeEvent(QEvent *e)
    {
        if(e->type() == QEvent::LanguageChange){
            retranslateUi();
        }

        QWidget::changeEvent(e);
    }

    void EncodeDecodeDialog::decode()
    {
        const QString in = input_->text();
        if(in.isEmpty()){
            return;
        }

        output_->clear();
        QString decoderText = decoders_->currentText();
        std::string sdec = common::convertToString(decoderText);
        IEDcoder* dec = IEDcoder::createEDCoder(sdec);

        if(!dec){
            return;
        }

        const std::string sin = common::convertToString(in);
        std::string out;
        common::ErrorValueSPtr er;
        if(encodeButton_->isChecked()){
            er = dec->encode(sin, out);
        }
        else{
            er = dec->decode(sin, out);
        }

        if(!er){
            output_->setText(common::convertFromString<QString>(out));
        }
    }

    void EncodeDecodeDialog::retranslateUi()
    {
        using namespace translations;
        encodeButton_->setText(trEncode);
        decodeButton_->setText(trDecode);
    }
}
