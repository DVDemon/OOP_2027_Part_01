# Лекция 8. Перегрузка операторов

## План лекции

1. Какие операторы можно и нельзя перегружать. Член класса vs свободная функция
2. Арифметические операторы (`+`, `-`, `*`, `/`) и составные (`+=`, `-=`)
3. Префиксный и постфиксный `++`/`--`
4. Операторы сравнения и `operator<=>` (C++20)
5. Операторы ввода/вывода (`<<`, `>>`)
6. Оператор индексирования `[]` (const/non-const)
7. Оператор вызова `()` — функторы
8. Оператор `->` (proxy-объекты)
9. Операторы преобразования типа (`operator T()`), `explicit` conversions
10. Пользовательские литералы и канонические формы

---

## 1. Правила перегрузки операторов

### Нельзя перегрузить
`::`, `.`, `.*`, `?:`, `sizeof`, `typeid`, `alignof`

### Только как метод класса
`=`, `()`, `[]`, `->`, `->*`, приведение типа

### Можно как метод или свободную функцию
Все остальные: `+`, `-`, `*`, `<<`, `==`, `<` и т.д.

### Когда метод, когда свободная функция?

```cpp
class Fraction {
    int num_, den_;
public:
    Fraction(int n = 0, int d = 1) : num_(n), den_(d) {}

    // Метод: левый операнд — всегда Fraction
    Fraction operator*(const Fraction& rhs) const {
        return Fraction(num_ * rhs.num_, den_ * rhs.den_);
    }

    // Проблема: 2 * frac не скомпилируется (левый операнд — int)
    // Решение: свободная функция
    friend Fraction operator*(int lhs, const Fraction& rhs) {
        return Fraction(lhs * rhs.num_, rhs.den_);
    }

    int num() const { return num_; }
    int den() const { return den_; }
};
```

> **Правило:** если оператор должен допускать неявное преобразование левого операнда — делайте свободную функцию.

Примеры:
- [01_member_vs_free](01_member_vs_free)

---

## 2. Арифметические и составные операторы

Канонический подход: реализовать `+=`, затем `+` через него.

```cpp
class Vector2D {
    double x_, y_;
public:
    Vector2D(double x = 0, double y = 0) : x_(x), y_(y) {}

    // Составной оператор — модифицирует *this, возвращает ссылку
    Vector2D& operator+=(const Vector2D& rhs) {
        x_ += rhs.x_;
        y_ += rhs.y_;
        return *this;
    }

    Vector2D& operator-=(const Vector2D& rhs) {
        x_ -= rhs.x_;
        y_ -= rhs.y_;
        return *this;
    }

    Vector2D& operator*=(double scalar) {
        x_ *= scalar;
        y_ *= scalar;
        return *this;
    }

    // Унарный минус
    Vector2D operator-() const {
        return Vector2D(-x_, -y_);
    }

    // Бинарные операторы через составные (свободные функции)
    friend Vector2D operator+(Vector2D lhs, const Vector2D& rhs) {
        lhs += rhs; // lhs — копия, модифицируем и возвращаем
        return lhs;
    }

    friend Vector2D operator-(Vector2D lhs, const Vector2D& rhs) {
        lhs -= rhs;
        return lhs;
    }

    friend Vector2D operator*(Vector2D v, double s) {
        v *= s;
        return v;
    }

    friend Vector2D operator*(double s, Vector2D v) {
        v *= s;
        return v;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
        return os << "(" << v.x_ << ", " << v.y_ << ")";
    }
};
```

Примеры:
- [02_arithmetic_compound](02_arithmetic_compound)

---

## 3. Префиксный и постфиксный `++`/`--`

```cpp
class Counter {
    int value_;
public:
    explicit Counter(int v = 0) : value_(v) {}

    // Префиксный: ++c (возвращает ссылку на изменённый объект)
    Counter& operator++() {
        ++value_;
        return *this;
    }

    // Постфиксный: c++ (принимает int-заглушку, возвращает копию ДО изменения)
    Counter operator++(int) {
        Counter old = *this;
        ++(*this); // используем префиксный
        return old;
    }

    // Аналогично для --
    Counter& operator--() {
        --value_;
        return *this;
    }

    Counter operator--(int) {
        Counter old = *this;
        --(*this);
        return old;
    }

    int value() const { return value_; }
};

// Counter c(5);
// ++c;        // c.value() == 6
// c++;        // возвращает Counter(6), c.value() == 7
```

