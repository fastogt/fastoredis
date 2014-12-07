#pragma once

#include "gui/fasto_editor.h"

namespace fastoredis
{
    bool isPythonEnabled();

    class PythonShell
            : public FastoEditorShell
    {
        Q_OBJECT
    public:
        static const QColor paperColor;
        PythonShell(QWidget* parent = 0);
    };
}
