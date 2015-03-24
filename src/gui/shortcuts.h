#pragma once

#include <QKeyEvent>

namespace fastoredis
{
    class FastoQKeySequence
    {
    public:
        explicit FastoQKeySequence(QKeySequence::StandardKey skey);
        explicit FastoQKeySequence(Qt::KeyboardModifiers mod, int key = Qt::Key_unknown);

        operator QKeySequence();
        operator QKeySequence() const;
        bool operator == (QKeyEvent* ev) const;

    private:
        const QKeySequence::StandardKey skey_;
        const Qt::KeyboardModifiers mod_;
        const int key_;
    };

    // global variables
    extern const FastoQKeySequence openKey;
    extern const FastoQKeySequence saveKey;
    extern const FastoQKeySequence saveAsKey;
    extern const FastoQKeySequence quitKey;
    extern const FastoQKeySequence closeKey;
    extern const FastoQKeySequence newTabKey;
    extern const FastoQKeySequence nextTabKey;
    extern const FastoQKeySequence prevTabKey;
    extern const FastoQKeySequence refreshKey;
    extern const FastoQKeySequence fullScreenKey;
    extern const FastoQKeySequence executeKey;

    bool isOpenShortcut(QKeyEvent* keyEvent);
    bool isSaveShortcut(QKeyEvent* keyEvent);
    bool isSaveAsShortcut(QKeyEvent* keyEvent);
    bool isQuitShortcut(QKeyEvent* keyEvent);
    bool isCloseShortcut(QKeyEvent* keyEvent);
    bool isNewTabShortcut(QKeyEvent* keyEvent);
    bool isNextTabShortcut(QKeyEvent* keyEvent);
    bool isPreviousTabShortcut(QKeyEvent* keyEvent);
    bool isRefreshShortcut(QKeyEvent* keyEvent);
    bool isFullScreenShortcut(QKeyEvent* keyEvent);
    bool isExecuteScriptShortcut(QKeyEvent* keyEvent);
    bool isAutoCompleteShortcut(QKeyEvent* keyEvent);
    bool isHideAutoCompleteShortcut(QKeyEvent* keyEvent);
}
