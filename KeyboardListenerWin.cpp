#ifdef Q_OS_WIN
#include "KeyboardListenerWin.h"
#include <sstream>

// For debug purpose only
static void debugNotify(const char* message) {
  std::stringstream os;
  os << std::this_thread::get_id();
  qDebug() << message << " thread::id() = " << os.str().c_str();
}


CKeyboardListenerWin::CKeyboardListenerWin(std::promise<CAnyKeyboardKiller> killerPromise) {
  createMessageWindow(std::move(killerPromise));
  provideHookToListener();
  registerKeyboard();
  debugNotify("CKLW");
}

CKeyboardListenerWin::~CKeyboardListenerWin() {
  unregisterKeyboard();
}

int CKeyboardListenerWin::exec() {
  CKeyboardListenerWin::CMessageStatus Status;
  MSG CurrentMessage;
  while ((Status = getMessage(&CurrentMessage)) != Quit) {
    if (Status == Error)
      handleError(&CurrentMessage);
    else
      dispatchMessage(&CurrentMessage);
  }
  return exitCode(&CurrentMessage);
}

CKeyboardListenerWin::CMessageStatus CKeyboardListenerWin::getMessage(MSG* CurrentMessage) {
  return ::GetMessage(CurrentMessage, MessageWindow_, 0, 0);
}

void CKeyboardListenerWin::dispatchMessage(MSG* CurrentMessage) {
  ::TranslateMessage(CurrentMessage);
  ::DispatchMessage(CurrentMessage);
}

void CKeyboardListenerWin::handleError(MSG* CurrentMessage) {
  (void)CurrentMessage;
  DWORD ErrorCode = GetLastError();
  std::string ErrorMessage = "KeyboardListener error with code = ";
  ErrorMessage += std::to_string(ErrorCode);
  throw std::runtime_error(ErrorMessage.c_str());
}

void CKeyboardListenerWin::stopListening() {
  ::PostMessage(MessageWindow_, WM_STOP_LISTENING, 0, 0);
}

void CKeyboardListenerWin::createMessageWindow(std::promise<CAnyKeyboardKiller> killerPromise) {
  WNDCLASS wc = getWndClass();
  if (!RegisterClass(&wc)) {
    killerPromise.set_value(CKiller(MessageWindow_));
    throw std::runtime_error("Cannot register KeyboardListenerWndClass");
  }

  MessageWindow_ = createWindow(wc);
  if (!MessageWindow_) {
    killerPromise.set_value(CKiller(MessageWindow_));
    throw std::runtime_error("Cannot Create KeyboardListenerWndClass");
  }

  killerPromise.set_value(CKiller(MessageWindow_));
}

void CKeyboardListenerWin::provideHookToListener() {
  ::SetWindowLongPtr(MessageWindow_, GWLP_USERDATA, LONG_PTR(this));
}

void CKeyboardListenerWin::registerKeyboard() {
  RAWINPUTDEVICE LowLevelKeyboard;
  LowLevelKeyboard.usUsagePage = 0x01;
  LowLevelKeyboard.usUsage = 0x06;
  LowLevelKeyboard.dwFlags = RIDEV_INPUTSINK;
  LowLevelKeyboard.hwndTarget = MessageWindow_;
  if (RegisterRawInputDevices(&LowLevelKeyboard,
                              1,
                              sizeof(LowLevelKeyboard))
      == FALSE)
    throw std::runtime_error("Cannot register RAWINPUT");
}

void CKeyboardListenerWin::unregisterKeyboard() {
  RAWINPUTDEVICE LowLevelKeyboard;
  LowLevelKeyboard.usUsagePage = 0x01;
  LowLevelKeyboard.usUsage = 0x06;
  LowLevelKeyboard.dwFlags = RIDEV_REMOVE;
  LowLevelKeyboard.hwndTarget = nullptr;
  if (RegisterRawInputDevices(&LowLevelKeyboard,
                              1,
                              sizeof(LowLevelKeyboard))
      == FALSE)
    qDebug() << "Cannot unregister RAWINPUT";
  // This is a bad idea to throw here since it is used in the destructor
  // and may throw in a stack unwinding process
  //throw std::runtime_error("Cannot unregister RAWINPUT");
}

void CKeyboardListenerWin::HandleRawInput(LPARAM lParam) {
  (void)lParam;
  // Translate the lParam to system independent representation
  // and send it to the required object
  qDebug() << "Handle lParam";
  emit signalKeyboardMessage("Message");
}

int CKeyboardListenerWin::exitCode(MSG* CurrentMessage) const {
  return static_cast<int>(CurrentMessage->wParam);
}

CKeyboardListenerWin* CKeyboardListenerWin::getKeyboardListener(HWND hwnd) {
  return reinterpret_cast<CKeyboardListenerWin*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));
}

LRESULT CKeyboardListenerWin::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
  switch (message) {
  case WM_STOP_LISTENING:
    debugNotify("WndProc");
    ::PostQuitMessage(0);
    break;
  case WM_INPUT:
    debugNotify("WndProc");
    qDebug() << "WM_INPUT";
    getKeyboardListener(hwnd)->HandleRawInput(lparam);
    break;
  default:
    ;
  }
  return ::DefWindowProc(hwnd, message, wparam, lparam);
}

WNDCLASS CKeyboardListenerWin::getWndClass() {
  WNDCLASS wc = {};
  wc.hInstance = ::GetModuleHandle(nullptr);
  wc.lpfnWndProc = WndProc;
  wc.lpszClassName = kWindowClassName;

  return wc;
}

HWND CKeyboardListenerWin::createWindow(const WNDCLASS& wc) {
  return ::CreateWindow(kWindowClassName,
                        TEXT(""),
                        NULL,
                        0, 0, 0, 0,
                        HWND_MESSAGE,
                        NULL,
                        wc.hInstance,
                        NULL);
}


CKeyboardListenerWin::CKiller::CKiller(HWND MessageWindow)
  : MessageWindow_(MessageWindow) {
  assert(MessageWindow_ != NULL);
}

void CKeyboardListenerWin::CKiller::stopListening() const {
  assert(MessageWindow_ != NULL);
  if (MessageWindow_ != NULL)
    ::PostMessage(MessageWindow_, WM_STOP_LISTENING, 0, 0);
}

#endif // Q_OS_WIN
