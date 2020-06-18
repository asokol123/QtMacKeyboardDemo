#ifndef CKEYBOARDHANDLER_H
#define CKEYBOARDHANDLER_H

#include "KeyboardMessage.h"
#include "KeyboardExceptionMessage.h"
#include "AnyKeyboardKiller.h"

// This is a system independent object listening the keyboards globally

class CKeyboardHandler : public QObject {
  Q_OBJECT
public:
  CKeyboardHandler();
  ~CKeyboardHandler();

  void activate();
  void deactivate();

signals:
  void signalKeyboardException(const CKeyboardExceptionMessage);
public slots:
  void slotOnKeyboardMessage(const CKeyboardMessage&);
  // A system dependent keyboard listener lives in an independent thread
  // all exceptions of the listener are sent to this slot
  // The listener dies on any exception or error
  void slotOnKeyboardException(const CKeyboardExceptionMessage&);
private:
  void stopListening() const;

  static void run(std::promise<CAnyKeyboardKiller>, CKeyboardHandler*);

  std::thread ListenerThread_;
  CAnyKeyboardKiller KeyboardKiller_;
  bool isActive_ = true;
};

#endif // CKEYBOARDHANDLER_H
