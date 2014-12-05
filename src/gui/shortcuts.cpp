#include "gui/shortcuts.h"

namespace fastoredis
{
    bool isCloseTabShortcut(QKeyEvent *keyEvent)
    {
        return (keyEvent->modifiers() & Qt::ControlModifier) && (keyEvent->key()==Qt::Key_F4 || keyEvent->key()==Qt::Key_W);
    }

    bool isNewTabShortcut(QKeyEvent *keyEvent)
    {
        bool ctrlShiftReturn = (keyEvent->modifiers() & Qt::ControlModifier) &&
            (keyEvent->modifiers() & Qt::ShiftModifier) &&
            (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter);

        // TODO: compare with QKeySequence::AddTab rather than Ctrl+T
        bool ctrlT = (keyEvent->modifiers() & Qt::ControlModifier) && !(keyEvent->modifiers() & Qt::ShiftModifier) && (keyEvent->key() == Qt::Key_T);

        return ctrlShiftReturn || ctrlT;
    }

    bool isDuplicateTabShortcut(QKeyEvent *keyEvent)
    {
        bool ctrlShiftT = (keyEvent->modifiers() & Qt::ControlModifier) &&
            (keyEvent->modifiers() & Qt::ShiftModifier) &&
            (keyEvent->key() == Qt::Key_T);

        return ctrlShiftT;
    }

    bool isSetFocusOnQueryLineShortcut(QKeyEvent *keyEvent)
    {
        return keyEvent->key() == Qt::Key_F6;
    }

    bool isExecuteScriptShortcut(QKeyEvent *keyEvent)
    {
        return (keyEvent->modifiers() & Qt::ControlModifier)
            &&
            (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter);
    }

    bool isAutoCompleteShortcut(QKeyEvent *keyEvent)
    {
        return (keyEvent->modifiers() & Qt::AltModifier) && (keyEvent->key() == Qt::Key_Space);
    }

    bool isHideAutoCompleteShortcut(QKeyEvent *keyEvent)
    {
        return (keyEvent->key() == Qt::Key_Escape);
    }

    bool isNextTabShortcut(QKeyEvent *keyEvent)
    {
        return (keyEvent->modifiers() & Qt::ControlModifier)
            && (keyEvent->modifiers() & Qt::AltModifier)
            && (keyEvent->key() == Qt::Key_Right);
    }

    bool isPreviousTabShortcut(QKeyEvent *keyEvent)
    {
        return (keyEvent->modifiers() & Qt::ControlModifier)
            && (keyEvent->modifiers() & Qt::AltModifier)
            && (keyEvent->key() == Qt::Key_Left);
    }

    bool isToggleCommentsShortcut(QKeyEvent *keyEvent)
    {
        return ((keyEvent->modifiers() & Qt::ControlModifier)
               && (keyEvent->key() == Qt::Key_Slash))
            ||
               ((keyEvent->modifiers() & Qt::ControlModifier)
               && (keyEvent->modifiers() & Qt::ShiftModifier)
               && (keyEvent->key() == Qt::Key_C));
    }
}
