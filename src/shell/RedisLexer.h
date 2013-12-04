#pragma once

#include <QObject>
#include <QColor>
#include <Qsci/qscilexercustom.h>
#include <Qsci/qscilexerdiff.h>

#include "global/global.h"

namespace fastoredis
{
    class RedisLexer
            : public QsciLexerCustom
    {
        Q_OBJECT
    public:
        enum {
            Command = 0,
            Types = 1
        };
        typedef QsciLexerCustom base_class;
        RedisLexer(QObject *parent = 0);
        virtual const char *language() const;
        virtual QString description(int style) const;
        virtual void styleText(int start, int end);
    };
}
