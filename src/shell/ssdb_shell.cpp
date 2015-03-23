#include "shell/ssdb_shell.h"

#include "shell/ssdb_lexer.h"

#include "gui/gui_factory.h"

namespace
{
    const QColor paperColor(166, 190, 233);
}

namespace fastoredis
{
    SsdbShell::SsdbShell(bool showAutoCompl, QWidget* parent)
        : FastoEditorShell(common::convertFromString<QString>(SsdbLexer::version()), showAutoCompl, parent)
    {
        SsdbLexer* red = new SsdbLexer(this);

        registerImage(SsdbLexer::Command, GuiFactory::instance().commandIcon(SSDB).pixmap(QSize(64,64)));
        registerImage(SsdbLexer::HelpKeyword, GuiFactory::instance().messageBoxQuestionIcon().pixmap(QSize(64,64)));

        setLexer(red);
        red->setPaper(paperColor);

        setAutoCompletionThreshold(1);
        if(showAutoCompletion_){
            setAutoCompletionSource(QsciScintilla::AcsAPIs);
        }
        else{
            setAutoCompletionSource(QsciScintilla::AcsNone);
        }
        setAutoCompletionCaseSensitivity(false);

        VERIFY(connect(this, &SsdbShell::customContextMenuRequested, this, &SsdbShell::showContextMenu));
    }

    void SsdbShell::showAutocompletion()
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

