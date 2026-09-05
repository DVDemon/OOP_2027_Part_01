# Лекция 7. Copy/Move семантика. Правило пяти

## План лекции

1. Проблема поверхностного копирования (shallow copy)
2. Копирующий конструктор и копирующее присваивание
3. Правило трёх (Rule of Three)
4. Идиома copy-and-swap
5. rvalue-ссылки (`T&&`) и категории значений (lvalue, prvalue, xvalue)
6. Перемещающий конструктор и перемещающее присваивание
7. `std::move`, `std::swap` через move, `std::exchange`
8. Правило пяти (Rule of Five) и Правило нуля (Rule of Zero)
9. `noexcept` для move-операций
10. Полная реализация класса String

---

## 1. Проблема поверхностного копирования

Если класс владеет ресурсом через указатель, сгенерированный компилятором копирующий конструктор копирует **значение указателя**, а не данные:

```cpp
class NaiveString {
    char* data_;
    size_t size_;
public:
    NaiveString(const char* s)
        : size_(std::strlen(s))
        , data_(new char[size_ + 1]) {
        std::strcpy(data_, s);
    }
    ~NaiveString() { delete[] data_; }
    // Компилятор сгенерирует копирующий конструктор:
    // NaiveString(const NaiveString& o) : data_(o.data_), size_(o.size_) {}
    // ^^^ Два объекта указывают на один и тот же буфер!
};

void problem() {
    NaiveString a("Hello");
    NaiveString b = a; // shallow copy
    // При выходе: double free! деструктор b удалит data_,
    // затем деструктор a попытается удалить тот же указатель
}
```

---

## 2. Копирующий конструктор и копирующее присваивание

### Копирующий конструктор

```cpp
class String {
    char* data_;
    size_t size_;

public:
    // Копирующий конструктор — глубокая копия
    String(const String& other)
        : size_(other.size_)
        , data_(new char[other.size_ + 1]) {
        std::strcpy(data_, other.data_);
    }
};
```

### Копирующий оператор присваивания

```cpp
class String {
    // ...
public:
    // Наивная реализация (проблемы: самоприсваивание, exception safety)
    String& operator=(const String& other) {
        if (this == &other) return *this; // проверка самоприсваивания

        delete[] data_;                   // освободить старое

        size_ = other.size_;
        data_ = new char[size_ + 1];     // выделить новое
        std::strcpy(data_, other.data_);

        return *this;
    }
};
```

> **Проблема:** если `new` бросит исключение после `delete[] data_`, объект останется в невалидном состоянии (`data_` — dangling pointer).

---

## 3. Правило трёх (Rule of Three)

Если класс нуждается в пользовательском определении **хотя бы одного** из:
- Деструктора
- Копирующего конструктора
- Копирующего оператора присваивания

...то, скорее всего, нужны **все три**.

```cpp
class IntArray {
    int* data_;
    size_t size_;

public:
    IntArray(size_t n) : data_(new int[n]{}), size_(n) {}

    // 1. Деструктор
    ~IntArray() { delete[] data_; }

    // 2. Копирующий конструктор
    IntArray(const IntArray& other)
        : data_(new int[other.size_])
        , size_(other.size_) {
        std::copy(other.data_, other.data_ + size_, data_);
    }

    // 3. Копирующее присваивание
    IntArray& operator=(const IntArray& other) {
        if (this != &other) {
            int* new_data = new int[other.size_]; // сначала выделяем
            std::copy(other.data_, other.data_ + other.size_, new_data);
            delete[] data_;                        // потом освобождаем
            data_ = new_data;
            size_ = other.size_;
        }
        return *this;
    }
};
```

---

## 4. Идиома copy-and-swap

Элегантное решение проблем самоприсваивания и exception safety:

