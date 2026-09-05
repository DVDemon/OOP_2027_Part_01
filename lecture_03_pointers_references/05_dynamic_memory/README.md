# 05. Динамическая память: `new`/`delete`, `new[]`/`delete[]`

**Пункт плана:** 5 (а также практическое задание лекции).

## Что показывает пример

* Выделение и освобождение одного объекта (`new`/`delete`) и массива (`new[]`/`delete[]`).
* Value-initialization (`new int[N]{}`) — гарантированные нули.
* Ручной динамический массив (`dynamic_array.h/.cpp`): `create/resize/insert/remove/destroy`.

## Запуск

```bash
./build/05_dynamic_memory
```

Сборка с санитайзерами (см. 07):

```bash
cmake -S . -B build -DENABLE_SANITIZERS=ON && cmake --build build
```

## Плохо → Хорошо

**Плохо** — смешивать `new[]` и `delete` (или `new` и `delete[]`) — неопределённое поведение:

```cpp
int* arr = new int[10];
delete arr;     // UB! нужно delete[]
```

**Хорошо** — парные операторы и (в реальном коде) контейнеры/умные указатели:

```cpp
int* arr = new int[10];
delete[] arr;                       // парный delete[]

// Ещё лучше — пусть памятью управляет контейнер:
std::vector<int> v(10);             // освобождение автоматическое
```

**Почему:** `new`/`new[]` и `delete`/`delete[]` — разные операторы (одиночный объект vs массив). Их смешивание — UB. В современном C++ ручной `new`/`delete` минимизируют в пользу `std::vector` и умных указателей.
