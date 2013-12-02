#include "shell/RedisShell.h"

#include <QKeyEvent>
#include <QStringListModel>
#include <QMenu>

#include "common/macros.h"
#include "gui/GuiFactory.h"
#include "shell/RedisLexer.h"

namespace fastoredis
{
    RedisShell::RedisShell(QWidget *parent)
        : base_class(parent)
    {
        setLexer(new RedisLexer(this));
        setAutoCompletionThreshold(1);
        setAutoCompletionSource(QsciScintilla::AcsAPIs);
        setAutoCompletionCaseSensitivity(false);

        VERIFY(connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint &))));
    }

    void RedisShell::showContextMenu(const QPoint &pt)
    {
        QMenu *menu = createStandardContextMenu();
        menu->exec(mapToGlobal(pt));
        delete menu;
    }

    void RedisShell::keyPressEvent(QKeyEvent *keyEvent)
    {
        if((keyEvent->modifiers() & Qt::ControlModifier) && (keyEvent->key() == Qt::Key_Return) ){
            emit executed();
        }
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Tab) {

        }
        if((keyEvent->modifiers() & Qt::ControlModifier) && (keyEvent->key() == Qt::Key_Space)){

        }

        return base_class::keyPressEvent(keyEvent);
    }

    /*void RedisShell::refreshCompleteList(const QStringList &ls)
    {
        QStringListModel * model = static_cast<QStringListModel *>(_completer->model());
        model->setStringList(ls);

        int currentLine = 0;
        int currentIndex = 0;
        getCursorPosition(&currentLine, &currentIndex);
        int height = (currentLine+1) * textHeight(-1) + 8;

        QRect rec = rect();
        QPoint tl = rec.topLeft();
        QPoint tr = rec.topRight();
        QRect r(tl, tr);
        r.setHeight(height);
        _completer->complete(r);
        _completer->setCurrentRow(0);
    }

    void RedisShell::onCompletionActivate(const QString &text)
    {
        int currentLine = 0;
        int currentIndex = 0;
        getCursorPosition(&currentLine, &currentIndex);
        setSelection(currentLine, 0, currentLine, currentIndex);
        replaceSelectedText(text);
    }*/
}

