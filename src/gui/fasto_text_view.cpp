#include "gui/fasto_text_view.h"

#include <QToolButton>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QMessageBox>
#include <QKeyEvent>

#include "common/macros.h"

#include "translations/global.h"

#include "gui/gui_factory.h"
#include "gui/fasto_editor.h"

namespace fastoredis
{
    FastoTextView::FastoTextView(const QString &delemitr, QWidget* parent)
        : QWidget(parent)
    {
        findPanel_ = new QFrame(this);
        findLine_ = new QLineEdit(this);
        close_ = new QToolButton(this);
        next_ = new QPushButton(this);
        prev_ = new QPushButton(this);
        caseSensitive_ = new QCheckBox(this);

        close_->setIcon(GuiFactory::instance().close16Icon());
        close_->setToolButtonStyle(Qt::ToolButtonIconOnly);
        close_->setIconSize(QSize(16, 16));
        findLine_->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);

        QHBoxLayout *layout = new QHBoxLayout;
        layout->setContentsMargins(6, 0, 6, 0);
        layout->setSpacing(4);
        layout->addWidget(close_);
        layout->addWidget(findLine_);
        layout->addWidget(next_);
        layout->addWidget(prev_);
        layout->addWidget(caseSensitive_);

        findPanel_->setFixedHeight(HeightFindPanel);
        findPanel_->setLayout(layout);

        QVBoxLayout *mainL = new QVBoxLayout;
        editor_ = new FastoEditorOutput(delemitr, this);
        editor_->installEventFilter(this);

        jsonRadioButton_ = new QRadioButton;
        csvRadioButton_ = new QRadioButton;
        rawRadioButton_ = new QRadioButton;
        hexRadioButton_ = new QRadioButton;
        msgPackRadioButton_ = new QRadioButton;
        gzipRadioButton_ = new QRadioButton;

        VERIFY(connect(jsonRadioButton_, &QRadioButton::toggled, this, &FastoTextView::viewChanged));
        VERIFY(connect(csvRadioButton_, &QRadioButton::toggled, this, &FastoTextView::viewChanged));
        VERIFY(connect(rawRadioButton_, &QRadioButton::toggled, this, &FastoTextView::viewChanged));
        VERIFY(connect(hexRadioButton_, &QRadioButton::toggled, this, &FastoTextView::viewChanged));
        VERIFY(connect(msgPackRadioButton_, &QRadioButton::toggled, this, &FastoTextView::viewChanged));
        VERIFY(connect(gzipRadioButton_, &QRadioButton::toggled, this, &FastoTextView::viewChanged));

        QHBoxLayout* radLaout = new QHBoxLayout;
        radLaout->addWidget(jsonRadioButton_);
        radLaout->addWidget(csvRadioButton_);
        radLaout->addWidget(rawRadioButton_);
        radLaout->addWidget(hexRadioButton_);
        radLaout->addWidget(msgPackRadioButton_);
        radLaout->addWidget(gzipRadioButton_);

        mainL->addLayout(radLaout);
        mainL->addWidget(editor_);
        mainL->addWidget(findPanel_);
        setLayout(mainL);

        findPanel_->hide();

        VERIFY(connect(close_, &QToolButton::clicked, findPanel_, &QFrame::hide));
        VERIFY(connect(next_, &QPushButton::clicked, this, &FastoTextView::goToNextElement));
        VERIFY(connect(prev_, &QPushButton::clicked, this, &FastoTextView::goToPrevElement));

        jsonRadioButton_->setChecked(true);
        retranslateUi();
    }

    void FastoTextView::setModel(QAbstractItemModel* model)
    {
        editor_->setModel(model);
    }

    void FastoTextView::viewChanged(bool checked)
    {
        if (!checked){
            return;
        }

        if(jsonRadioButton_->isChecked()){
            editor_->viewChanged(JSON);
            return;
        }

        if(csvRadioButton_->isChecked()){
            editor_->viewChanged(CSV);
            return;
        }

        if(rawRadioButton_->isChecked()){
            editor_->viewChanged(RAW);
            return;
        }

        if(hexRadioButton_->isChecked()){
            editor_->viewChanged(HEX);
            return;
        }

        if(msgPackRadioButton_->isChecked()){
            editor_->viewChanged(MSGPACK);
            return;
        }

        if(gzipRadioButton_->isChecked()){
            editor_->viewChanged(GZIP);
            return;
        }
    }

    void FastoTextView::setReadOnly(bool readOnly)
    {
        editor_->setReadOnly(readOnly);
    }

    void FastoTextView::changeEvent(QEvent *e)
    {
        if(e->type() == QEvent::LanguageChange){
            retranslateUi();
        }

        QWidget::changeEvent(e);
    }

    bool FastoTextView::eventFilter(QObject* object, QEvent* event)
    {
        if (object == editor_) {
            if (event->type() == QEvent::KeyPress) {
                QKeyEvent *keyEvent = (QKeyEvent *)event;
                if (((keyEvent->modifiers() & Qt::ControlModifier) && keyEvent->key() == Qt::Key_F)) {
                    findPanel_->show();
                    findLine_->setFocus();
                    //findPanel_->selectAll();
                    keyEvent->accept();
                    return true;
                }
            }
        }

        return QWidget::eventFilter(object, event);
    }

    void FastoTextView::keyPressEvent(QKeyEvent *keyEvent)
    {
        bool isFocusScin = editor_->isActiveWindow();
        bool isShowFind = findPanel_->isVisible();

        if (keyEvent->key() == Qt::Key_Escape && isFocusScin && isShowFind) {
            findPanel_->hide();
            editor_->setFocus();
            keyEvent->accept();
        }
        else if (keyEvent->key() == Qt::Key_Return && (keyEvent->modifiers() & Qt::ShiftModifier) && isFocusScin && isShowFind) {
            goToPrevElement();
        }
        else if (keyEvent->key() == Qt::Key_Return && isFocusScin && isShowFind) {
            goToNextElement();
        }
        else{
            QWidget::keyPressEvent(keyEvent);
        }
    }

    void FastoTextView::goToNextElement()
    {
        findElement(true);
    }

    void FastoTextView::goToPrevElement()
    {
        findElement(false);
    }

    void FastoTextView::findElement(bool forward)
    {
        const QString &text = findLine_->text();
        if (!text.isEmpty()) {
            bool re = false;
            bool wo = false;
            bool looped = true;
            int index = 0;
            int line = 0;
            editor_->getCursorPosition(&line, &index);

            if (!forward){
                index -= editor_->selectedText().length();
            }

            editor_->setCursorPosition(line, 0);
            bool isFounded = editor_->findFirst(text, re, caseSensitive_->checkState() == Qt::Checked, wo, looped, forward, line, index);

            if (isFounded) {
                editor_->ensureCursorVisible();
            }
            else {
                QMessageBox::warning(this, translations::trSearch, tr("The specified text was not found."));
            }
        }
    }

    void FastoTextView::retranslateUi()
    {
        using namespace translations;
        next_->setText(trNext);
        prev_->setText(trPrevious);
        caseSensitive_->setText(trMatchCase);

        jsonRadioButton_->setText(trJson);
        csvRadioButton_->setText(trCsv);
        rawRadioButton_->setText(trRawText);
        hexRadioButton_->setText(trHex);
        msgPackRadioButton_->setText(trMsgPack);
        gzipRadioButton_->setText(trGzip);
    }
}
