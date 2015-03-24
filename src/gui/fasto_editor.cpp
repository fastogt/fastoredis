#include "gui/fasto_editor.h"

#include <QMenu>

#include "common/qt/convert_string.h"
#include "common/qt/utils_qt.h"

#include "gui/fasto_common_item.h"
#include "gui/gui_factory.h"
#include "gui/shortcuts.h"

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

    const QColor caretForegroundColor = QColor(QColor(Qt::black));
    const QColor selectionBackgroundColor = QColor(QColor(Qt::blue));
    const QColor selectionForegroundColor = QColor(QColor(Qt::white));

    const QColor matchedBraceForegroundColor = QColor(190, 190, 190);
    const QColor matchedBraceBackgroundColor = QColor(30, 36, 38);

    const QColor marginsBackgroundColor = QColor(Qt::green);
    const QColor marginsForegroundColor = QColor(Qt::white);
}

namespace fastoredis
{
    FastoEditor::FastoEditor(QWidget* parent)
        : QsciScintilla(parent), lineNumberMarginWidth_(0)
    {
        setAutoIndent(true);
        setIndentationsUseTabs(false);
        setIndentationWidth(indentationWidth);
        setUtf8(true);
        setMarginWidth(1, 0);

        setCaretForegroundColor(caretForegroundColor);

        setMatchedBraceForegroundColor(matchedBraceForegroundColor);
        setMatchedBraceBackgroundColor(matchedBraceBackgroundColor);

        setSelectionBackgroundColor(selectionBackgroundColor);
        setSelectionForegroundColor(selectionForegroundColor);

        setContentsMargins(0, 0, 0, 0);
        setViewportMargins(3, 3, 3, 3);
        QFont ourFont = GuiFactory::instance().font();
        setMarginsFont(ourFont);
        setMarginLineNumbers(0, true);

        // Margins colors
        // line numbers margin
        setMarginsBackgroundColor(marginsBackgroundColor);
        setMarginsForegroundColor(marginsForegroundColor);

        SendScintilla(QsciScintilla::SCI_STYLESETFONT, 1, ourFont.family().data());
        SendScintilla(QsciScintilla::SCI_SETHSCROLLBAR, 0);

        setWrapMode((QsciScintilla::WrapMode)QsciScintilla::SC_WRAP_NONE);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        VERIFY(connect(this, &FastoEditor::linesChanged, this, &FastoEditor::updateLineNumbersMarginWidth));
    }

    FastoEditor::~FastoEditor()
    {

    }

    void FastoEditor::updateLineNumbersMarginWidth()
    {
        int numberOfDigits = getNumberOfDigits(lines());

        int tw = textWidth(STYLE_LINENUMBER, "0");
        lineNumberMarginWidth_ = numberOfDigits * tw + rowNumberWidth;

        // If line numbers margin already displayed, update its width
        if (lineNumberMarginWidth()) {
            setMarginWidth(0, lineNumberMarginWidth_);
        }
    }

    void FastoEditor::keyPressEvent(QKeyEvent* keyEvent)
    {
        if (keyEvent->key() == Qt::Key_F11) {
            keyEvent->ignore();
            showOrHideLinesNumbers();
            return;
        }

        return QsciScintilla::keyPressEvent(keyEvent);
    }

