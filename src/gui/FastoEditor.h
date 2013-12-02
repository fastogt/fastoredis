#pragma once

#include <Qsci/qsciscintilla.h>

#include "global/global.h"

namespace fastoredis
{
    class FastoEditor
            : public QsciScintilla
    {
        Q_OBJECT
    public:
        typedef QsciScintilla base_class;
        enum { rowNumberWidth = 6, indentationWidth = 4 };
        static const QColor marginsBackgroundColor;
        static const QColor caretForegroundColor;
        static const QColor matchedBraceForegroundColor;

        FastoEditor(QWidget *parent = 0);

    private Q_SLOTS:
        void updateLineNumbersMarginWidth();

    protected:
        virtual void keyPressEvent(QKeyEvent *e);
        int lineNumberMarginWidth() const;

    private:
        void showOrHideLinesNumbers();
        int textWidth(int style, const QString &text);

        int _lineNumberMarginWidth;
    };
}
