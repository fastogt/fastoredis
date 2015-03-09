#pragma once

#include <QKeyEvent>

namespace fastoredis
{
    class FastoQKeySequence
    {
    public:
        FastoQKeySequence(QKeySequence::StandardKey skey);
        FastoQKeySequence(Qt::KeyboardModifiers mod, int key = Qt::Key_unknown);

        operator QKeySequence();
        operator QKeySequence() const;
        bool operator == (QKeyEvent* ev) const;

    private:
        const QKeySequence::StandardKey skey_;

        const Qt::KeyboardModifiers mod_;
        const int key_;
    };

    const FastoQKeySequence openKey = QKeySequence::Open;
    bool isOpenShortcut(QKeyEvent* keyEvent);

    const FastoQKeySequence saveKey = QKeySequence::Save;
    bool isSaveShortcut(QKeyEvent* keyEvent);

    const FastoQKeySequence saveAsKey = QKeySequence::SaveAs;
    bool isSaveAsShortcut(QKeyEvent* keyEvent);

    const FastoQKeySequence quitKey = QKeySequence::Quit;
    bool isQuitShortcut(QKeyEvent* keyEvent);

    const FastoQKeySequence closeKey = QKeySequence::Close;
    bool isCloseShortcut(QKeyEvent* keyEvent);

    const FastoQKeySequence newTabKey = QKeySequence::AddTab;
    bool isNewTabShortcut(QKeyEvent* keyEvent);

    const FastoQKeySequence nextTabKey = QKeySequence::NextChild;
    bool isNextTabShortcut(QKeyEvent* keyEvent);

    const FastoQKeySequence prevTabKey = QKeySequence::PreviousChild;
    bool isPreviousTabShortcut(QKeyEvent* keyEvent);

    const FastoQKeySequence refreshKey = QKeySequence::Refresh;
    bool isRefreshShortcut(QKeyEvent* keyEvent);

    const FastoQKeySequence fullScreenKey = QKeySequence::FullScreen;
    bool isFullScreenShortcut(QKeyEvent* keyEvent);

    const FastoQKeySequence executeKey = FastoQKeySequence(Qt::ControlModifier, Qt::Key_Return);
    bool isExecuteScriptShortcut(QKeyEvent* keyEvent);

    bool isAutoCompleteShortcut(QKeyEvent* keyEvent);
    bool isHideAutoCompleteShortcut(QKeyEvent* keyEvent);
}
