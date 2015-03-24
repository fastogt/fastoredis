#include "shell/redis_shell.h"

#include "shell/redis_lexer.h"

#include "gui/gui_factory.h"

namespace fastoredis
{
    RedisShell::RedisShell(bool showAutoCompl, QWidget* parent)
        : FastoEditorShell(common::convertFromString<QString>(RedisLexer::version()), showAutoCompl, parent)
    {
        RedisLexer* red = new RedisLexer(this);

        registerImage(RedisLexer::Command, GuiFactory::instance().commandIcon(REDIS).pixmap(QSize(64,64)));
        registerImage(RedisLexer::Types, GuiFactory::instance().typeIcon(REDIS).pixmap(QSize(64,64)));
        registerImage(RedisLexer::HelpKeyword, GuiFactory::instance().messageBoxQuestionIcon().pixmap(QSize(64,64)));

        setLexer(red);

        setAutoCompletionThreshold(1);
        if(showAutoCompletion_){
            setAutoCompletionSource(QsciScintilla::AcsAPIs);
        }
        else{
            setAutoCompletionSource(QsciScintilla::AcsNone);
        }
        setAutoCompletionCaseSensitivity(false);

        VERIFY(connect(this, &RedisShell::customContextMenuRequested, this, &RedisShell::showContextMenu));
    }

    void RedisShell::showAutocompletion()
    {
        int start, ignore;
        QStringList context = apiContext(SendScintilla(SCI_GETCURRENTPOS), start,
                ignore);

        if(context.empty()){
            // Generate the string representing the valid words to select from.
            QStringList wlist;

            QsciAbstractAPIs *apis = lexer()->apis();

            if (apis){
                apis->updateAutoCompletionList(QStringList() << ALL_COMMANDS, wlist);
            }

            if (wlist.isEmpty())
                return;

            wlist.sort();

            SendScintilla(SCI_AUTOCSETCHOOSESINGLE, autoCompletionShowSingle());
            SendScintilla(SCI_AUTOCSETSEPARATOR, '\x03');

            ScintillaBytes wlist_s = textAsBytes(wlist.join(QChar('\x03')));
            int last_len = 0;
            SendScintilla(SCI_AUTOCSHOW, last_len, ScintillaBytesConstData(wlist_s));
        }
        else{
            autoCompleteFromAll();
        }
    }
}