```cpp
class String {
    char* data_;
    size_t size_;

    friend void swap(String& a, String& b) noexcept {
        using std::swap;
        swap(a.data_, b.data_);
        swap(a.size_, b.size_);
    }

public:
    String(const char* s = "")
        : size_(std::strlen(s))
        , data_(new char[size_ + 1]) {
        std::strcpy(data_, s);
    }

    String(const String& other)
        : size_(other.size_)
        , data_(new char[other.size_ + 1]) {
        std::strcpy(data_, other.data_);
    }

    ~String() { delete[] data_; }

    // Copy-and-swap: параметр передаётся ПО ЗНАЧЕНИЮ
    String& operator=(String other) { // копия уже сделана!
        swap(*this, other);            // обмениваемся с копией
        return *this;                  // old данные уничтожатся с other
    }
};
```

Преимущества:
- **Нет проверки самоприсваивания** — корректно работает и так.
- **Exception safety** — если копирование (в параметре) бросит исключение, `*this` не изменится.
- **Переиспользование** — один оператор для копирования и перемещения.

Примеры:
- [04_copy_and_swap](04_copy_and_swap)

---

## 5. rvalue-ссылки и категории значений

### Категории значений в C++11

```
          expression
          /       \
       glvalue   rvalue
       /    \    /    \
    lvalue  xvalue  prvalue
```

- **lvalue** — имеет имя, можно взять адрес: `int x; std::string s;`
- **prvalue** — чистое rvalue, временный объект: `42`, `std::string("hi")`, `x + y`
- **xvalue** — «истекающее» значение: `std::move(x)`, возврат `T&&`

### rvalue-ссылки

```cpp
void process(int& x)  { std::cout << "lvalue: " << x << "\n"; }
void process(int&& x) { std::cout << "rvalue: " << x << "\n"; }

int a = 10;
process(a);          // lvalue: 10
process(42);         // rvalue: 42
process(std::move(a)); // rvalue: 10 (a приведён к xvalue)
```

rvalue-ссылка (`T&&`) привязывается к временным объектам и позволяет «красть» их ресурсы вместо копирования.

---

## 6. Перемещающий конструктор и перемещающее присваивание

```cpp
class String {
    char* data_;
    size_t size_;

public:
    // Перемещающий конструктор
    String(String&& other) noexcept
        : data_(other.data_)
        , size_(other.size_) {
        // «Обнуляем» источник — он должен быть в валидном, но неопределённом состоянии
        other.data_ = nullptr;
        other.size_ = 0;
    }

    // Перемещающее присваивание
    String& operator=(String&& other) noexcept {
        if (this != &other) {
            delete[] data_;          // освободить свои ресурсы
            data_ = other.data_;     // украсть чужие
            size_ = other.size_;
            other.data_ = nullptr;   // обнулить источник
            other.size_ = 0;
        }
        return *this;
    }
};
```

> После перемещения объект-источник должен быть **деструктуируемым** и **присваиваемым**. Обычно это «пустое» состояние.

---

## 7. `std::move`, `std::swap`, `std::exchange`

### `std::move`

`std::move` — это просто `static_cast<T&&>(x)`. Он НЕ перемещает — он **разрешает** перемещение:

```cpp
std::string a = "Hello";
std::string b = std::move(a); // перемещающий конструктор
// a теперь в valid-but-unspecified state (обычно пустая строка)
```

### `std::swap` через move

```cpp
template <typename T>
void swap(T& a, T& b) noexcept(
    std::is_nothrow_move_constructible_v<T> &&
    std::is_nothrow_move_assignable_v<T>) {
    T tmp = std::move(a);   // 1 move construction
    a = std::move(b);       // 1 move assignment
    b = std::move(tmp);     // 1 move assignment
    // Итого: 3 move-операции вместо 3 копирований
}
```

### `std::exchange` (C++14)

```cpp
// Заменяет значение и возвращает старое
template <class T, class U = T>
T exchange(T& obj, U&& new_value) {
    T old = std::move(obj);
    obj = std::forward<U>(new_value);
    return old;
}

// Удобно в move-конструкторе:
String(String&& other) noexcept
    : data_(std::exchange(other.data_, nullptr))
    , size_(std::exchange(other.size_, 0))
{}
```

---

## 8. Правило пяти и Правило нуля

### Правило пяти (Rule of Five)

Если определён один из пяти специальных методов, определите все пять:

