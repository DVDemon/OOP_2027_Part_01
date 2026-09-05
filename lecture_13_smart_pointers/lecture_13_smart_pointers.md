# Лекция 13. Умные указатели

## План лекции

1. Мотивация: ручное управление памятью + исключения = утечки
2. `std::unique_ptr`: эксклюзивное владение, `make_unique`, пользовательский deleter
3. `std::shared_ptr`: разделяемое владение, подсчёт ссылок, `make_shared`
4. `std::weak_ptr`: борьба с циклическими ссылками
5. `enable_shared_from_this`
6. `dynamic_pointer_cast` / `static_pointer_cast`
7. Реализация собственного `unique_ptr` (учебная)
8. Реализация собственного `shared_ptr` (учебная)
9. Умные указатели и полиморфизм
10. Практические рекомендации и типичные ошибки

---

## 1. Мотивация

Ручное управление памятью в C++ при наличии исключений неизбежно ведёт к утечкам:

```cpp
void dangerous() {
    int* a = new int(10);
    double* b = new double(3.14);
    std::string* s = new std::string("hello");

    process(a, b, s);  // Что если здесь исключение?

    delete s;  // Не выполнится при исключении!
    delete b;
    delete a;
}

// RAII-решение:
void safe() {
    auto a = std::make_unique<int>(10);
    auto b = std::make_unique<double>(3.14);
    auto s = std::make_unique<std::string>("hello");

    process(a.get(), b.get(), s.get());
    // Деструкторы unique_ptr освободят память автоматически
}
```

**Принцип:** в современном C++ `new` и `delete` не должны появляться в пользовательском коде. Используйте умные указатели.

---

## 2. `std::unique_ptr`

`unique_ptr` реализует эксклюзивное (единоличное) владение ресурсом.

```cpp
#include <memory>

// Создание
auto p1 = std::make_unique<int>(42);          // предпочтительный способ
std::unique_ptr<int> p2(new int(42));          // допустимо, но менее безопасно
std::unique_ptr<int[]> arr = std::make_unique<int[]>(100);  // массив

// Основные операции
std::cout << *p1 << std::endl;       // 42
int* raw = p1.get();                  // получить сырой указатель (не передаёт владение)
p1.reset();                           // освободить ресурс, p1 == nullptr
p1.reset(new int(100));               // освободить старый, владеть новым
int* released = p1.release();         // отказаться от владения (вызывающий должен delete)
delete released;

// Перемещение (копирование запрещено!)
auto a = std::make_unique<int>(1);
// auto b = a;                        // Ошибка: копирование запрещено
auto b = std::move(a);                // OK: a теперь nullptr
```

### Пользовательский deleter

```cpp
// Для файлов
auto fileDeleter = [](FILE* f) {
    if (f) {
        std::cout << "Закрываем файл" << std::endl;
        fclose(f);
    }
};
std::unique_ptr<FILE, decltype(fileDeleter)> file(fopen("data.txt", "r"), fileDeleter);

// Для C-библиотек
struct SDL_Deleter {
    void operator()(SDL_Window* w) const { SDL_DestroyWindow(w); }
    void operator()(SDL_Renderer* r) const { SDL_DestroyRenderer(r); }
};
std::unique_ptr<SDL_Window, SDL_Deleter> window(SDL_CreateWindow(/*...*/));
```

> Примеры: [15_UniquePointers](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection06_07/15_UniquePointers), [16_CustomUniquePointer](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection06_07/16_CustomUniquePointer)

---

## 3. `std::shared_ptr`

`shared_ptr` реализует разделяемое владение через подсчёт ссылок:

```cpp
#include <memory>

auto sp1 = std::make_shared<int>(42);
std::cout << sp1.use_count() << std::endl;  // 1

{
    auto sp2 = sp1;  // Копирование -- счётчик увеличивается
    std::cout << sp1.use_count() << std::endl;  // 2
    std::cout << sp2.use_count() << std::endl;  // 2
}  // sp2 разрушен, счётчик уменьшается

std::cout << sp1.use_count() << std::endl;  // 1
// Когда счётчик станет 0 -- объект удалится
```

### `make_shared` vs конструктор

```cpp
// Предпочтительно: одна аллокация (объект + control block)
auto p1 = std::make_shared<Widget>(arg1, arg2);

// Менее эффективно: две аллокации
std::shared_ptr<Widget> p2(new Widget(arg1, arg2));

// Опасно: если между new и конструктором shared_ptr возникнет исключение
void f(std::shared_ptr<Widget> a, std::shared_ptr<Widget> b);
// f(std::shared_ptr<Widget>(new Widget), std::shared_ptr<Widget>(new Widget));
// Порядок вычисления аргументов не определён -- потенциальная утечка!
// С make_shared такой проблемы нет:
f(std::make_shared<Widget>(), std::make_shared<Widget>());
```