    int FastoEditor::lineNumberMarginWidth() const
    {
        return marginWidth(0);
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

    int FastoEditor::textWidth(int style, const QString& text)
    {
        const QByteArray utf8 = text.toUtf8();
        const char *byteArray = utf8.constData();
        return SendScintilla(SCI_TEXTWIDTH, style, byteArray);
    }

    FastoEditorOutput::FastoEditorOutput(const QString &delemitr, QWidget *parent)
        : FastoEditor(parent), model_(NULL), viewMethod_(JSON), delemitr_(delemitr)
    {

    }

    void FastoEditorOutput::setModel(QAbstractItemModel* model)
    {
        if (model == model_){
            return;
        }

        if(model_){
            VERIFY(disconnect(model_, &QAbstractItemModel::destroyed, this, &FastoEditorOutput::modelDestroyed));
            VERIFY(disconnect(model_, &QAbstractItemModel::dataChanged, this, &FastoEditorOutput::dataChanged));
            VERIFY(disconnect(model_, &QAbstractItemModel::headerDataChanged, this, &FastoEditorOutput::headerDataChanged));
            VERIFY(disconnect(model_, &QAbstractItemModel::rowsInserted, this, &FastoEditorOutput::rowsInserted));
            VERIFY(disconnect(model_, &QAbstractItemModel::rowsAboutToBeRemoved, this, &FastoEditorOutput::rowsAboutToBeRemoved));
            VERIFY(disconnect(model_, &QAbstractItemModel::rowsRemoved, this, &FastoEditorOutput::rowsRemoved));
            VERIFY(disconnect(model_, &QAbstractItemModel::columnsAboutToBeRemoved, this, &FastoEditorOutput::columnsAboutToBeRemoved));
            VERIFY(disconnect(model_, &QAbstractItemModel::columnsRemoved, this, &FastoEditorOutput::columnsRemoved));
            VERIFY(disconnect(model_, &QAbstractItemModel::columnsInserted, this, &FastoEditorOutput::columnsInserted));
            VERIFY(disconnect(model_, &QAbstractItemModel::modelReset, this, &FastoEditorOutput::reset));
            VERIFY(disconnect(model_, &QAbstractItemModel::layoutChanged, this, &FastoEditorOutput::layoutChanged));
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
            VERIFY(connect(model_, &QAbstractItemModel::destroyed, this, &FastoEditorOutput::modelDestroyed));
            VERIFY(connect(model_, &QAbstractItemModel::dataChanged, this, &FastoEditorOutput::dataChanged));
            VERIFY(connect(model_, &QAbstractItemModel::headerDataChanged, this, &FastoEditorOutput::headerDataChanged));
            VERIFY(connect(model_, &QAbstractItemModel::rowsInserted, this, &FastoEditorOutput::rowsInserted));
            VERIFY(connect(model_, &QAbstractItemModel::rowsAboutToBeRemoved, this, &FastoEditorOutput::rowsAboutToBeRemoved));
            VERIFY(connect(model_, &QAbstractItemModel::rowsRemoved, this, &FastoEditorOutput::rowsRemoved));
            VERIFY(connect(model_, &QAbstractItemModel::columnsAboutToBeRemoved, this, &FastoEditorOutput::columnsAboutToBeRemoved));
            VERIFY(connect(model_, &QAbstractItemModel::columnsRemoved, this, &FastoEditorOutput::columnsRemoved));
            VERIFY(connect(model_, &QAbstractItemModel::columnsInserted, this, &FastoEditorOutput::columnsInserted));
            VERIFY(connect(model_, &QAbstractItemModel::modelReset, this, &FastoEditorOutput::reset));
            VERIFY(connect(model_, &QAbstractItemModel::layoutChanged, this, &FastoEditorOutput::layoutChanged));
        }

        reset();
    }

    void FastoEditorOutput::viewChanged(int viewMethod)
    {
        viewMethod_ = viewMethod;
        layoutChanged();
    }

    void FastoEditorOutput::modelDestroyed()
    {

    }

    void FastoEditorOutput::dataChanged(QModelIndex first, QModelIndex last)
    {
        layoutChanged();
    }

    void FastoEditorOutput::headerDataChanged()
    {

    }

    void FastoEditorOutput::rowsInserted(QModelIndex index, int r, int c)
    {
        layoutChanged();
    }

    void FastoEditorOutput::rowsAboutToBeRemoved(QModelIndex index, int r, int c)
    {

    }

    void FastoEditorOutput::rowsRemoved(QModelIndex index, int r, int c)
    {

    }

    void FastoEditorOutput::columnsAboutToBeRemoved(QModelIndex index, int r, int c)
    {

    }

    void FastoEditorOutput::columnsRemoved(QModelIndex index, int r, int c)
    {

    }

    void FastoEditorOutput::columnsInserted(QModelIndex index, int r, int c)
    {

    }

    void FastoEditorOutput::reset()
    {
        layoutChanged();
    }

    void FastoEditorOutput::layoutChanged()
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
            FastoCommonItem* child = dynamic_cast<FastoCommonItem*>(root->child(i));
            if(!child){
                continue;
            }

            if(viewMethod_ == JSON){
                QString json = toJson(child);
                result += common::escapedText(json);
            }
            else if(viewMethod_ == CSV){
                QString csv = toCsv(child, delemitr_);
                result += common::escapedText(csv);
            }
            else if(viewMethod_ == RAW){
                QString raw = toRaw(child);
                result += common::escapedText(raw);
            }
            else if(viewMethod_ == HEX){
                QString hex = toHex(child);
                result += common::escapedText(hex);
            }
            else if(viewMethod_ == MSGPACK){
                QString hex = fromHexMsgPack(child);
                result += common::escapedText(hex);
            }
            else if(viewMethod_ == GZIP){
                QString gzip = fromGzip(child);
                result += common::escapedText(gzip);
            }
        }

        setText(result);
    }

    FastoEditorShell::FastoEditorShell(const QString& version, bool showAutoCompl, QWidget* parent)
        : FastoEditor(parent), version_(version), showAutoCompletion_(showAutoCompl)
    {
        VERIFY(connect(this, &FastoEditorShell::customContextMenuRequested, this, &FastoEditorShell::showContextMenu));
    }

    QString FastoEditorShell::version() const
    {
        return version_;
    }

    void FastoEditorShell::showAutocompletion()
    {
        if(showAutoCompletion_){
            autoCompleteFromAll();
        }
    }

    void FastoEditorShell::hideAutocompletion()
    {
        if(showAutoCompletion_){
            cancelList();
        }
    }

    void FastoEditorShell::showContextMenu(const QPoint& pt)
    {
        QMenu *menu = createStandardContextMenu();
        menu->exec(mapToGlobal(pt));
        delete menu;
    }

    void FastoEditorShell::keyPressEvent(QKeyEvent* keyEvent)
    {
        if(showAutoCompletion_){
            if(isAutoCompleteShortcut(keyEvent)){
                showAutocompletion();
                return;
            }
            else if(isHideAutoCompleteShortcut(keyEvent)){
                hideAutocompletion();
                return;
            }
        }

        return FastoEditor::keyPressEvent(keyEvent);
    }
}
