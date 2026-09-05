# 10. Полиморфные коллекции и паттерны Factory Method, Strategy

**Пункт плана:** 10 — полиморфные коллекции (`vector<unique_ptr<Base>>`),
паттерны Factory Method и Strategy.

## Что показывает пример

* Хранение полиморфных объектов в `std::vector<std::unique_ptr<Shape>>`
  (`Canvas`) — без срезки, с автоматическим владением.
* **Factory Method** (`createShape`) — инкапсулирует создание объектов:
  клиент не знает конкретные классы.
* **Strategy** (`Sorter` + `SortStrategy`) — выбор алгоритма во время
  выполнения, сменяемый «на лету».
* Разделение на `shapes.h` / `shapes.cpp` / `main.cpp`.

## Сборка и запуск

```bash
cmake -S .. -B ../build       # из папки лекции
cmake --build ../build
./../build/10_polymorphic_collections_demo
```

Цели: `10_polymorphic_collections_demo` — демонстрация в консоли.

## Плохо → Хорошо

**Плохо** — хранить полиморфные объекты по значению — это срезка (slicing):

```cpp
std::vector<Shape> shapes;          // (если бы Shape был не абстрактным)
shapes.push_back(Circle{5.0});      // в вектор скопируется ТОЛЬКО часть Shape
// area() вызовет версию базы — данные и поведение Circle потеряны
```

**Хорошо** — хранить указатели с владением:

```cpp
std::vector<std::unique_ptr<Shape>> shapes;
shapes.push_back(std::make_unique<Circle>(5.0));   // полиморфизм сохранён
shapes.push_back(createShape("rectangle", 3, 4));  // + Factory Method
```

**Почему:** контейнер значений хранит фиксированный тип `Shape` — при
копировании потомка «обрезается» его производная часть (slicing), полиморфизм
теряется. `unique_ptr<Shape>` хранит указатель на полный объект, виртуальные
вызовы работают, а время жизни управляется автоматически. Factory Method
прячет `new`/конкретные классы за функцией, а Strategy выносит сменяемый
алгоритм в отдельный полиморфный объект.
