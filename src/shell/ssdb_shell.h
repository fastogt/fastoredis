#pragma once

#include "gui/fasto_editor.h"

namespace fastoredis
{
    class SsdbShell
            : public FastoEditorShell
    {
        Q_OBJECT
    public:
        SsdbShell(QWidget* parent = 0);

        virtual void showAutocompletion();
    };
}
