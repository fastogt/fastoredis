#include "shell/RedisApi.h"

#include <help.h>

#include "common/qt_helper/converter_patterns.h"

namespace
{
    QStringList getList()
    {
        static int commandslen = sizeof(commandHelp)/sizeof(struct commandHelp);
        static int groupslen = sizeof(commandGroups)/sizeof(char*);
        QStringList list;
        for(int i = 0; i < commandslen; ++i){
            struct commandHelp command = commandHelp[i];
            list.append(common::utils_qt::toQString(std::string(command.name)));
        }
        for(int i = 0; i < groupslen; ++i){
            char* command = commandGroups[i];
            list.append(common::utils_qt::toQString(std::string(command)));
        }
        return list;
    }
    const QStringList allCommands = getList();
}

namespace fastoredis
{
    RedisApi::RedisApi(QsciLexer *lexer)
        : base_class(lexer)
    {
    }

    void RedisApi::updateAutoCompletionList(const QStringList &context, QStringList &list)
    {
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
