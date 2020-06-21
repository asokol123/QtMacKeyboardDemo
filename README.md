# QtMacKeyboardDemo
Это простая демонстрационная версия, поэтому информация будет просто выводиться в консоль и использовать коды клавишь, а не красивые называния. Так же запуск только из консоли, в полноценном macOS приложении нет смысла.
## Файлы проекта
Сам проект реализован в файлах `KeyboardListenerMac.cpp`, `KeyboardListenerMac.h`, `HoldersMac.cpp`, `HoldersMac.h`. Остальные файлы нужны для соблюдения общей архитектуры проекта.
## Зависимости
Необходим `qt >= 5.14`
Установить его можно с помощью `brew install qt`
Проверить версию можно с помощью `qmake --version`
## Установка
```bash
git clone https://github.com/asokol123/QtMacKeyboardDemo
cd QtMacKeyboardDemo
qmake
make
```
## Запуск
```bash
./QtKeyboard.app/Contents/MacOS/QtKeyboard
```
