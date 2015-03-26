#include "gui/fasto_scintilla.h"

#include <QKeyEvent>

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
    FastoScintilla::FastoScintilla(QWidget *parent)
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

        VERIFY(connect(this, &FastoScintilla::linesChanged, this, &FastoScintilla::updateLineNumbersMarginWidth));
    }

    void FastoScintilla::updateLineNumbersMarginWidth()
    {
        int numberOfDigits = getNumberOfDigits(lines());

        int tw = textWidth(QsciScintilla::STYLE_LINENUMBER, "0");
        lineNumberMarginWidth_ = numberOfDigits * tw + rowNumberWidth;

        // If line numbers margin already displayed, update its width
        if (lineNumberMarginWidth()) {
            setMarginWidth(0, lineNumberMarginWidth_);
        }
    }

    void FastoScintilla::keyPressEvent(QKeyEvent* keyEvent)
    {
        if (keyEvent->key() == Qt::Key_F11) {
            keyEvent->ignore();
            showOrHideLinesNumbers();
            return;
        }

        return QsciScintilla::keyPressEvent(keyEvent);
    }

    int FastoScintilla::lineNumberMarginWidth() const
    {
        return marginWidth(0);
    }

    int FastoScintilla::textWidth(int style, const QString& text)
    {
        const QByteArray utf8 = text.toUtf8();
        const char *byteArray = utf8.constData();
        return SendScintilla(QsciScintilla::SCI_TEXTWIDTH, style, byteArray);
    }

    void FastoScintilla::showOrHideLinesNumbers()
    {
        updateLineNumbersMarginWidth();
        if (!lineNumberMarginWidth()) {
            setMarginWidth(0, lineNumberMarginWidth_);
        }
        else {
            setMarginWidth(0, 0);
        }
    }
}
