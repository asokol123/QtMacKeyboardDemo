#include "HoldersMac.h"

// CEventTap
CEventTap::~CEventTap() noexcept {
    Free();
}

CEventTap& CEventTap::operator = (CEventTap&& other) noexcept {
    Free();
    eventTap = other.Release();
    return *this;
}

CEventTap& CEventTap::operator = (nullptr_t) noexcept {
    Free();
    return *this;
}

CEventTap::operator bool() const noexcept {
    return eventTap != nullptr;
}

CFMachPortRef CEventTap::Release() noexcept {
    CFMachPortRef result = eventTap;
    eventTap = nullptr;
    return result;
}

CFMachPortRef CEventTap::Get() const noexcept {
    return eventTap;
}

void CEventTap::Set(CFMachPortRef otherEventTap) noexcept {
    Free();
    eventTap = otherEventTap;
}

void CEventTap::Free() noexcept {
    if (eventTap) {
        CFMachPortInvalidate(eventTap);
        CFRelease(eventTap);
    }
}

// CMessagePort
CMessagePort::~CMessagePort() noexcept {
    Free();
}

CMessagePort& CMessagePort::operator = (CMessagePort&& other) noexcept {
    Free();
    messagePort = other.Release();
    return *this;
}

CMessagePort& CMessagePort::operator = (nullptr_t) noexcept {
    Free();
    return *this;
}

CMessagePort::operator bool() const noexcept {
    return messagePort != nullptr;
}

CFMessagePortRef CMessagePort::Release() noexcept {
    CFMessagePortRef result = messagePort;
    messagePort = nullptr;
    return result;
}

CFMessagePortRef CMessagePort::Get() const noexcept {
    return messagePort;
}

void CMessagePort::Set(CFMessagePortRef otherEventTap) noexcept {
    Free();
    messagePort = otherEventTap;
}

int CMessagePort::SendRequest(
        int msgid,
        CFDataRef data,
        CFTimeInterval sendTimeout,
        CFTimeInterval rcvTimeout,
        CFStringRef replyMode,
        CFDataRef* returnData) const {
    return CFMessagePortSendRequest(
            messagePort,
            msgid,
            data,
            sendTimeout,
            rcvTimeout,
            replyMode,
            returnData);
}

void CMessagePort::Free() noexcept {
    if (messagePort) {
        CFMessagePortInvalidate(messagePort);
        CFRelease(messagePort);
    }
}


// CRunLoopSource
CRunLoopSource::~CRunLoopSource() noexcept {
    Free();
}

CRunLoopSource& CRunLoopSource::operator = (CRunLoopSource&& other) noexcept {
    Free();
    runLoopSource = other.Release();
    return *this;
}

CRunLoopSource& CRunLoopSource::operator = (nullptr_t) noexcept {
    Free();
    return *this;
}

CRunLoopSource::operator bool() const noexcept {
    return runLoopSource != nullptr;
}

CFRunLoopSourceRef CRunLoopSource::Release() noexcept {
    CFRunLoopSourceRef result = runLoopSource;
    runLoopSource = nullptr;
    return result;
}

CFRunLoopSourceRef CRunLoopSource::Get() const noexcept {
    return runLoopSource;
}

void CRunLoopSource::Set(CFRunLoopSourceRef otherRunLoopSource) noexcept {
    Free();
    runLoopSource = otherRunLoopSource;
}

void CRunLoopSource::Free() noexcept {
    if (runLoopSource) {
        CFRelease(runLoopSource);
    }
}
