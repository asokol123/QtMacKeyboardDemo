#ifndef CKEYBOARDLISTENERWIN_H
#define CKEYBOARDLISTENERWIN_H

#include <QtGlobal>

// DO NOT inlclude this file in any header file because it contains Windows dependent headers

#ifdef Q_OS_WIN

#include <Windows.h>
#include "KeyboardMessage.h"
#include "KeyboardExceptionMessage.h"
#include "AnyKeyboardKiller.h"

// This is a Windows specific KeyboardListener

class CKeyboardListenerWin : public QObject {
  Q_OBJECT
  using CMessageStatus = BOOL;
public:
  CKeyboardListenerWin(std::promise<CAnyKeyboardKiller>);
  ~CKeyboardListenerWin();

  int exec();

signals:
  void signalKeyboardMessage(const CKeyboardMessage);
private:
  // The object provides a way to shut down the listener
  class CKiller {
  public:
    CKiller() = default;
    CKiller(HWND);
    void stopListening() const;
  private:
    HWND MessageWindow_;
  };

  CMessageStatus getMessage(MSG*);
  void dispatchMessage(MSG*);
  [[noreturn]] void handleError(MSG*);
  void stopListening();

  void createMessageWindow(std::promise<CAnyKeyboardKiller>);
  void provideHookToListener();
  void registerKeyboard();
  void unregisterKeyboard();
  void HandleRawInput(LPARAM lParam);

  int exitCode(MSG*) const;

  static CKeyboardListenerWin* getKeyboardListener(HWND hwnd);
  static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

  static WNDCLASS getWndClass();
  static HWND createWindow(const WNDCLASS&);

  static constexpr CMessageStatus Error = -1;
  static constexpr CMessageStatus Quit = 0;

  static constexpr const wchar_t* kWindowClassName = L"KeyboardListenerWndClass";

  // This is a user-defined message to notify the listener to stop listening
  static constexpr UINT WM_STOP_LISTENING = WM_APP;

  // KeyboardListener creates an object MessageWindow_ with a message box in the Windows Eco-System
  // Then it registers RAWINPUT to receive keyboard input to the MessageWindow_ message box
  // KeyboardListener handles the keyboard input in exec() function
  // It may receive two types of messages: (1) RAWINPUT, (2) WM_STOP_LISTENING signal
  // In the first case, it handles the keyboard input
  // In the second case, it stops listening and exits exec() function

  HWND MessageWindow_;
};
#endif // Q_OS_WIN
#endif // CKEYBOARDLISTENERWIN_H
