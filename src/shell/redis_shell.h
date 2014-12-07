#pragma once

#include "gui/fasto_editor.h"

namespace fastoredis
{
    class RedisShell
            : public FastoEditorShell
    {
        Q_OBJECT
    public:
        static const QColor paperColor;
        RedisShell(QWidget* parent = 0);

    public:
        virtual void showAutocompletion();
    };
}
