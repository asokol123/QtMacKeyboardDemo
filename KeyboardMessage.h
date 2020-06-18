#ifndef CKEYBOARDMESSAGE_H
#define CKEYBOARDMESSAGE_H

// This is just a dummy message for debug purpose
class CKeyboardMessage {
public:
  CKeyboardMessage() = default;
  CKeyboardMessage(const char* text);
  std::string Text;
};

// This macro is required to use the message class in Qt message system
Q_DECLARE_METATYPE(CKeyboardMessage);

#endif // CKEYBOARDMESSAGE_H