> **Предпочитайте `++i`** — постфиксный создаёт копию.

Примеры:
- [03_increment_decrement](03_increment_decrement)

---

## 4. Операторы сравнения и `operator<=>` (C++20)

### До C++20: вручную

```cpp
class Date {
    int year_, month_, day_;
public:
    Date(int y, int m, int d) : year_(y), month_(m), day_(d) {}

    bool operator==(const Date& o) const {
        return year_ == o.year_ && month_ == o.month_ && day_ == o.day_;
    }
    bool operator!=(const Date& o) const { return !(*this == o); }
    bool operator<(const Date& o) const {
        if (year_ != o.year_) return year_ < o.year_;
        if (month_ != o.month_) return month_ < o.month_;
        return day_ < o.day_;
    }
    bool operator>(const Date& o) const { return o < *this; }
    bool operator<=(const Date& o) const { return !(o < *this); }
    bool operator>=(const Date& o) const { return !(*this < o); }
};
```

### C++20: spaceship operator

```cpp
#include <compare>

class Date {
    int year_, month_, day_;
public:
    Date(int y, int m, int d) : year_(y), month_(m), day_(d) {}

    // Одна строка заменяет все 6 операторов!
    auto operator<=>(const Date&) const = default;
};

// Теперь работает:
// Date d1(2025, 1, 1), d2(2025, 6, 15);
// d1 < d2   -> true
// d1 == d2  -> false
// d1 >= d2  -> false
```

Если нужна пользовательская логика:

```cpp
class CaseInsensitiveString {
    std::string data_;
public:
    explicit CaseInsensitiveString(std::string s) : data_(std::move(s)) {}

    std::strong_ordering operator<=>(const CaseInsensitiveString& other) const {
        // Посимвольное сравнение без учёта регистра
        auto to_lower = [](unsigned char c) { return std::tolower(c); };
        auto it1 = data_.begin(), it2 = other.data_.begin();
        for (; it1 != data_.end() && it2 != other.data_.end(); ++it1, ++it2) {
            auto c1 = to_lower(*it1), c2 = to_lower(*it2);
            if (c1 < c2) return std::strong_ordering::less;
            if (c1 > c2) return std::strong_ordering::greater;
        }
        return data_.size() <=> other.data_.size();
    }

    bool operator==(const CaseInsensitiveString& other) const {
        // operator== нужно определить отдельно при пользовательском <=>
        return (*this <=> other) == std::strong_ordering::equal;
    }
};
```

---

## 5. Операторы ввода/вывода

Всегда свободные функции (левый операнд — `std::ostream`/`std::istream`):

```cpp
class Complex {
    double re_, im_;
public:
    Complex(double r = 0, double i = 0) : re_(r), im_(i) {}

    // Оператор вывода
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << c.re_;
        if (c.im_ >= 0) os << "+";
        os << c.im_ << "i";
        return os; // для цепочки: cout << c1 << " " << c2
    }

    // Оператор ввода
    friend std::istream& operator>>(std::istream& is, Complex& c) {
        // Формат: "3.0 4.5"
        is >> c.re_ >> c.im_;
        return is;
    }
};

// std::cout << Complex(3, -4) << "\n"; // "3-4i"
// Complex c; std::cin >> c;
```

Примеры:
- [05_io_operators](05_io_operators)

---

## 6. Оператор `[]` — индексирование

Нужны **две версии**: для `const` и для модифицируемого доступа.

```cpp
class Matrix {
    std::vector<double> data_;
    size_t rows_, cols_;

public:
    Matrix(size_t r, size_t c) : data_(r * c, 0.0), rows_(r), cols_(c) {}

    // Proxy-класс для синтаксиса m[i][j]
    class Row {
        double* data_;
        size_t cols_;
    public:
        Row(double* d, size_t c) : data_(d), cols_(c) {}
        double& operator[](size_t j) {
            if (j >= cols_) throw std::out_of_range("column index");
            return data_[j];
        }
        const double& operator[](size_t j) const {
            if (j >= cols_) throw std::out_of_range("column index");
            return data_[j];
        }
    };

    Row operator[](size_t i) {
        if (i >= rows_) throw std::out_of_range("row index");
        return Row(data_.data() + i * cols_, cols_);
    }

    // const-версия
    class ConstRow {
        const double* data_;
        size_t cols_;
    public:
        ConstRow(const double* d, size_t c) : data_(d), cols_(c) {}
        const double& operator[](size_t j) const {
            if (j >= cols_) throw std::out_of_range("column index");
            return data_[j];
        }
    };

    ConstRow operator[](size_t i) const {
        if (i >= rows_) throw std::out_of_range("row index");
        return ConstRow(data_.data() + i * cols_, cols_);
    }
};

// Matrix m(3, 4);
// m[1][2] = 5.0;           // запись
// double v = m[1][2];      // чтение
```

