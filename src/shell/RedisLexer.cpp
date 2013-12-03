#include "shell/RedisLexer.h"

#include <Qsci/qscilexerjavascript.h>

#include "shell/RedisApi.h"

namespace fastoredis
{
    RedisLexer::RedisLexer(QObject *parent)
        : base_class(parent)
    {
        setAPIs(new RedisApi(this));
    }

    const char *RedisLexer::language() const
    {
        return "Redis";
    }

    QString RedisLexer::description(int style) const
    {
        switch (style)
        {
        case Command:
            return tr("Default");

        case Types:
            return tr("Types");
        }

        return QString();
    }

    void RedisLexer::styleText(int start, int end)
    {

    }
}
