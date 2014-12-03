#include "gui/utils/mac_utils.h"

#include <Cocoa/Cocoa.h>

namespace
{
    bool isLion()
    {
        NSString *string = [NSString string];
        // this selector was added only in Lion. so we can check if it's responding, we are on Lion
        return [string respondsToSelector:@selector(linguisticTagsInRange:scheme:options:orthography:tokenRanges:)];
    }
}

namespace fastoredis
{
    void addFullscreenButton(QWidget* widget)
    {
#if defined(MAC_OS_X_VERSION_10_7) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_7
        if (isLion()) // checks if lion is running
        {
            const WId id = widget->winId();
            NSView *nsview = reinterpret_cast<NSView*>(id);
            NSWindow *nswindow = [nsview window];
            [nswindow setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
        }
#else
        #warning no fullscreen support will be included in this build
#endif
    }

    void gotoFullScreen(QWidget *widget)
    {
        const WId id = widget->winId();
        NSView *nsview = reinterpret_cast<NSView*>(id);
        NSWindow *nswindow = [nsview window];
        [nswindow toggleFullScreen:nil];
    }
}

