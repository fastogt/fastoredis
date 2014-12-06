#pragma once

#include <Qsci/qsciscintilla.h>

#include <QModelIndex>
#include <QFrame>

#define JSON 0
#define CSV 1
#define RAW 2

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
        virtual void keyPressEvent(QKeyEvent* e);
        int lineNumberMarginWidth() const;

    private:
        void showOrHideLinesNumbers();
        int textWidth(int style, const QString &text);

        int lineNumberMarginWidth_;
    };

    class FastoEditorOutput
            : public FastoEditor
    {
        Q_OBJECT
    public:

        FastoEditorOutput(const QString &delemitr, QWidget *parent = 0);
        void setModel(QAbstractItemModel* model);
        void setRootIndex(const QModelIndex& index);

    public Q_SLOTS:
        void viewChanged(int viewMethod);

    private Q_SLOTS:
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

    private:
        QAbstractItemModel* model_;
        int viewMethod_;
        const QString delemitr_;
    };
}
