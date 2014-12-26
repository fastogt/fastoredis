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

    class RedisPyShell
            : public PythonShell
    {
        Q_OBJECT
    public:
        RedisPyShell(QWidget* parent = 0);
    };
}
