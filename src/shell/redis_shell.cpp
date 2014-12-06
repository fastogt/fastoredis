#include "shell/redis_shell.h"

#include <QKeyEvent>
#include <QMenu>

#include "shell/redis_lexer.h"

#include "gui/shortcuts.h"
#include "gui/gui_factory.h"

namespace fastoredis
{
    const QColor RedisShell::paperColor = QColor(166, 190, 233);

    RedisShell::RedisShell(QWidget* parent)
        : FastoEditor(parent)
    {
        RedisLexer* red = new RedisLexer(this);
        version_ = common::convertFromString<QString>(red->version());

        registerImage(RedisLexer::Command, GuiFactory::instance().commandIcon(REDIS).pixmap(QSize(64,64)));
        registerImage(RedisLexer::Types, GuiFactory::instance().typeIcon(REDIS).pixmap(QSize(64,64)));
        registerImage(RedisLexer::HelpKeyword, GuiFactory::instance().messageBoxQuestionIcon().pixmap(QSize(64,64)));

        setLexer(red);
        red->setPaper(paperColor);

        setAutoCompletionThreshold(1);
        setAutoCompletionSource(QsciScintilla::AcsAPIs);
        setAutoCompletionCaseSensitivity(false);

        VERIFY(connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint &))));
    }

    QString RedisShell::version() const
    {
        return version_;
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

            if (apis)
                apis->updateAutoCompletionList(QStringList() << ALL_COMMANDS, wlist);

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

    void RedisShell::hideAutocompletion()
    {
        cancelList();
    }

    void RedisShell::showContextMenu(const QPoint& pt)
    {
        QMenu *menu = createStandardContextMenu();
        menu->exec(mapToGlobal(pt));
        delete menu;
    }

    void RedisShell::keyPressEvent(QKeyEvent* keyEvent)
    {
        if(isAutoCompleteShortcut(keyEvent)){
            showAutocompletion();
            return;
        }
        else if(isHideAutoCompleteShortcut(keyEvent)){
            hideAutocompletion();
            return;
        }

        return FastoEditor::keyPressEvent(keyEvent);
    }
}

