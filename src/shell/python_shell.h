#pragma once

#include "gui/fasto_editor.h"

namespace fastoredis
{
    class PythonShell
            : public FastoEditorShell
    {
        Q_OBJECT
    public:
        PythonShell(QWidget* parent = 0);
    };
}
