#include "gui/shortcuts.h"

namespace fastoredis
{
    /*bool isDuplicateTabShortcut(QKeyEvent* keyEvent)
    {
        bool ctrlShiftT = (keyEvent->modifiers() & Qt::ControlModifier) &&
            (keyEvent->modifiers() & Qt::ShiftModifier) &&
            (keyEvent->key() == Qt::Key_T);

        return ctrlShiftT;
    }*/

    // global variables
    const common::qt::FastoQKeySequence openKey = common::qt::FastoQKeySequence(QKeySequence::Open);
    const common::qt::FastoQKeySequence saveKey = common::qt::FastoQKeySequence(QKeySequence::Save);
    const common::qt::FastoQKeySequence saveAsKey = common::qt::FastoQKeySequence(QKeySequence::SaveAs);
#ifdef OS_MAC
    const common::qt::FastoQKeySequence quitKey = common::qt::FastoQKeySequence(QKeySequence::Quit);
#else
    const common::qt::FastoQKeySequence quitKey = common::qt::FastoQKeySequence(Qt::ControlModifier, Qt::Key_Q);
#endif
    const common::qt::FastoQKeySequence closeKey = common::qt::FastoQKeySequence(QKeySequence::Close);
    const common::qt::FastoQKeySequence newTabKey = common::qt::FastoQKeySequence(QKeySequence::AddTab);
    const common::qt::FastoQKeySequence nextTabKey = common::qt::FastoQKeySequence(QKeySequence::NextChild);
    const common::qt::FastoQKeySequence prevTabKey = common::qt::FastoQKeySequence(QKeySequence::PreviousChild);
    const common::qt::FastoQKeySequence refreshKey = common::qt::FastoQKeySequence(QKeySequence::Refresh);
#ifdef OS_MAC
    const common::qt::FastoQKeySequence fullScreenKey = common::qt::FastoQKeySequence(QKeySequence::FullScreen);
#else
    const common::qt::FastoQKeySequence fullScreenKey = common::qt::FastoQKeySequence(Qt::ControlModifier | Qt::ShiftModifier, Qt::Key_F11);
#endif
    const common::qt::FastoQKeySequence executeKey = common::qt::FastoQKeySequence(Qt::ControlModifier, Qt::Key_Return);

    bool isOpenShortcut(QKeyEvent* keyEvent)
    {
        return openKey == keyEvent;
    }

    bool isSaveShortcut(QKeyEvent* keyEvent)
    {
        return saveKey == keyEvent;
    }

    bool isSaveAsShortcut(QKeyEvent* keyEvent)
    {
        return saveAsKey == keyEvent;
    }

    bool isQuitShortcut(QKeyEvent *keyEvent)
    {
        return quitKey == keyEvent;
    }

    bool isCloseShortcut(QKeyEvent* keyEvent)
    {
        return closeKey == keyEvent;
    }

    bool isSetFocusOnQueryLineShortcut(QKeyEvent* keyEvent)
    {
        return keyEvent->key() == Qt::Key_F6;
    }

    bool isNewTabShortcut(QKeyEvent* keyEvent)
    {
        return newTabKey == keyEvent;
    }

    bool isNextTabShortcut(QKeyEvent* keyEvent)
    {
        return nextTabKey == keyEvent;
    }

    bool isPreviousTabShortcut(QKeyEvent* keyEvent)
    {
        return prevTabKey == keyEvent;
    }

    bool isRefreshShortcut(QKeyEvent* keyEvent)
    {
        return refreshKey == keyEvent;
    }

    bool isFullScreenShortcut(QKeyEvent* keyEvent)
    {
        return fullScreenKey == keyEvent;
    }

    bool isExecuteScriptShortcut(QKeyEvent* keyEvent)
    {
        return executeKey == keyEvent;
    }

    /*bool isToggleCommentsShortcut(QKeyEvent* keyEvent)
    {
        return ((keyEvent->modifiers() & Qt::ControlModifier) && (keyEvent->key() == Qt::Key_Slash)) ||
               ((keyEvent->modifiers() & Qt::ControlModifier) && (keyEvent->modifiers() & Qt::ShiftModifier) && (keyEvent->key() == Qt::Key_C));
    }*/

    bool isAutoCompleteShortcut(QKeyEvent* keyEvent)
    {
        return (keyEvent->modifiers() & Qt::ControlModifier) && (keyEvent->key() == Qt::Key_Space);
    }

    bool isHideAutoCompleteShortcut(QKeyEvent* keyEvent)
    {
        return (keyEvent->key() == Qt::Key_Escape);
    }
}
