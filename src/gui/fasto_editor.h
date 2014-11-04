#pragma once

/**/

#include <Qsci/qsciscintilla.h>

namespace fastoredis
{
    class FastoEditor
            : public QsciScintilla
    {
        Q_OBJECT
    public:
        enum { rowNumberWidth = 6, indentationWidth = 4 };

        FastoEditor(QWidget *parent = 0);

    private Q_SLOTS:
        void updateLineNumbersMarginWidth();

    protected:
        virtual void keyPressEvent(QKeyEvent *e);
        int lineNumberMarginWidth() const;

    private:
        void showOrHideLinesNumbers();
        int textWidth(int style, const QString &text);

        int lineNumberMarginWidth_;
    };
}