```cpp
class Buffer {
    std::byte* data_;
    size_t size_;

public:
    explicit Buffer(size_t n) : data_(new std::byte[n]{}), size_(n) {}

    ~Buffer() { delete[] data_; }                         // 1

    Buffer(const Buffer& o)                               // 2
        : data_(new std::byte[o.size_]), size_(o.size_) {
        std::copy(o.data_, o.data_ + size_, data_);
    }

    Buffer& operator=(const Buffer& o) {                  // 3
        if (this != &o) {
            auto* tmp = new std::byte[o.size_];
            std::copy(o.data_, o.data_ + o.size_, tmp);
            delete[] data_;
            data_ = tmp;
            size_ = o.size_;
        }
        return *this;
    }

    Buffer(Buffer&& o) noexcept                           // 4
        : data_(std::exchange(o.data_, nullptr))
        , size_(std::exchange(o.size_, 0)) {}

    Buffer& operator=(Buffer&& o) noexcept {              // 5
        if (this != &o) {
            delete[] data_;
            data_ = std::exchange(o.data_, nullptr);
            size_ = std::exchange(o.size_, 0);
        }
        return *this;
    }
};
```

### Правило нуля (Rule of Zero)

Если класс не управляет ресурсами напрямую — **не определяйте ни одного** из пяти:

```cpp
class Person {
    std::string name_;     // std::string сам управляет памятью
    std::vector<int> ids_; // std::vector тоже
    int age_ = 0;

    // Компилятор генерирует корректные copy/move/dtor автоматически!
};
```

> **Совет:** используйте RAII-обёртки (`unique_ptr`, `shared_ptr`, `string`, `vector`) и следуйте Rule of Zero.

---

## 9. `noexcept` для move-операций

**Критически важно** помечать move-операции как `noexcept`:

```cpp
class Widget {
public:
    Widget(Widget&& other) noexcept;            // ОБЯЗАТЕЛЬНО noexcept
    Widget& operator=(Widget&& other) noexcept; // ОБЯЗАТЕЛЬНО noexcept
};
```

Почему?
- `std::vector::push_back` при реаллокации перемещает элементы **только если** move-конструктор `noexcept`.
- Без `noexcept` — будет **копировать** (для сильной гарантии исключений).
- Проверить: `std::is_nothrow_move_constructible_v<T>`.

```cpp
std::vector<Widget> v;
v.reserve(2);
v.push_back(Widget()); // move, если noexcept; copy, если нет
v.push_back(Widget()); // при реаллокации — то же правило
```

---

## 10. Полная реализация класса String

```cpp
#include <cstring>
#include <algorithm>
#include <iostream>
#include <utility>

class String {
    char* data_;
    size_t size_;
    size_t capacity_;

    friend void swap(String& a, String& b) noexcept {
        using std::swap;
        swap(a.data_, b.data_);
        swap(a.size_, b.size_);
        swap(a.capacity_, b.capacity_);
    }

public:
    // --- Конструкторы ---

    String() : data_(new char[1]{'\0'}), size_(0), capacity_(0) {}

    String(const char* s)
        : size_(std::strlen(s))
        , capacity_(size_)
        , data_(new char[size_ + 1]) {
        std::strcpy(data_, s);
    }

    // --- Правило пяти ---

    // 1. Деструктор
    ~String() { delete[] data_; }

    // 2. Копирующий конструктор
    String(const String& other)
        : size_(other.size_)
        , capacity_(other.capacity_)
        , data_(new char[other.capacity_ + 1]) {
        std::strcpy(data_, other.data_);
    }

    // 3. Копирующее присваивание (copy-and-swap)
    String& operator=(String other) { // по значению!
        swap(*this, other);
        return *this;
    }

    // 4. Перемещающий конструктор
    String(String&& other) noexcept
        : data_(std::exchange(other.data_, nullptr))
        , size_(std::exchange(other.size_, 0))
        , capacity_(std::exchange(other.capacity_, 0)) {
        other.data_ = new char[1]{'\0'}; // valid state
    }

    // Примечание: operator= по значению обрабатывает и move-присваивание
    // (параметр будет создан move-конструктором)

    // --- Методы ---

    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }
    const char* c_str() const { return data_; }
    bool empty() const { return size_ == 0; }

    char& operator[](size_t i) { return data_[i]; }
    const char& operator[](size_t i) const { return data_[i]; }

    String& operator+=(const String& other) {
        size_t new_size = size_ + other.size_;
        if (new_size > capacity_) {
            capacity_ = new_size * 2;
            char* new_data = new char[capacity_ + 1];
            std::strcpy(new_data, data_);
            delete[] data_;
            data_ = new_data;
        }
        std::strcat(data_, other.data_);
        size_ = new_size;
        return *this;
    }

    friend String operator+(String lhs, const String& rhs) {
        lhs += rhs;
        return lhs;
    }

    friend std::ostream& operator<<(std::ostream& os, const String& s) {
        return os << s.data_;
    }

    friend bool operator==(const String& a, const String& b) {
        return a.size_ == b.size_ && std::strcmp(a.data_, b.data_) == 0;
    }

    friend bool operator!=(const String& a, const String& b) {
        return !(a == b);
    }
};

// Пример использования:
// String s1("Hello");
// String s2(" World");
// String s3 = s1 + s2;         // copy + move
// String s4 = std::move(s3);   // move ctor
// std::cout << s4 << "\n";     // "Hello World"
```