### Внутреннее устройство

```
shared_ptr<T> sp:
+----------+
| ptr  -----------> T object
| ctrl -----------> Control Block
+----------+        +-------------------+
                    | strong_count: 2   |
                    | weak_count: 1     |
                    | deleter           |
                    | allocator         |
                    +-------------------+
```

> Примеры: [17_SharedPtr](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection06_07/17_SharedPtr), [18_CustomShared](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection06_07/18_CustomShared)

---

## 4. `std::weak_ptr`

`weak_ptr` -- не-владеющий наблюдатель за `shared_ptr`. Решает проблему циклических ссылок:

```cpp
// ПРОБЛЕМА: циклическая ссылка -- утечка памяти
struct Node {
    std::shared_ptr<Node> next;
    std::shared_ptr<Node> prev;  // Цикл! Ни один Node не освободится
    ~Node() { std::cout << "~Node" << std::endl; }
};

void leak() {
    auto a = std::make_shared<Node>();
    auto b = std::make_shared<Node>();
    a->next = b;
    b->prev = a;  // Цикл: a -> b -> a
    // При выходе: use_count у обоих == 1 (из-за цикла)
    // Деструкторы НЕ вызовутся!
}

// РЕШЕНИЕ: weak_ptr разрывает цикл
struct NodeFixed {
    std::shared_ptr<NodeFixed> next;
    std::weak_ptr<NodeFixed> prev;   // weak не увеличивает счётчик!
    ~NodeFixed() { std::cout << "~NodeFixed" << std::endl; }
};

void noLeak() {
    auto a = std::make_shared<NodeFixed>();
    auto b = std::make_shared<NodeFixed>();
    a->next = b;
    b->prev = a;  // weak_ptr -- не участвует в подсчёте ссылок
}  // Деструкторы вызовутся корректно
```

### Использование `weak_ptr`

```cpp
auto shared = std::make_shared<int>(42);
std::weak_ptr<int> weak = shared;

// Проверка живости
std::cout << weak.expired() << std::endl;  // false

// Получение shared_ptr из weak_ptr
if (auto locked = weak.lock()) {
    std::cout << *locked << std::endl;  // 42
} else {
    std::cout << "Объект уже удалён" << std::endl;
}

shared.reset();  // Удаляем объект
std::cout << weak.expired() << std::endl;  // true
auto locked = weak.lock();  // Возвращает пустой shared_ptr
```

> Примеры: [22_Weak_ptr](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection06_07/22_Weak_ptr), [23_Weak_ptr_deadlock](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection06_07/23_Weak_ptr_deadlock), [21_Dead_lock](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection06_07/21_Dead_lock)

---

## 5. `enable_shared_from_this`

Позволяет объекту безопасно получить `shared_ptr` на самого себя:

```cpp
class Server : public std::enable_shared_from_this<Server> {
public:
    void startAsync() {
        // Нужно передать shared_ptr на себя в callback
        auto self = shared_from_this();
        asyncOperation([self]() {
            self->onComplete();
        });
    }

    void onComplete() {
        std::cout << "Операция завершена" << std::endl;
    }
};

int main() {
    auto server = std::make_shared<Server>();
    server->startAsync();
    // server -- ОБЯЗАТЕЛЬНО shared_ptr, иначе shared_from_this() бросит исключение
}
```

**Ограничение:** нельзя вызывать `shared_from_this()` из конструктора -- `shared_ptr` ещё не создан.

> Примеры: [20_Enable_shared_from_this](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection06_07/20_Enable_shared_from_this)

---

## 6. `dynamic_pointer_cast` / `static_pointer_cast`

Приведение типов для `shared_ptr`:

```cpp
class Base {
public:
    virtual ~Base() = default;
    virtual void info() { std::cout << "Base" << std::endl; }
};

class Derived : public Base {
public:
    void info() override { std::cout << "Derived" << std::endl; }
    void special() { std::cout << "Derived::special" << std::endl; }
};

int main() {
    std::shared_ptr<Base> base = std::make_shared<Derived>();

    // static_pointer_cast -- без проверки (как static_cast)
    auto derived1 = std::static_pointer_cast<Derived>(base);
    derived1->special();

    // dynamic_pointer_cast -- с проверкой (как dynamic_cast)
    auto derived2 = std::dynamic_pointer_cast<Derived>(base);
    if (derived2) {
        derived2->special();
    }

    // Важно: все shared_ptr разделяют один control block
    std::cout << base.use_count() << std::endl;  // 3

    // Для unique_ptr (C++17 нет, но можно вручную):
    std::unique_ptr<Base> up = std::make_unique<Derived>();
    Derived* raw = dynamic_cast<Derived*>(up.get());
    if (raw) raw->special();
}
```