Примеры:
- [06_subscript](06_subscript)

---

## 7. Оператор `()` — функторы

Объект, для которого определён `operator()`, называется **функтором** (функциональный объект).

```cpp
class Adder {
    int offset_;
public:
    explicit Adder(int offset) : offset_(offset) {}

    int operator()(int x) const {
        return x + offset_;
    }
};

// Adder add5(5);
// add5(10); // 15

// Использование с алгоритмами STL:
// std::vector<int> v = {1, 2, 3, 4, 5};
// std::transform(v.begin(), v.end(), v.begin(), Adder(10));
// v: {11, 12, 13, 14, 15}
```

Более сложный пример — предикат:

```cpp
class InRange {
    double lo_, hi_;
public:
    InRange(double lo, double hi) : lo_(lo), hi_(hi) {}

    bool operator()(double x) const {
        return x >= lo_ && x <= hi_;
    }
};

// std::vector<double> data = {1.5, 3.7, 2.1, 8.0, 4.2};
// auto count = std::count_if(data.begin(), data.end(), InRange(2.0, 5.0));
// count == 3
```

Функторы могут хранить состояние (в отличие от обычных функций):

```cpp
class CallCounter {
    mutable int count_ = 0;
public:
    void operator()(int) const { ++count_; }
    int count() const { return count_; }
};
```

Примеры:
- [07_call_functor](07_call_functor)

---

## 8. Оператор `->` — proxy-объекты

`operator->` должен возвращать либо указатель, либо объект, у которого тоже есть `operator->` (цепочка).

```cpp
template <typename T>
class SmartPtr {
    T* ptr_;
public:
    explicit SmartPtr(T* p = nullptr) : ptr_(p) {}
    ~SmartPtr() { delete ptr_; }

    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }

    SmartPtr(const SmartPtr&) = delete;
    SmartPtr& operator=(const SmartPtr&) = delete;
};

// struct Widget { void doWork() {} };
// SmartPtr<Widget> p(new Widget);
// p->doWork(); // вызов через operator->
```

Пример proxy для логирования:

```cpp
template <typename T>
class LoggingProxy {
    T* ptr_;
public:
    LoggingProxy(T* p) : ptr_(p) {
        std::cout << "Access start\n";
    }
    ~LoggingProxy() {
        std::cout << "Access end\n";
    }
    T* operator->() { return ptr_; }
};

template <typename T>
class LoggingWrapper {
    T obj_;
public:
    template <typename... Args>
    LoggingWrapper(Args&&... args) : obj_(std::forward<Args>(args)...) {}

    LoggingProxy<T> operator->() {
        return LoggingProxy<T>(&obj_);
    }
};
```

---

## 9. Операторы преобразования типа

```cpp
class Fraction {
    int num_, den_;
public:
    Fraction(int n, int d = 1) : num_(n), den_(d) {}

    // Неявное преобразование в double
    operator double() const {
        return static_cast<double>(num_) / den_;
    }

    // Явное преобразование в bool (C++11)
    explicit operator bool() const {
        return num_ != 0;
    }
};

Fraction f(3, 4);
double d = f;       // OK: неявная конверсия -> 0.75
// bool b = f;      // Ошибка: explicit!
if (f) { /* OK */ } // explicit operator bool работает в условиях
```

### Ловушка: неявные преобразования

```cpp
class BadString {
    char* data_;
public:
    BadString(const char* s) { /* ... */ }  // конвертирующий ctor (без explicit)
    operator const char*() const { return data_; } // неявная конверсия
};

// BadString s("hello");
// if (s == "hello") { ... } // Сравниваются УКАЗАТЕЛИ, а не строки!
// Компилятор преобразует s в const char* и сравнивает адреса
```

> **Правило:** операторы преобразования должны быть `explicit`, если только преобразование не является абсолютно естественным.

Примеры:
- [09_conversions](09_conversions)

---

## 10. Пользовательские литералы и канонические формы

### Пользовательские литералы (C++11)

