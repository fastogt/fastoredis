#pragma once

#include "gui/fasto_editor.h"

namespace fastoredis
{
    class MemcachedShell
            : public FastoEditorShell
    {
        Q_OBJECT
    public:
        MemcachedShell(QWidget* parent = 0);

        virtual void showAutocompletion();
    };
}
