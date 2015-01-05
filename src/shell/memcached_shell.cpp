#include "shell/memcached_shell.h"

#include "shell/memcached_lexer.h"

#include "gui/gui_factory.h"

namespace
{
    const QColor paperColor = QColor(166, 190, 233);
}

namespace fastoredis
{
    MemcachedShell::MemcachedShell(QWidget* parent)
        : FastoEditorShell(common::convertFromString<QString>(MemcachedLexer::version()), parent)
    {
        MemcachedLexer* red = new MemcachedLexer(this);

        registerImage(MemcachedLexer::Command, GuiFactory::instance().commandIcon(MEMCACHED).pixmap(QSize(64,64)));
        registerImage(MemcachedLexer::HelpKeyword, GuiFactory::instance().messageBoxQuestionIcon().pixmap(QSize(64,64)));

        setLexer(red);
        red->setPaper(paperColor);

        setAutoCompletionThreshold(1);
        setAutoCompletionSource(QsciScintilla::AcsAll);
        setAutoCompletionCaseSensitivity(false);

        VERIFY(connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint &))));
    }

    void MemcachedShell::showAutocompletion()
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

