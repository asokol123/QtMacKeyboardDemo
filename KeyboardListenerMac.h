#ifndef CKEYBOARDLISTENERMAC_H
#define CKEYBOARDLISTENERMAC_H

#include <QtGlobal>

#ifdef Q_OS_MAC

#include "KeyboardMessage.h"
#include "KeyboardExceptionMessage.h"
#include "AnyKeyboardKiller.h"
#include "HoldersMac.h"
#include <ApplicationServices/ApplicationServices.h>

class CKeyboardListenerMac : public QObject {
    Q_OBJECT
public:
    CKeyboardListenerMac(std::promise<CAnyKeyboardKiller>);
    ~CKeyboardListenerMac();

    int exec();

signals:
    void signalKeyboardMessage(const CKeyboardMessage);

private:
    class CKiller {
    public:
        CKiller(CMessagePort&& message)
            : messagePort(std::move(message))
        {}
        void stopListening() const;

    private:
        CMessagePort messagePort;
    };

    static CGEventRef StaticCallbackFunction(
            CGEventTapProxy proxy,
            CGEventType type,
            CGEventRef event,
            void* userInfo);
    CGEventRef CallbackFunction(CGEventType type, CGEventRef event);
    static CGEventMask CreateMask();
    // Just KillFunction wrapper
    static CFDataRef MessagePortCallback(CFMessagePortRef, int, CFDataRef, void*);
    static void KillFunction();
    void SetupRunLoop();

    // Order is important, eventTap and messagePort have to be released before source
    // TODO: Also this behavior is counterintuitive, change CRunLoopSource
    CRunLoopSource runLoopSource;
    CRunLoopSource messagePortSource;
    CEventTap eventTap;
    CMessagePort messagePort;

    // Can't make it constant because CFSTR is not constexpr and has weirt type
    static decltype(CFSTR("")) PORT_NAME;
};

#endif // Q_OS_MAC
#endif // CKEYBOARDLISTENERMAC_H
