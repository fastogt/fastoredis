#include "gui/shortcuts.h"

namespace fastoredis
{
    FastoQKeySequence::FastoQKeySequence(QKeySequence::StandardKey skey)
        : skey_(skey), mod_(Qt::NoModifier), key_(Qt::Key_unknown)
    {

    }

    FastoQKeySequence::FastoQKeySequence(Qt::KeyboardModifiers mod, int key)
        : skey_(QKeySequence::UnknownKey), mod_(mod), key_(key)
    {

    }

    FastoQKeySequence::operator QKeySequence()
    {
        if(mod_ == Qt::NoModifier && key_ == Qt::Key_unknown){
            return QKeySequence(skey_);
        }
        else{
            return QKeySequence(mod_ + key_);
        }
    }

    FastoQKeySequence::operator QKeySequence() const
    {
        if(mod_ == Qt::NoModifier && key_ == Qt::Key_unknown){
            return QKeySequence(skey_);
        }
        else{
            return QKeySequence(mod_ + key_);
        }
    }

    bool FastoQKeySequence::operator == (QKeyEvent* ev) const
    {
        if(!ev){
            return false;
        }

        if(mod_ == Qt::NoModifier && key_ == Qt::Key_unknown){
            return ev->matches(skey_);
        }
        else{
            return ev->modifiers() == mod_ && ev->key() == key_;
        }
    }

    /*bool isDuplicateTabShortcut(QKeyEvent* keyEvent)
    {
        bool ctrlShiftT = (keyEvent->modifiers() & Qt::ControlModifier) &&
            (keyEvent->modifiers() & Qt::ShiftModifier) &&
            (keyEvent->key() == Qt::Key_T);

        return ctrlShiftT;
    }*/

    // global variables
    const FastoQKeySequence openKey = FastoQKeySequence(QKeySequence::Open);
    const FastoQKeySequence saveKey = FastoQKeySequence(QKeySequence::Save);
    const FastoQKeySequence saveAsKey = FastoQKeySequence(QKeySequence::SaveAs);
    const FastoQKeySequence quitKey = FastoQKeySequence(QKeySequence::Quit);
    const FastoQKeySequence closeKey = FastoQKeySequence(QKeySequence::Close);
    const FastoQKeySequence newTabKey = FastoQKeySequence(QKeySequence::AddTab);
    const FastoQKeySequence nextTabKey = FastoQKeySequence(QKeySequence::NextChild);
    const FastoQKeySequence prevTabKey = FastoQKeySequence(QKeySequence::PreviousChild);
    const FastoQKeySequence refreshKey = FastoQKeySequence(QKeySequence::Refresh);
#ifdef OS_MAC
    const FastoQKeySequence fullScreenKey = FastoQKeySequence(QKeySequence::FullScreen);
#else
    const FastoQKeySequence fullScreenKey = FastoQKeySequence(Qt::ControlModifier | Qt::ShiftModifier, Qt::Key_F11);
#endif
    const FastoQKeySequence executeKey = FastoQKeySequence(Qt::ControlModifier, Qt::Key_Return);

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