> Примеры: [19_Dynamic_pointer_cast](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection06_07/19_Dynamic_pointer_cast)

---

## 7. Реализация собственного `unique_ptr` (учебная)

```cpp
template<typename T>
class UniquePtr {
    T* ptr;
public:
    // Конструкторы
    explicit UniquePtr(T* p = nullptr) : ptr(p) {}

    // Запрет копирования
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // Перемещение
    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    // Деструктор
    ~UniquePtr() { delete ptr; }

    // Операторы доступа
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    T* get() const { return ptr; }

    // Проверка на nullptr
    explicit operator bool() const { return ptr != nullptr; }

    // Управление
    T* release() {
        T* tmp = ptr;
        ptr = nullptr;
        return tmp;
    }

    void reset(T* p = nullptr) {
        if (ptr != p) {
            delete ptr;
            ptr = p;
        }
    }

    void swap(UniquePtr& other) noexcept {
        std::swap(ptr, other.ptr);
    }
};

// Аналог make_unique
template<typename T, typename... Args>
UniquePtr<T> makeUnique(Args&&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

// Использование
int main() {
    auto p = makeUnique<std::string>("Hello");
    std::cout << *p << std::endl;          // "Hello"
    std::cout << p->size() << std::endl;   // 5

    auto p2 = std::move(p);
    // p теперь nullptr
    std::cout << (p ? "not null" : "null") << std::endl;  // "null"
}
```

---

## 8. Реализация собственного `shared_ptr` (учебная)

```cpp
template<typename T>
class SharedPtr {
    T* ptr;
    size_t* ref_count;

    void release() {
        if (ref_count) {
            --(*ref_count);
            if (*ref_count == 0) {
                delete ptr;
                delete ref_count;
            }
        }
        ptr = nullptr;
        ref_count = nullptr;
    }

public:
    // Конструкторы
    explicit SharedPtr(T* p = nullptr)
        : ptr(p), ref_count(p ? new size_t(1) : nullptr) {}

    // Копирование -- увеличиваем счётчик
    SharedPtr(const SharedPtr& other)
        : ptr(other.ptr), ref_count(other.ref_count) {
        if (ref_count) ++(*ref_count);
    }

    // Перемещение -- "крадём" ресурс
    SharedPtr(SharedPtr&& other) noexcept
        : ptr(other.ptr), ref_count(other.ref_count) {
        other.ptr = nullptr;
        other.ref_count = nullptr;
    }

    // Деструктор
    ~SharedPtr() { release(); }

    // Присваивание копированием
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            ref_count = other.ref_count;
            if (ref_count) ++(*ref_count);
        }
        return *this;
    }

    // Присваивание перемещением
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            release();
            ptr = other.ptr;
            ref_count = other.ref_count;
            other.ptr = nullptr;
            other.ref_count = nullptr;
        }
        return *this;
    }

    // Операторы доступа
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    T* get() const { return ptr; }

    size_t use_count() const { return ref_count ? *ref_count : 0; }
    explicit operator bool() const { return ptr != nullptr; }

    void reset(T* p = nullptr) {
        release();
        ptr = p;
        ref_count = p ? new size_t(1) : nullptr;
    }
};

int main() {
    SharedPtr<int> a(new int(42));
    std::cout << "count: " << a.use_count() << std::endl;  // 1

    {
        SharedPtr<int> b = a;
        std::cout << "count: " << a.use_count() << std::endl;  // 2
        std::cout << *b << std::endl;  // 42
    }

    std::cout << "count: " << a.use_count() << std::endl;  // 1
}
```

**Примечание:** настоящий `std::shared_ptr` использует атомарные операции для потокобезопасности, хранит deleter и allocator в control block, поддерживает `weak_ptr`.

---

## 9. Умные указатели и полиморфизм

Умные указатели прекрасно работают с полиморфными иерархиями:

