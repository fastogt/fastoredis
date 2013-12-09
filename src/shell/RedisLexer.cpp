#include "shell/RedisLexer.h"

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

    QColor RedisLexer::defaultColor(int style) const
    {
        return base_class::defaultColor(style);
    }

    void RedisLexer::styleText(int start, int end)
    {

    }

    const char *RedisLexer::keywords(int set) const
    {
        if (set == 1)
            return RedisDriver::allCommandsLine().c_str();

        return 0;
    }
}
