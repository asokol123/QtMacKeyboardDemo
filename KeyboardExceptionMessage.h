#ifndef CKEYBOARDEXCEPTIONMESSAGE_H
#define CKEYBOARDEXCEPTIONMESSAGE_H

// This is just a dummy message for debug purpose
class CKeyboardExceptionMessage {
public:
  CKeyboardExceptionMessage() = default;
  CKeyboardExceptionMessage(const char*);
  std::string Text;
};

// This macro is required to use the message class in Qt message system
Q_DECLARE_METATYPE(CKeyboardExceptionMessage);

#endif // CKEYBOARDEXCEPTIONMESSAGE_H
