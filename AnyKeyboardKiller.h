#ifndef ANYKEYBOARDKILLER_H
#define ANYKEYBOARDKILLER_H

#include "AnyMovable.h"

namespace NSDetail {


template<class TBase>
class IAnyKeyboardKiller : public TBase {
public:
  virtual void stopListening() const = 0;
protected:
  virtual ~IAnyKeyboardKiller() = default;
};

template<class TBase, class TObject>
class CAnyKeyboardKillerImpl : public TBase {
  using CBase = TBase;
public:
  using CBase::CBase;
  void stopListening() const {
    CBase::Object().stopListening();
  }
};

using CAnyKeyboardKillerT = CAnyMovable<IAnyKeyboardKiller, CAnyKeyboardKillerImpl>;
} // NSDetail

// This is Any wrapper around Keyboard Listener Killer
// The object must be able to stop the Listener via
// stopListening() method
class CAnyKeyboardKiller : public NSDetail::CAnyKeyboardKillerT {
  using CBase = NSDetail::CAnyKeyboardKillerT;
public:
  using CBase::CBase;
};

#endif // ANYKEYBOARDKILLER_H