```cpp
class Shape {
public:
    virtual double area() const = 0;
    virtual void draw() const = 0;
    virtual ~Shape() = default;
};

class Circle : public Shape {
    double r;
public:
    Circle(double r) : r(r) {}
    double area() const override { return 3.14159 * r * r; }
    void draw() const override { std::cout << "Circle(r=" << r << ")" << std::endl; }
};

class Rect : public Shape {
    double w, h;
public:
    Rect(double w, double h) : w(w), h(h) {}
    double area() const override { return w * h; }
    void draw() const override { std::cout << "Rect(" << w << "x" << h << ")" << std::endl; }
};

int main() {
    // unique_ptr и полиморфизм
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Rect>(3.0, 4.0));

    for (const auto& s : shapes) {
        s->draw();
        std::cout << "  area = " << s->area() << std::endl;
    }

    // shared_ptr и полиморфизм + dynamic_pointer_cast
    std::shared_ptr<Shape> shape = std::make_shared<Circle>(10.0);
    if (auto circle = std::dynamic_pointer_cast<Circle>(shape)) {
        std::cout << "Это круг!" << std::endl;
    }

    // Фабрика с unique_ptr
    auto createShape(const std::string& type) -> std::unique_ptr<Shape>;
}
```

**Когда что использовать:**

| Ситуация | Указатель |
|---|---|
| Единоличное владение (95% случаев) | `unique_ptr` |
| Разделяемое владение | `shared_ptr` |
| Наблюдатель без владения (кэш, observer) | `weak_ptr` |
| Не-владеющая ссылка (параметр функции) | Сырой указатель или ссылка |

---

## 10. Практические рекомендации

- **Предпочитайте `unique_ptr`** -- он легче, быстрее, проще. Переходите к `shared_ptr` только при необходимости
- **Всегда используйте `make_unique`/`make_shared`** -- они безопаснее и эффективнее
- **Не создавайте `shared_ptr` из сырого указателя, на который уже есть `shared_ptr`** -- двойной `delete`
- **Передавайте `unique_ptr` по значению при передаче владения** (`void consume(std::unique_ptr<T> p)`)
- **Передавайте `shared_ptr` по `const&` если не нужно увеличивать счётчик**
- **Используйте `weak_ptr` для разрыва циклов** -- типичный случай: двусвязные списки, граф, observer

---

## Типичные ошибки

1. **Два `shared_ptr` из одного `new`** -- двойное освобождение:
```cpp
int* raw = new int(42);
std::shared_ptr<int> sp1(raw);
std::shared_ptr<int> sp2(raw);  // Катастрофа! Два независимых control block
```

2. **Циклические ссылки с `shared_ptr`** -- утечка памяти (используйте `weak_ptr`)

3. **Вызов `shared_from_this()` без `shared_ptr`:**
```cpp
auto obj = new MyClass();
obj->shared_from_this();  // UB! Нет shared_ptr, управляющего obj
```

4. **Хранение `this` в `shared_ptr`:**
```cpp
class Bad {
    std::shared_ptr<Bad> self;
    Bad() : self(this) {}  // УЖАС: shared_ptr удалит объект в стеке
};
```

5. **`unique_ptr` в контейнере без `std::move`:**
```cpp
std::vector<std::unique_ptr<int>> v;
auto p = std::make_unique<int>(42);
// v.push_back(p);              // Ошибка: копирование запрещено
v.push_back(std::move(p));      // OK
```

---

## Вопросы для самопроверки

1. Почему `make_shared` эффективнее `shared_ptr(new T())`?
2. Можно ли скопировать `unique_ptr`? А переместить? Почему?
3. Что произойдёт при циклической ссылке `shared_ptr`? Как это решить?
4. Когда нужен `enable_shared_from_this`? Какие у него ограничения?
5. Чем `dynamic_pointer_cast` отличается от обычного `dynamic_cast`?
6. Реализуйте `UniquePtr` -- какие специальные функции нужно определить?
7. Почему в учебной реализации `SharedPtr` нет потокобезопасности?
8. Когда уместно передавать сырой указатель вместо умного?

---

## Практическое задание

1. Реализуйте двусвязный список на `shared_ptr` / `weak_ptr`:
   - `next` -- `shared_ptr<Node<T>>`
   - `prev` -- `weak_ptr<Node<T>>`
   - Методы: `push_front`, `push_back`, `pop_front`, `pop_back`, `print`
   - Убедитесь, что все узлы корректно освобождаются (выведите деструкторы)

2. Реализуйте свои `UniquePtr<T>` и `SharedPtr<T>`:
   - `UniquePtr`: конструкторы, перемещение, `release`, `reset`, `operator*`, `operator->`
   - `SharedPtr`: конструкторы, копирование, перемещение, `use_count`, `reset`
   - Покройте тестами: создание, копирование/перемещение, разрушение, полиморфизм

3. Создайте полиморфную коллекцию `vector<unique_ptr<Shape>>` с фабричным методом и покажите, что при исключении в процессе заполнения вектора утечек памяти нет
