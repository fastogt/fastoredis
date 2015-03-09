#pragma once

#include <QFrame>

class QAbstractItemModel;
class QRadioButton;
class QToolButton;
class QLineEdit;
class QPushButton;
class QCheckBox;

namespace fastoredis
{
    class FastoEditorOutput;

    class FastoTextView
            : public QWidget
    {
        Q_OBJECT
    public:
        enum
        {
            HeightFindPanel = 40
        };

        FastoTextView(const QString& delemitr, QWidget* parent = 0);

        void setModel(QAbstractItemModel* model);
        void setReadOnly(bool readOnly);

    private Q_SLOTS:
        void viewChanged(bool checked);
        void goToNextElement();
        void goToPrevElement();

    protected:
        virtual void changeEvent(QEvent *);
        virtual void keyPressEvent(QKeyEvent* e);
        virtual bool eventFilter(QObject* object, QEvent* event);

    private:
        void findElement(bool forward);
        void retranslateUi();        

        QRadioButton* jsonRadioButton_;
        QRadioButton* csvRadioButton_;
        QRadioButton* rawRadioButton_;
        QRadioButton* hexRadioButton_;
        QRadioButton* msgPackRadioButton_;
        QRadioButton* gzipRadioButton_;
        FastoEditorOutput* editor_;

        QFrame* findPanel_;
        QLineEdit* findLine_;
        QToolButton* close_;
        QPushButton* next_;
        QPushButton* prev_;
        QCheckBox*  caseSensitive_;
    };
}
