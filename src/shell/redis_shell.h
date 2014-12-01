#pragma once

/**/

#include "gui/fasto_editor.h"

namespace fastoredis
{
    class RedisShell
            : public FastoEditor
    {
        Q_OBJECT
    public:
        static const QColor paperColor;

        RedisShell(QWidget* parent = 0);
        QString version() const;

    Q_SIGNALS:
        void executed();

    private Q_SLOTS:
        void showContextMenu(const QPoint& pt);

    protected:
        virtual void keyPressEvent(QKeyEvent* keyEvent);

    private:
        QString version_;
    };
}
