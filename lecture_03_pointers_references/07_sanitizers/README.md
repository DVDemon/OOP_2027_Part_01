# 07. Санитайзеры (`-fsanitize`)

**Пункт плана:** 7.

## Что показывает пример

* `main.cpp` — корректный код, который **проходит** AddressSanitizer и UBSan без ошибок.
* Как включить санитайзеры в этом проекте через CMake-опцию.
* Как выглядит диагностика санитайзера на коде с ошибкой.

## Запуск

Обычная сборка:

```bash
./build/07_sanitizers
```

Сборка проекта с санитайзерами (опция `ENABLE_SANITIZERS` определена в `CMakeLists.txt` лекции; ей собираются все примеры):

```bash
cmake -S . -B build -DENABLE_SANITIZERS=ON
cmake --build build
./build/07_sanitizers
```

Сборка одного файла вручную:

```bash
g++ -std=c++20 -fsanitize=address,undefined -g 07_sanitizers/main.cpp -o san
./san
```

## Что ловят санитайзеры

| Санитайзер | Флаг | Ловит |
|---|---|---|
| AddressSanitizer | `-fsanitize=address` | выход за границы, use-after-free, утечки* |
| UBSanitizer | `-fsanitize=undefined` | целочисленное переполнение, плохие приведения, и т.п. |
| LeakSanitizer | `-fsanitize=leak` | утечки памяти (на Linux входит в ASan) |

\* LeakSanitizer интегрирован в ASan на Linux; на macOS утечки могут не отображаться.

## Плохо → Хорошо

**Плохо** — собирать и тестировать без диагностики; ошибка проявится как редкий «плавающий» баг:

```cpp
int* arr = new int[10];
arr[10] = 42;     // выход за границу — без ASan может «случайно работать»
delete[] arr;
```

С ASan этот код немедленно даёт отчёт:

```
==12345==ERROR: AddressSanitizer: heap-buffer-overflow on address ...
WRITE of size 4 at ... thread T0
```

**Хорошо** — разрабатывать с `-fsanitize=address,undefined`:

```bash
cmake -S . -B build -DENABLE_SANITIZERS=ON && cmake --build build
```

**Почему:** санитайзеры превращают тихое неопределённое поведение в явный отчёт с точным местом
ошибки. Это самый дешёвый способ ловить ошибки памяти на этапе разработки.
