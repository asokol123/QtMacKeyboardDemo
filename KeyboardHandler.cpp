#include "KeyboardHandler.h"
// for debug purpose only
#include <sstream>

#ifdef Q_OS_WIN
#include "KeyboardListenerWin.h"
using CKeyboardListener = CKeyboardListenerWin;
#endif // Q_OS_WIN

#ifdef Q_OS_MACOS
#include "KeyboardListenerMac.h"
using CKeyboardListener = CKeyboardListenerMac;
#endif // Q_OS_MACOS

#ifdef Q_OS_LINUX
#include "KeyboardListenerLinux.h"
using CKeyboardListener = CKeyboardListenerLinux;
#endif // Q_OS_LINUX

// For debug purpose only
static void debugNotify(const char* message) {
  std::stringstream os;
  os << std::this_thread::get_id();
  qDebug() << message << " thread::id() = " << os.str().c_str();
}

CKeyboardHandler::CKeyboardHandler() {
  debugNotify("ctor");

  std::promise<CAnyKeyboardKiller> killerPromise;
  std::future<CAnyKeyboardKiller> killerFuture = killerPromise.get_future();
  // - Я адрессую этот объект, значит надо зашить это в имплементацию через
  // указатели
  // - Переименовать run во что-то более приличное
  ListenerThread_ = std::thread(run, std::move(killerPromise), this);
  KeyboardKiller_ = killerFuture.get();

  connect(this, &CKeyboardHandler::signalKeyboardException,
          this, &CKeyboardHandler::slotOnKeyboardException,
          Qt::ConnectionType::QueuedConnection);
}

CKeyboardHandler::~CKeyboardHandler() {
  debugNotify("dtor");

  stopListening();
  ListenerThread_.join();
}

void CKeyboardHandler::activate() {
  isActive_ = true;
}

void CKeyboardHandler::deactivate() {
  isActive_ = false;
}

void CKeyboardHandler::slotOnKeyboardMessage(const CKeyboardMessage& message) {
  qDebug() << "slotOnKeyboardMessage";
  if (isActive_)
    qDebug() << "KeyboardMessage() = " << message.Text.c_str();
}

void CKeyboardHandler::slotOnKeyboardException(const CKeyboardExceptionMessage& message) {
  qDebug() << "slotOnKeyboardException";
  qDebug() << "ExceptionMessage() = " << message.Text.c_str();
}

void CKeyboardHandler::stopListening() const {
  KeyboardKiller_->stopListening();
}

void CKeyboardHandler::run(std::promise<CAnyKeyboardKiller> killerPromise,
                           CKeyboardHandler* pKeyboardHandler) {
  debugNotify("run");

  try {
    CKeyboardListener KeyboardListener(std::move(killerPromise));

    connect(&KeyboardListener, &CKeyboardListener::signalKeyboardMessage,
            pKeyboardHandler, &CKeyboardHandler::slotOnKeyboardMessage,
            Qt::ConnectionType::QueuedConnection);

    qDebug() << "Kb.exec()";
    KeyboardListener.exec();
  } catch (const std::exception& Exception) {
    pKeyboardHandler->signalKeyboardException(Exception.what());
  } catch (...) {
    pKeyboardHandler->signalKeyboardException("Unknown keyboard exception!");
  }
}
