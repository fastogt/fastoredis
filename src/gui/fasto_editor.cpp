#include "gui/fasto_editor.h"

#include <QKeyEvent>
#include <QAbstractItemModel>
#include <QVBoxLayout>
#include <QRadioButton>

#include "common/qt/convert_string.h"
#include "common/qt/utils_qt.h"

#include "gui/fasto_common_item.h"
#include "gui/gui_factory.h"

namespace
{
    int getNumberOfDigits(int x)
    {
        if (x < 0) return getNumberOfDigits(-x) + 1;

        if (x >= 10000) {
            if (x >= 10000000) {
                if (x >= 100000000) {
                    if (x >= 1000000000)
                        return 10;
                    return 9;
                }
                return 8;
            }
            if (x >= 100000) {
                if (x >= 1000000)
                    return 7;
                return 6;
            }
            return 5;
        }
        if (x >= 100) {
            if (x >= 1000)
                return 4;
            return 3;
        }
        if (x >= 10)
            return 2;
        return 1;
    }
}

namespace fastoredis
{
    FastoEditor::FastoEditor(QWidget *parent)
        : QsciScintilla(parent), lineNumberMarginWidth_(0), model_(NULL), isJsonChecked_(true)
    {
        setAutoIndent(true);
        setIndentationsUseTabs(false);
        setIndentationWidth(indentationWidth);
        setUtf8(true);
        setMarginWidth(1, 0);
        setContentsMargins(0, 0, 0, 0);
        setViewportMargins(3, 3, 3, 3);
        QFont ourFont = GuiFactory::instance().font();
        setMarginsFont(ourFont);
        setMarginLineNumbers(0, true);

        SendScintilla(QsciScintilla::SCI_STYLESETFONT, 1, ourFont.family().data());
        SendScintilla(QsciScintilla::SCI_SETHSCROLLBAR, 0);

        setWrapMode((QsciScintilla::WrapMode)QsciScintilla::SC_WRAP_NONE);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        VERIFY(connect(this, SIGNAL(linesChanged()), this, SLOT(updateLineNumbersMarginWidth())));
    }

