#pragma once

#include "gui/fasto_editor.h"

namespace fastoredis
{
    class RedisShell
            : public FastoEditorShell
    {
        Q_OBJECT
    public:
        RedisShell(QWidget* parent = 0);

        virtual void showAutocompletion();
    };
}
