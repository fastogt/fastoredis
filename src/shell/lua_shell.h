#pragma once

#include "gui/fasto_editor.h"

namespace fastoredis
{
    class LuaShell
            : public FastoEditorShell
    {
        Q_OBJECT
    public:
        LuaShell(QWidget* parent = 0);
    };

    class RedisLuaShell
            : public LuaShell
    {
        Q_OBJECT
    public:
        RedisLuaShell(QWidget* parent = 0);
    };
}