    void FastoEditor::setModel(QAbstractItemModel *model)
    {
        if (model == model_){
            return;
        }

        if(model_){
            VERIFY(disconnect(model_, SIGNAL(destroyed()), this, SLOT(modelDestroyed())));
            VERIFY(disconnect(model_, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(dataChanged(QModelIndex,QModelIndex))));
            VERIFY(disconnect(model_, SIGNAL(headerDataChanged(Qt::Orientation,int,int)), this, SLOT(headerDataChanged())));
            VERIFY(disconnect(model_, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(rowsInserted(QModelIndex,int,int))));
            VERIFY(disconnect(model_, SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)), this, SLOT(rowsAboutToBeRemoved(QModelIndex,int,int))));
            VERIFY(disconnect(model_, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(rowsRemoved(QModelIndex,int,int))));
            VERIFY(disconnect(model_, SIGNAL(columnsAboutToBeRemoved(QModelIndex,int,int)), this, SLOT(columnsAboutToBeRemoved(QModelIndex,int,int))));
            VERIFY(disconnect(model_, SIGNAL(columnsRemoved(QModelIndex,int,int)), this, SLOT(columnsRemoved(QModelIndex,int,int))));
            VERIFY(disconnect(model_, SIGNAL(columnsInserted(QModelIndex,int,int)), this, SLOT(columnsInserted(QModelIndex,int,int))));
            VERIFY(disconnect(model_, SIGNAL(modelReset()), this, SLOT(reset())));
            VERIFY(disconnect(model_, SIGNAL(layoutChanged()), this, SLOT(layoutChanged())));
        }

        model_ = model;

        // These asserts do basic sanity checking of the model
        Q_ASSERT_X(model_->index(0,0) == model_->index(0,0),
                   "QAbstractItemView::setModel",
                   "A model should return the exact same index "
                   "(including its internal id/pointer) when asked for it twice in a row.");
        Q_ASSERT_X(model_->index(0,0).parent() == QModelIndex(),
                   "QAbstractItemView::setModel",
                   "The parent of a top level index should be invalid");

        if (model_) {
            VERIFY(connect(model_, SIGNAL(destroyed()), this, SLOT(modelDestroyed())));
            VERIFY(connect(model_, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(dataChanged(QModelIndex,QModelIndex))));
            VERIFY(connect(model_, SIGNAL(headerDataChanged(Qt::Orientation,int,int)), this, SLOT(headerDataChanged())));
            VERIFY(connect(model_, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(rowsInserted(QModelIndex,int,int))));
            VERIFY(connect(model_, SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)), this, SLOT(rowsAboutToBeRemoved(QModelIndex,int,int))));
            VERIFY(connect(model_, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(rowsRemoved(QModelIndex,int,int))));
            VERIFY(connect(model_, SIGNAL(columnsAboutToBeRemoved(QModelIndex,int,int)), this, SLOT(columnsAboutToBeRemoved(QModelIndex,int,int))));
            VERIFY(connect(model_, SIGNAL(columnsRemoved(QModelIndex,int,int)), this, SLOT(columnsRemoved(QModelIndex,int,int))));
            VERIFY(connect(model_, SIGNAL(columnsInserted(QModelIndex,int,int)),this, SLOT(columnsInserted(QModelIndex,int,int))));
            VERIFY(connect(model_, SIGNAL(modelReset()), this, SLOT(reset())));
            VERIFY(connect(model_, SIGNAL(layoutChanged()), this, SLOT(layoutChanged())));
        }

        reset();
    }

    void FastoEditor::modelDestroyed()
    {

    }

    void FastoEditor::dataChanged(QModelIndex first, QModelIndex last)
    {

    }

    void FastoEditor::headerDataChanged()
    {

    }

    void FastoEditor::rowsInserted(QModelIndex index, int r, int c)
    {
        layoutChanged();
    }

    void FastoEditor::rowsAboutToBeRemoved(QModelIndex index, int r, int c)
    {

    }

    void FastoEditor::rowsRemoved(QModelIndex index, int r, int c)
    {

    }

    void FastoEditor::columnsAboutToBeRemoved(QModelIndex index, int r, int c)
    {

    }

    void FastoEditor::columnsRemoved(QModelIndex index, int r, int c)
    {

    }

    void FastoEditor::columnsInserted(QModelIndex index, int r, int c)
    {

    }

    void FastoEditor::reset()
    {
        layoutChanged();
    }

    void FastoEditor::viewChanged(bool isJson)
    {
        isJsonChecked_ = isJson;
        layoutChanged();
    }

    void FastoEditor::layoutChanged()
    {
        clear();
        if(!model_){
            return;
        }

        QModelIndex index = model_->index(0, 0);
        if(!index.isValid()){
            return;
        }

        FastoCommonItem* child = common::utils_qt::item<FastoCommonItem*>(index);
        if(!child){
            return;
        }

        FastoCommonItem* root = dynamic_cast<FastoCommonItem*>(child->parent());
        if(!root){
            return;
        }

        QString result;
        for(int i = 0; i < root->childrenCount(); ++i){
            if(isJsonChecked_){
                QString json = toJson(dynamic_cast<FastoCommonItem*>(root->child(i)));
                result += common::escapedText(json);
            }
            else{
                QString raw = toRaw(dynamic_cast<FastoCommonItem*>(root->child(i)));
                result += common::escapedText(raw);
            }
        }

        setText(result);
    }

    void FastoEditor::keyPressEvent(QKeyEvent *keyEvent)
    {
        if (keyEvent->key() == Qt::Key_F11) {
            keyEvent->ignore();
            showOrHideLinesNumbers();
            return;
        }

        return QsciScintilla::keyPressEvent(keyEvent);
    }

    void FastoEditor::showOrHideLinesNumbers()
    {
        updateLineNumbersMarginWidth();
        if (!lineNumberMarginWidth()) {
            setMarginWidth(0, lineNumberMarginWidth_);
        }
        else {
            setMarginWidth(0, 0);
        }
    }

    int FastoEditor::lineNumberMarginWidth() const
    {
        return marginWidth(0);
    }

    int FastoEditor::textWidth(int style, const QString &text)
    {
        const char *byteArray = (text.toUtf8()).constData();
        return SendScintilla(SCI_TEXTWIDTH, style, byteArray);
    }

    void FastoEditor::updateLineNumbersMarginWidth()
    {
        int numberOfDigits = getNumberOfDigits(lines());

        lineNumberMarginWidth_ = numberOfDigits * textWidth(STYLE_LINENUMBER, "0") + rowNumberWidth;

        // If line numbers margin already displayed, update its width
        if (lineNumberMarginWidth()) {
            setMarginWidth(0, lineNumberMarginWidth_);
        }
    }

    FastoEditorView::FastoEditorView(QWidget* parent)
        : QWidget(parent)
    {
        QVBoxLayout *mainL = new QVBoxLayout;
        editor_ = new FastoEditor;

        jsonRadioButton_ = new QRadioButton;
        rawRadioButton_ = new QRadioButton;

        VERIFY(connect(jsonRadioButton_, SIGNAL(toggled(bool)), this, SLOT(viewChanged(bool))));
        VERIFY(connect(rawRadioButton_, SIGNAL(toggled(bool)), this, SLOT(viewChanged(bool))));

        QHBoxLayout* radLaout = new QHBoxLayout;
        radLaout->addWidget(jsonRadioButton_);
        radLaout->addWidget(rawRadioButton_);

        mainL->addLayout(radLaout);
        mainL->addWidget(editor_);
        setLayout(mainL);

        jsonRadioButton_->setChecked(true);
        retranslateUi();
    }

    void FastoEditorView::setModel(QAbstractItemModel* model)
    {
        editor_->setModel(model);
    }

    void FastoEditorView::viewChanged(bool checked)
    {
        if (!checked){
            return;
        }

        if(jsonRadioButton_->isChecked()){
            editor_->viewChanged(true);
            return;
        }

        if(rawRadioButton_->isChecked()){
            editor_->viewChanged(false);
            return;
        }
    }

    void FastoEditorView::setReadOnly(bool readOnly)
    {
        editor_->setReadOnly(readOnly);
    }

    void FastoEditorView::changeEvent(QEvent *e)
    {
        if(e->type() == QEvent::LanguageChange){
            retranslateUi();
        }

        QWidget::changeEvent(e);
    }

    void FastoEditorView::retranslateUi()
    {
        jsonRadioButton_->setText(tr("Json"));
        rawRadioButton_->setText(tr("Raw text"));
    }
}
