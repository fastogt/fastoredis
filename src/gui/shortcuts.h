#pragma once

#include "fasto/common/qt/gui/shortcuts.h"

namespace fastoredis
{
    // global variables
    extern const common::qt::FastoQKeySequence openKey;
    extern const common::qt::FastoQKeySequence saveKey;
    extern const common::qt::FastoQKeySequence saveAsKey;
    extern const common::qt::FastoQKeySequence quitKey;
    extern const common::qt::FastoQKeySequence closeKey;
    extern const common::qt::FastoQKeySequence newTabKey;
    extern const common::qt::FastoQKeySequence nextTabKey;
    extern const common::qt::FastoQKeySequence prevTabKey;
    extern const common::qt::FastoQKeySequence refreshKey;
    extern const common::qt::FastoQKeySequence fullScreenKey;
    extern const common::qt::FastoQKeySequence executeKey;

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
