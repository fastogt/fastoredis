#include "shell/RedisApi.h"

#include "core/RedisDriver.h"

namespace fastoredis
{
    RedisApi::RedisApi(QsciLexer *lexer)
        : base_class(lexer)
    {
    }

    void RedisApi::updateAutoCompletionList(const QStringList &context, QStringList &list)
    {
        QStringList allCommands = RedisDriver::allCommands();
        for(QStringList::const_iterator it = context.begin(); it != context.end(); ++it){
            QString val = *it;
            for(QStringList::const_iterator jt = allCommands.begin(); jt != allCommands.end(); ++jt){
                QString jval = *jt;
                if(jval.startsWith(val,Qt::CaseInsensitive)){
                    list.append(jval);
                }
            }
        }
    }

    QStringList RedisApi::callTips(const QStringList &context, int commas, QsciScintilla::CallTipsStyle style, QList<int> &shifts)
    {
        return QStringList();
    }
}
