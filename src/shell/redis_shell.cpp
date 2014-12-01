#include "shell/redis_shell.h"

#include <QKeyEvent>
#include <QMenu>

#include "shell/redis_lexer.h"

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

    void RedisShell::showContextMenu(const QPoint& pt)
    {
        QMenu *menu = createStandardContextMenu();
        menu->exec(mapToGlobal(pt));
        delete menu;
    }

    void RedisShell::keyPressEvent(QKeyEvent* keyEvent)
    {
        if((keyEvent->modifiers() & Qt::ControlModifier) && (keyEvent->key() == Qt::Key_Return) ){
            emit executed();
        }

        return FastoEditor::keyPressEvent(keyEvent);
    }
}

