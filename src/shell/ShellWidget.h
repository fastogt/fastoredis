#pragma once
#include <QWidget>
#include "global/global.h"
QT_BEGIN_NAMESPACE
class QTextEdit;
class QAction;
QT_END_NAMESPACE

namespace fastoredis
{
    class ShellWidget
            : public QWidget
    {
        Q_OBJECT
    public:
        typedef QWidget base_class;
        ShellWidget(QWidget* parent = 0);

    private Q_SLOTS:
        void showContextMenu(const QPoint &pt);

    private:
        QTextEdit *_input;
        QAction *_clear;
    };
}
