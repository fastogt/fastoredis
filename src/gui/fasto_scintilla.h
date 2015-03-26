#pragma once

#include <Qsci/qsciscintilla.h>

namespace fastoredis
{
    class FastoScintilla
            : public QsciScintilla
    {
        Q_OBJECT
    public:
        enum
        {
            rowNumberWidth = 6,
            indentationWidth = 4
        };

        FastoScintilla(QWidget *parent = 0);

    private Q_SLOTS:
        void updateLineNumbersMarginWidth();

    protected:
        virtual void keyPressEvent(QKeyEvent* e);

    private:
        int lineNumberMarginWidth() const;
        int textWidth(int style, const QString& text);
        void showOrHideLinesNumbers();

        int lineNumberMarginWidth_;
    };
}
