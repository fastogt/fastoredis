#pragma once

/**/

#include <Qsci/qsciscintilla.h>

#include <QModelIndex>

namespace fastoredis
{
    class FastoTreeModel;
    class FastoEditor
            : public QsciScintilla
    {
        Q_OBJECT
    public:
        enum { rowNumberWidth = 6, indentationWidth = 4 };

        FastoEditor(QWidget *parent = 0);
        void setModel(FastoTreeModel *model);

    private Q_SLOTS:
        void updateLineNumbersMarginWidth();

        void modelDestroyed();
        void dataChanged(QModelIndex first, QModelIndex last);
        void headerDataChanged();
        void rowsInserted(QModelIndex index, int r, int c);
        void rowsAboutToBeRemoved(QModelIndex index, int r, int c);
        void rowsRemoved(QModelIndex index, int r, int c);
        void columnsAboutToBeRemoved(QModelIndex index, int r, int c);
        void columnsRemoved(QModelIndex index, int r, int c);
        void columnsInserted(QModelIndex index, int r, int c);
        void reset();
        void layoutChanged();

    protected:
        virtual void keyPressEvent(QKeyEvent *e);
        int lineNumberMarginWidth() const;

    private:
        void showOrHideLinesNumbers();
        int textWidth(int style, const QString &text);        

        int lineNumberMarginWidth_;
        FastoTreeModel* model_;
    };
}
