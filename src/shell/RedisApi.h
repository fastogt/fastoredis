#pragma once

#include <Qsci/qsciapis.h>

#include "global/global.h"

namespace fastoredis
{
    class RedisApi
            : public QsciAbstractAPIs
    {
        Q_OBJECT
    public:
        typedef QsciAbstractAPIs base_class;
        RedisApi(QsciLexer *lexer);
        virtual void updateAutoCompletionList(const QStringList &context, QStringList &list);

        virtual QStringList callTips(const QStringList &context, int commas, QsciScintilla::CallTipsStyle style, QList<int> &shifts);
    };
}
