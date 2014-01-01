#include "gui/FastoEditor.h"

#include <QKeyEvent>

#include "gui/GuiFactory.h"
#include "common/macros.h"

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
    const QColor FastoEditor::marginsBackgroundColor = QColor(73, 76, 78);
    const QColor FastoEditor::caretForegroundColor = QColor("#FFFFFF");
    const QColor FastoEditor::matchedBraceForegroundColor = QColor("#FF8861");


    FastoEditor::FastoEditor(QWidget *parent)
        : QsciScintilla(parent), _lineNumberMarginWidth(0)
    {
        setAutoIndent(true);
        setIndentationsUseTabs(false);
        setIndentationWidth(indentationWidth);
        setUtf8(true);
        setMarginWidth(1, 0);
        setCaretForegroundColor(caretForegroundColor);
        setMatchedBraceForegroundColor(matchedBraceForegroundColor); //1AB0A6
        setMatchedBraceBackgroundColor(marginsBackgroundColor);
        setContentsMargins(0, 0, 0, 0);
        setViewportMargins(3, 3, 3, 3);
        QFont ourFont = GuiFactory::instance().font();
        setMarginsFont(ourFont);
        setMarginLineNumbers(0, true);
        setMarginsBackgroundColor(QColor(53, 56, 58));
        setMarginsForegroundColor(QColor(173, 176, 178));

        SendScintilla(QsciScintilla::SCI_STYLESETFONT, 1, ourFont.family().data());
        SendScintilla(QsciScintilla::SCI_SETHSCROLLBAR, 0);

        setWrapMode((QsciScintilla::WrapMode)QsciScintilla::SC_WRAP_NONE);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        VERIFY(connect(this, SIGNAL(linesChanged()), this, SLOT(updateLineNumbersMarginWidth())));
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
            setMarginWidth(0, _lineNumberMarginWidth);

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

        _lineNumberMarginWidth = numberOfDigits * textWidth(STYLE_LINENUMBER, "0") + rowNumberWidth;

        // If line numbers margin already displayed, update its width
        if (lineNumberMarginWidth()) {
            setMarginWidth(0, _lineNumberMarginWidth);
        }
    }
}
