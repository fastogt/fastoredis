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

        scin_->setLexer(red);

        scin_->setAutoCompletionThreshold(1);
        if(showAutoCompletion_){
            scin_->setAutoCompletionSource(QsciScintilla::AcsAPIs);
        }
        else{
            scin_->setAutoCompletionSource(QsciScintilla::AcsNone);
        }
        scin_->setAutoCompletionCaseSensitivity(false);

        VERIFY(connect(this, &RedisShell::customContextMenuRequested, this, &RedisShell::showContextMenu));
    }

    void RedisShell::showAutocompletion()
    {
        int start, ignore;
        QStringList context = scin_->apiContext(scin_->SendScintilla(QsciScintilla::SCI_GETCURRENTPOS), start, ignore);

        if(context.empty()){
            // Generate the string representing the valid words to select from.
            QStringList wlist;

            QsciAbstractAPIs *apis = scin_->lexer()->apis();

            if (apis){
                apis->updateAutoCompletionList(QStringList() << ALL_COMMANDS, wlist);
            }

            if (wlist.isEmpty())
                return;

            wlist.sort();

            scin_->SendScintilla(QsciScintilla::SCI_AUTOCSETCHOOSESINGLE, scin_->autoCompletionShowSingle());
            scin_->SendScintilla(QsciScintilla::SCI_AUTOCSETSEPARATOR, '\x03');

            QString text = wlist.join(QChar('\x03'));
            QByteArray wlist_s = text.toUtf8();
            int last_len = 0;
            scin_->SendScintilla(QsciScintilla::SCI_AUTOCSHOW, last_len, ScintillaBytesConstData(wlist_s));
        }
        else{
            scin_->autoCompleteFromAll();
        }
    }
}

