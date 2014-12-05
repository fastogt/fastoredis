#include "shell/redis_lexer.h"

#include "core/redis/redis_driver.h"

namespace
{
    QStringList initRedisCommand()
    {
        QStringList list;
        for(std::vector<QString>::const_iterator jt = fastoredis::redisCommandsKeywords.begin(); jt != fastoredis::redisCommandsKeywords.end(); ++jt){
            QString jval = *jt;
            list.append(jval + "?1");
        }
        return list;
    }

    QStringList initRedisTypes()
    {
        QStringList list;
        for(std::vector<QString>::const_iterator jt = fastoredis::redisTypesKeywords.begin(); jt != fastoredis::redisTypesKeywords.end(); ++jt){
            QString jval = *jt;
            list.append(jval + "?2");
        }
        return list;
    }

    const QString help("help");
    const QStringList nredisCommandsKeywords = initRedisCommand();
    const QStringList nredisTypesKeywords = initRedisTypes();
}

namespace fastoredis
{
    RedisApi::RedisApi(QsciLexer *lexer)
        : QsciAbstractAPIs(lexer)
    {
    }

    void RedisApi::updateAutoCompletionList(const QStringList& context, QStringList& list)
    {        
        for(QStringList::const_iterator it = context.begin(); it != context.end(); ++it){
            QString val = *it;
            for(QStringList::const_iterator jt = nredisCommandsKeywords.begin(); jt != nredisCommandsKeywords.end(); ++jt){
                QString jval = *jt;
                if(jval.startsWith(val, Qt::CaseInsensitive)){
                    list.append(jval);
                }
            }

            for(QStringList::const_iterator jt = nredisTypesKeywords.begin(); jt != nredisTypesKeywords.end(); ++jt){
                QString jval = *jt;
                if(jval.startsWith(val, Qt::CaseInsensitive)){
                    list.append(jval);
                }
            }

            if(help.startsWith(val, Qt::CaseInsensitive)){
                list.append(help + "?3");
            }
        }
    }

    QStringList RedisApi::callTips(const QStringList& context, int commas, QsciScintilla::CallTipsStyle style, QList<int>& shifts)
    {
        return QStringList();
    }

    RedisLexer::RedisLexer(QObject* parent)
        : QsciLexerCustom(parent)
    {
        setAPIs(new RedisApi(this));
    }

    const char *RedisLexer::language() const
    {
        return "Redis";
    }

    const char* RedisLexer::version() const
    {
        return RedisDriver::versionApi();
    }

    QString RedisLexer::description(int style) const
    {
        switch (style)
        {
        case Default:
             return "Default";
        case Command:
            return "Command";
        case Types:
            return "Types";
        case HelpKeyword:
            return "HelpKeyword";
        }

        return QString(style);
    }

    QColor RedisLexer::defaultColor(int style) const
    {
        switch(style) {
            case Default:
                return Qt::black;
            case Command:
                return Qt::red;
            case Types:
                return Qt::blue;
            case HelpKeyword:
                return Qt::red;
        }

        return Qt::black;
    }

    void RedisLexer::styleText(int start, int end)
    {
        if(!editor()){
            return;
        }

        char *data = new char[end - start + 1];
        editor()->SendScintilla(QsciScintilla::SCI_GETTEXTRANGE, start, end, data);
        QString source(data);
        delete [] data;

        if(source.isEmpty()){
            return;
        }

        paintCommands(source, start);
        paintTypes(source, start);

        int index = 0;
        int begin = 0;
        while( (begin = source.indexOf(help, index, Qt::CaseInsensitive)) != -1){
            index = begin + help.length();

            startStyling(start + begin);
            setStyling(help.length(), HelpKeyword);
            startStyling(start + begin);
        }
    }

    void RedisLexer::paintCommands(const QString& source, int start)
    {
        for(std::vector<QString>::const_iterator it = redisCommandsKeywords.begin(); it != redisCommandsKeywords.end(); ++it){
            QString word = (*it);
            int index = 0;
            int begin = 0;
            while( (begin = source.indexOf(word, index, Qt::CaseInsensitive)) != -1){
                index = begin + word.length();

                startStyling(start + begin);
                setStyling(word.length(), Command);
                startStyling(start + begin);
            }
        }
    }

    void RedisLexer::paintTypes(const QString& source, int start)
    {
        for(std::vector<QString>::const_iterator it = redisTypesKeywords.begin(); it != redisTypesKeywords.end(); ++it){
            QString word = *it;
            int index = 0;
            int begin = 0;
            while( (begin = source.indexOf(word, index, Qt::CaseInsensitive)) != -1){
                index = begin + word.length();

                startStyling(start + begin);
                setStyling(word.length(), Types);
                startStyling(start + begin);
            }
        }
    }
}