---

## Ключевые концепции

| Концепция | Суть |
|-----------|------|
| Shallow copy | Копирование указателя, а не данных — double free |
| Deep copy | Выделение новой памяти и копирование содержимого |
| Copy-and-swap | Безопасное присваивание через копию + обмен |
| rvalue reference | `T&&` — привязка к временным объектам |
| `std::move` | Приведение к rvalue для разрешения перемещения |
| `std::exchange` | Замена значения с возвратом старого |
| Rule of Three | Dtor + Copy ctor + Copy assign |
| Rule of Five | Rule of Three + Move ctor + Move assign |
| Rule of Zero | Не определять ничего — делегировать RAII-членам |
| `noexcept` | Без него `vector` будет копировать вместо перемещения |

---

## Типичные ошибки

1. **Забыть обнулить источник при перемещении** — double free.
2. **Не пометить move-операции `noexcept`** — потеря производительности в контейнерах.
3. **`std::move` на `const`-объекте** — вызовется копирующий конструктор, а не перемещающий.
4. **Использование объекта после `std::move`** — UB нет, но значение неопределённо.
5. **Самоприсваивание без copy-and-swap** — `delete` до копирования.
6. **Определить только copy ctor, забыть copy assign** — нарушение Rule of Three.
7. **Не выделить new data_ в move ctor для источника** — деструктор источника вызовет `delete[] nullptr` (OK), но объект перестаёт быть «полноценным».

---

## Вопросы для самопроверки

1. Почему сгенерированный компилятором копирующий конструктор опасен для классов с `new`?
2. Как copy-and-swap решает проблему exception safety?
3. Чем `std::move` отличается от реального перемещения данных?
4. Что произойдёт, если вызвать `std::move` на `const std::string`?
5. Почему `noexcept` на move-конструкторе критично для `std::vector`?
6. Когда следовать Rule of Zero, а когда Rule of Five?
7. Объясните категории значений: lvalue, prvalue, xvalue. Приведите примеры каждого.
8. В чём преимущество `std::exchange` в move-конструкторе?

---

## Практическое задание

Реализуйте класс `DynamicArray<T>` (шаблон не обязателен, можно `int`):

1. Поля: `data_`, `size_`, `capacity_`.
2. Конструктор по умолчанию, из `initializer_list`, из `size_t count`.
3. Полное правило пяти (глубокая копия, move с `noexcept`).
4. Идиома copy-and-swap для оператора присваивания.
5. Методы: `push_back`, `pop_back`, `operator[]`, `at`, `size`, `capacity`, `empty`, `clear`.
6. `operator<<` для вывода (дружественная функция).
7. `operator==` и `operator!=`.
8. Проверьте корректность с помощью:
   - Копирования: `DynamicArray b = a;`
   - Перемещения: `DynamicArray c = std::move(a);`
   - Самоприсваивания: `a = a;`
   - Вставки в `std::vector<DynamicArray>` (проверить, что используется move).
