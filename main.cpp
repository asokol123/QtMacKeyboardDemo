#include "MainWindow.h"
#include "KeyboardHandler.h"

int main(int argc, char* argv[]) {
  QApplication QEnvironment(argc, argv);
  MainWindow Window;
  Window.show();

  // This registrations are required to use these messages in QueuedConnections
  // across different threads
  qRegisterMetaType<CKeyboardMessage>();
  qRegisterMetaType<CKeyboardExceptionMessage>();

  CKeyboardHandler Kb;
  return QEnvironment.exec();
}
