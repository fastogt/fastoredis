#pragma once

#include <QModelIndex>

#include <Qsci/qsciscintilla.h>

#define JSON 0
#define CSV 1
#define RAW 2
#define HEX 3
#define MSGPACK 4

namespace fastoredis
{
    class FastoEditor
        : public QsciScintilla
    {
        Q_OBJECT
    public:
        enum
        {
            rowNumberWidth = 6,
            indentationWidth = 4
        };

        FastoEditor(QWidget *parent = 0);        

    private Q_SLOTS:
        void updateLineNumbersMarginWidth();

    protected:
        virtual void keyPressEvent(QKeyEvent* e);
        int lineNumberMarginWidth() const;

    private:
        void showOrHideLinesNumbers();
        int textWidth(int style, const QString& text);

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

    class FastoEditorShell
            : public FastoEditor
    {
        Q_OBJECT
    public:
        QString version() const;

    public:
        virtual void showAutocompletion();
        virtual void hideAutocompletion();

    private Q_SLOTS:
        void showContextMenu(const QPoint& pt);

    protected:
        FastoEditorShell(const QString& version, QWidget *parent = 0);
        void keyPressEvent(QKeyEvent* keyEvent);

    private:
        const QString version_;
    };
}
