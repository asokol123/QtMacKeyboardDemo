#ifndef CHOLDERSMAC_H
#define CHOLDERSMAC_H

#include <ApplicationServices/ApplicationServices.h>

class CEventTap {
public:
    CEventTap() noexcept
        : eventTap(nullptr)
    {}
    explicit CEventTap(CFMachPortRef eventTap) noexcept
        : eventTap(eventTap)
    {}
    CEventTap(CEventTap&& other) noexcept
        : eventTap(other.Release())
    {}
    CEventTap(nullptr_t) noexcept
        : eventTap(nullptr)
    {}
    ~CEventTap() noexcept;

    CEventTap& operator = (CEventTap&& other) noexcept;
    CEventTap& operator = (nullptr_t) noexcept;

    operator bool () const noexcept;
    CFMachPortRef Release() noexcept;
    CFMachPortRef Get() const noexcept;
    void Set(CFMachPortRef eventTap) noexcept;

private:
    void Free() noexcept;

    CFMachPortRef eventTap;
};

class CMessagePort {
public:
    CMessagePort() noexcept
        : messagePort(nullptr)
    {}
    explicit CMessagePort(CFMessagePortRef messagePort) noexcept
        : messagePort(messagePort)
    {}
    CMessagePort(CMessagePort&& other) noexcept
        : messagePort(other.Release())
    {}
    CMessagePort(nullptr_t) noexcept
        : messagePort(nullptr)
    {}
    ~CMessagePort() noexcept;

    CMessagePort& operator = (CMessagePort&& other) noexcept;
    CMessagePort& operator = (nullptr_t) noexcept;

    operator bool () const noexcept;
    CFMessagePortRef Release() noexcept;
    CFMessagePortRef Get() const noexcept;
    void Set(CFMessagePortRef messagePort) noexcept;

    int SendRequest(
            int msgid,
            CFDataRef data,
            CFTimeInterval sendTimeout,
            CFTimeInterval rcvTimeout,
            // NULL if oneway message
            CFStringRef replyMode,
            CFDataRef* returnData) const;

private:
    void Free() noexcept;

    CFMessagePortRef messagePort;
};

class CRunLoopSource {
public:
    CRunLoopSource() noexcept
        : runLoopSource(nullptr)
    {}
    explicit CRunLoopSource(CFRunLoopSourceRef runLoopSource) noexcept
        : runLoopSource(runLoopSource)
    {}
    CRunLoopSource(CRunLoopSource&& other) noexcept
        : runLoopSource(other.Release())
    {}
    CRunLoopSource(nullptr_t) noexcept
        : runLoopSource(nullptr)
    {}
    ~CRunLoopSource() noexcept;

    CRunLoopSource& operator = (CRunLoopSource&& other) noexcept;
    CRunLoopSource& operator = (nullptr_t) noexcept;

    operator bool () const noexcept;
    CFRunLoopSourceRef Release() noexcept;
    CFRunLoopSourceRef Get() const noexcept;
    void Set(CFRunLoopSourceRef eventTap) noexcept;

private:
    void Free() noexcept;

    CFRunLoopSourceRef runLoopSource;
};

#endif // CHOLDERSMAC_H
