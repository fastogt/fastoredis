#pragma once

/**/

#include <Qsci/qsciscintilla.h>

#include <QModelIndex>

class QAbstractItemModel;
class QRadioButton;

namespace fastoredis
{
    class FastoEditor
            : public QsciScintilla
    {
        Q_OBJECT
    public:
        enum { rowNumberWidth = 6, indentationWidth = 4 };

        FastoEditor(QWidget *parent = 0);
        void setModel(QAbstractItemModel* model);
        void setRootIndex(const QModelIndex& index);

    public Q_SLOTS:
        void viewChanged(bool isJson);

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
        QAbstractItemModel* model_;

        QModelIndex root_index_;
        bool isJsonChecked_;
    };

    class FastoEditorView
            : public QWidget
    {
        Q_OBJECT
    public:
        FastoEditorView(QWidget* parent = 0);
        void setModel(QAbstractItemModel* model);
        void setRootIndex(const QModelIndex& index);
        void setReadOnly(bool readOnly);

    protected:
        virtual void changeEvent(QEvent *);

    private Q_SLOTS:
        void viewChanged(bool checked);

    private:
        void retranslateUi();

        QRadioButton* jsonRadioButton_;
        QRadioButton* rawRadioButton_;
        FastoEditor* editor_;
    };
}
