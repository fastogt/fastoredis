#pragma once

#include "gui/fasto_editor.h"

namespace fastoredis
{
    class PythonShell
            : public FastoEditor
    {
        Q_OBJECT
    public:
        static const QColor paperColor;

        PythonShell(QWidget* parent = 0);
        QString version() const;

    public Q_SLOTS:
        void showAutocompletion();
        void hideAutocompletion();

    private Q_SLOTS:
        void showContextMenu(const QPoint& pt);

    protected:
        virtual void keyPressEvent(QKeyEvent* keyEvent);

    private:
        QString version_;
    };
}
