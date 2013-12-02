#pragma once

#include <QObject>
#include <QColor>
#include <Qsci/qscilexerjavascript.h>

#include "global/global.h"

namespace fastoredis
{
    class RedisLexer
            : public QsciLexerJavaScript
    {
        Q_OBJECT
    public:
        typedef QsciLexerJavaScript base_class;
        RedisLexer(QObject *parent = 0);
        QColor defaultPaper(int style) const;
        QColor defaultColor(int style) const;
        const char *keywords(int set) const;
    };
}
