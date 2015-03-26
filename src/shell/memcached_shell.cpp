#include "shell/memcached_shell.h"

#include "shell/memcached_lexer.h"

#include "gui/gui_factory.h"

namespace fastoredis
{
    MemcachedShell::MemcachedShell(bool showAutoCompl, QWidget* parent)
        : FastoEditorShell(common::convertFromString<QString>(MemcachedLexer::version()), showAutoCompl, parent)
    {
        MemcachedLexer* red = new MemcachedLexer(this);

        registerImage(MemcachedLexer::Command, GuiFactory::instance().commandIcon(MEMCACHED).pixmap(QSize(64,64)));
        registerImage(MemcachedLexer::HelpKeyword, GuiFactory::instance().messageBoxQuestionIcon().pixmap(QSize(64,64)));

        scin_->setLexer(red);

        scin_->setAutoCompletionThreshold(1);
        if(showAutoCompletion_){
            scin_->setAutoCompletionSource(QsciScintilla::AcsAPIs);
        }
        else{
            scin_->setAutoCompletionSource(QsciScintilla::AcsNone);
        }
        scin_->setAutoCompletionCaseSensitivity(false);

        VERIFY(connect(this, &MemcachedShell::customContextMenuRequested, this, &MemcachedShell::showContextMenu));
    }

    void MemcachedShell::showAutocompletion()
    {
        int start, ignore;
        QStringList context = scin_->apiContext(scin_->SendScintilla(QsciScintilla::SCI_GETCURRENTPOS), start,
                ignore);

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

            QByteArray wlist_s;// = textAsBytes(wlist.join(QChar('\x03')));
            int last_len = 0;
            scin_->SendScintilla(QsciScintilla::SCI_AUTOCSHOW, last_len, ScintillaBytesConstData(wlist_s));
        }
        else{
            scin_->autoCompleteFromAll();
        }
    }
}