```cpp
// Должны начинаться с _
constexpr long double operator""_km(long double val) {
    return val * 1000.0L; // в метрах
}

constexpr long double operator""_m(long double val) {
    return val;
}

constexpr long double operator""_cm(long double val) {
    return val / 100.0L;
}

// auto distance = 5.0_km + 200.0_m + 50.0_cm;
// distance == 5200.005
```

Для строк (C++14 — `std::string` литералы):

```cpp
using namespace std::string_literals;
auto s = "Hello"s; // std::string, а не const char*
```

### Канонические формы операторов

| Оператор | Форма | Возврат | Заметка |
|----------|-------|---------|---------|
| `+` | `friend T operator+(T lhs, const T& rhs)` | `T` | Через `+=` |
| `+=` | `T& operator+=(const T& rhs)` | `T&` | Метод |
| `++` (pre) | `T& operator++()` | `T&` | Метод |
| `++` (post) | `T operator++(int)` | `T` | Метод, через pre |
| `==` | `bool operator==(const T&) const` | `bool` | Метод или friend |
| `<=>` | `auto operator<=>(const T&) const` | `ordering` | C++20 |
| `<<` | `friend ostream& operator<<(ostream&, const T&)` | `ostream&` | Свободная |
| `>>` | `friend istream& operator>>(istream&, T&)` | `istream&` | Свободная |
| `[]` | `T& operator[](size_t)` + const-версия | `T&`/`const T&` | Метод |
| `()` | `R operator()(Args...) const` | `R` | Метод |
| `->` | `T* operator->()` | `T*` | Метод |
| `=` (copy) | `T& operator=(T)` или `T& operator=(const T&)` | `T&` | Метод |
| `bool` | `explicit operator bool() const` | `bool` | Метод |

Примеры:
- [10_literals_canonical](10_literals_canonical)

---

## Ключевые концепции

| Концепция | Суть |
|-----------|------|
| Метод vs свободная функция | Метод — левый операнд фиксирован; свободная — допускает конверсию |
| `+=` -> `+` | Бинарный оператор реализуется через составной |
| Pre/Post increment | Пре — ссылка, пост — копия + int-заглушка |
| `operator<=>` | Один оператор заменяет шесть (C++20) |
| Функторы | Объекты с `operator()`, совместимы с алгоритмами STL |
| `explicit operator T()` | Запрет неявных опасных преобразований |
| Пользовательские литералы | Расширение синтаксиса для выразительности |

---

## Типичные ошибки

1. **`operator+` как метод** — `2 + obj` не скомпилируется, используйте свободную функцию.
2. **Забыть `const` у `operator==`** — не сможете сравнивать `const` объекты.
3. **Постфиксный `++` без `int`-параметра** — компилятор не отличит от префиксного.
4. **`operator[]` только non-const** — не работает с `const`-ссылками.
5. **Неявный `operator bool()`** — `obj + 1` скомпилируется через `bool`->`int`, используйте `explicit`.
6. **`operator<<` как метод** — `cout << obj` потребует `cout.operator<<(obj)`, что невозможно.
7. **Возврат `const T` из `operator+`** — мешает move-семантике, устарело.

---

## Вопросы для самопроверки

1. Какие операторы нельзя перегрузить? Какие можно только как методы?
2. Почему `operator+` лучше реализовать через `operator+=`?
3. Как компилятор различает префиксный и постфиксный `++`?
4. Что возвращает `operator<=>` и какие типы упорядочения бывают?
5. Почему `operator<<` не может быть методом класса?
6. Зачем нужны две версии `operator[]` (const и non-const)?
7. В чём преимущество функтора перед указателем на функцию?
8. Почему `operator bool()` должен быть `explicit`?

---

## Практическое задание

Реализуйте класс `BigInteger` для работы с целыми числами произвольной длины:

1. Внутреннее представление: `std::vector<int8_t>` (цифры) + `bool` (знак).
2. Конструкторы: из `int64_t`, из `std::string`, копирующий, перемещающий.
3. Арифметика: `+`, `-`, `*` (через `+=`, `-=`, `*=`).
4. Сравнение: `operator<=>` (C++20) или полный набор `==`, `!=`, `<`, `>`, `<=`, `>=`.
5. Инкремент/декремент: префиксный и постфиксный.
6. Ввод/вывод: `operator<<` и `operator>>`.
7. Индексирование: `operator[]` — доступ к i-й цифре.
8. `explicit operator bool()` — true, если не ноль.
9. Пользовательский литерал: `operator""_bi` для создания из строки.
10. Проверьте: `123456789_bi * 987654321_bi`, вывод через `cout`.
