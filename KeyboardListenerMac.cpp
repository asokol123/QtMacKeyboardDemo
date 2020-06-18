#ifdef Q_OS_MAC
#include "KeyboardListenerMac.h"

#define UNUSED(x) (void)(x)

CKeyboardListenerMac::CKeyboardListenerMac(std::promise<CAnyKeyboardKiller> killer) {
    SetupRunLoop();
    CMessagePort killerMessagePort(CFMessagePortCreateRemote(NULL, PORT_NAME));
    if (!killerMessagePort) {
        throw std::runtime_error("Failed to create killerMessagePort");
    }
    killer.set_value(CKiller(std::move(killerMessagePort)));
}

CKeyboardListenerMac::~CKeyboardListenerMac() {}

int CKeyboardListenerMac::exec() {
    CFRunLoopRun();
    return 0;
}

void CKeyboardListenerMac::CKiller::stopListening() const {
    messagePort.SendRequest(0, NULL, 60, 0, NULL, NULL);
}

CGEventRef CKeyboardListenerMac::StaticCallbackFunction(
        CGEventTapProxy proxy,
        CGEventType type,
        CGEventRef event,
        void *self) {
    UNUSED(proxy);
    return reinterpret_cast<CKeyboardListenerMac*>(self)->
        CallbackFunction(type, event);
}

CGEventRef CKeyboardListenerMac::CallbackFunction(
        CGEventType type,
        CGEventRef event) {
    switch (type) {
    case kCGEventFlagsChanged:
        // Key changed event for a modifier or status key.
        qDebug() << "FlagsChanged, keycode: " << QString::number(CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode)) << endl;
        emit signalKeyboardMessage("FlagsChanged");
        break;
    case kCGEventKeyDown:
        qDebug() << "KeyDown, keycode: " << QString::number(CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode)) << endl;
        emit signalKeyboardMessage("KeyDown");
        break;
    case kCGEventKeyUp:
        qDebug() << "KeyUp, keycode: " << QString::number(CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode)) << endl;
        emit signalKeyboardMessage("KeyUp");
        break;
    default:
        throw std::runtime_error("Unknown event type, something went wrong");
    }
    return event;
}

CGEventMask CKeyboardListenerMac::CreateMask() {
    return CGEventMaskBit(kCGEventKeyDown)
        | CGEventMaskBit(kCGEventKeyUp)
        | CGEventMaskBit(kCGEventFlagsChanged);
}

CFDataRef CKeyboardListenerMac::MessagePortCallback(CFMessagePortRef, int, CFDataRef, void*) {
    KillFunction();
    return NULL;
}

void CKeyboardListenerMac::KillFunction() {
    CFRunLoopStop(CFRunLoopGetCurrent());
}

// More info about CFRunLoop:
// https://developer.apple.com/documentation/corefoundation/cfrunloop-rht
void CKeyboardListenerMac::SetupRunLoop() {
    auto eventMask = CreateMask();
    eventTap.Set(CGEventTapCreate(
            kCGSessionEventTap,
            kCGHeadInsertEventTap,
            kCGEventTapOptionListenOnly,
            eventMask,
            // we need this because callback functions must be static
            StaticCallbackFunction,
            // StaticCallbacFunction will just call real callack
            this
        ));
    if (!eventTap) {
        throw std::runtime_error("Failed to create eventTap");
    }
    messagePort.Set(CFMessagePortCreateLocal(kCFAllocatorDefault, PORT_NAME, MessagePortCallback, NULL, NULL));
    if (!messagePort) {
        throw std::runtime_error("Failed to create messagePort");
    }

    messagePortSource.Set(CFMessagePortCreateRunLoopSource(kCFAllocatorDefault, messagePort.Get(), 0));
    runLoopSource.Set(CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap.Get(), 0));
    CFRunLoopAddSource(CFRunLoopGetCurrent(), messagePortSource.Get(), kCFRunLoopCommonModes);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource.Get(), kCFRunLoopCommonModes);
}

decltype(CFSTR("")) CKeyboardListenerMac::PORT_NAME = CFSTR("KeyboardKillerPort");

#endif // Q_OS_MAC
