# Лекция 9. Наследование

## План лекции

1. Концепция наследования и синтаксис
2. Модификаторы наследования: public, protected, private (таблица доступа)
3. Порядок конструирования и разрушения объектов
4. Наследование конструкторов (`using Base::Base`)
5. Сокрытие имён (name hiding) и директива `using`
6. Срезка объектов (object slicing)
7. Приватное наследование vs композиция
8. Спецификатор `final` для классов
9. Принцип "предпочитайте композицию наследованию"
10. Практические рекомендации

---

## 1. Концепция наследования

Наследование -- один из трёх столпов ООП. Оно позволяет создавать новый класс на основе существующего, перенимая его данные и поведение.

```cpp
class Animal {
protected:
    std::string name;
    int age;
public:
    Animal(const std::string& n, int a) : name(n), age(a) {}
    void info() const {
        std::cout << name << ", возраст: " << age << std::endl;
    }
};

class Dog : public Animal {
    std::string breed;
public:
    Dog(const std::string& n, int a, const std::string& b)
        : Animal(n, a), breed(b) {}
    void bark() const { std::cout << name << " говорит: Гав!" << std::endl; }
};
```

> Пример: [01_inheritance_basics](01_inheritance_basics)

---

## 2. Модификаторы наследования: таблица доступа

При наследовании уровень доступа к членам базового класса может измениться в зависимости от модификатора наследования.

### Таблица доступа

| Член в базовом классе | `public`-наследование | `protected`-наследование | `private`-наследование |
|---|---|---|---|
| `public`    | `public`    | `protected` | `private` |
| `protected` | `protected` | `protected` | `private` |
| `private`   | недоступен  | недоступен  | недоступен |

```cpp
class Base {
public:
    int pub;
protected:
    int prot;
private:
    int priv;  // никогда не доступен в наследнике
};

class PubDerived : public Base {
    // pub  -> public
    // prot -> protected
    // priv -> недоступен
};

class ProtDerived : protected Base {
    // pub  -> protected
    // prot -> protected
    // priv -> недоступен
};

class PrivDerived : private Base {
    // pub  -> private
    // prot -> private
    // priv -> недоступен
};
```

> Пример: [02_access_modifiers](02_access_modifiers)

**Ключевой момент:** `private`-члены базового класса *существуют* в объекте наследника, но к ним нельзя обратиться напрямую -- только через методы базового класса.

---

## 3. Порядок конструирования и разрушения

Порядок строго определён стандартом:

**Конструирование (сверху вниз):**
1. Конструктор базового класса
2. Инициализация членов производного класса (в порядке объявления)
3. Тело конструктора производного класса

**Разрушение (снизу вверх) -- обратный порядок:**
1. Тело деструктора производного класса
2. Деструкторы членов производного класса
3. Деструктор базового класса

```cpp
class Base {
public:
    Base()  { std::cout << "Base()" << std::endl; }
    ~Base() { std::cout << "~Base()" << std::endl; }
};

class Derived : public Base {
    std::string tag{"hello"};
public:
    Derived()  { std::cout << "Derived()" << std::endl; }
    ~Derived() { std::cout << "~Derived()" << std::endl; }
};

// Вывод при создании Derived d;
// Base()
// Derived()
// При разрушении:
// ~Derived()
// ~Base()
```

> Пример: [03_construction_order](03_construction_order)

---

## 4. Наследование конструкторов (`using Base::Base`)

C++11 позволяет "унаследовать" конструкторы базового класса:

```cpp
class Base {
public:
    Base(int x) { std::cout << "Base(int): " << x << std::endl; }
    Base(int x, double y) { std::cout << "Base(int,double)" << std::endl; }
};

class Derived : public Base {
public:
    using Base::Base;  // наследуем все конструкторы Base

    // Можно добавить свои конструкторы
    Derived() : Base(0) { std::cout << "Derived()" << std::endl; }
};

int main() {
    Derived d1(42);       // вызывает Base(int) через using
    Derived d2(1, 3.14);  // вызывает Base(int, double) через using
    Derived d3;           // вызывает Derived()
}
```

**Ограничения:**
- Унаследованные конструкторы не инициализируют члены производного класса (используйте default member initializers)
- Конструктор копирования и перемещения не наследуются через `using`

> Пример: [04_inheriting_constructors](04_inheriting_constructors)

---

## 5. Сокрытие имён (name hiding) и `using`

Если в производном классе объявлена функция с тем же именем, что и в базовом, **все** перегрузки базового класса скрываются:

```cpp
class Base {
public:
    void foo(int x)    { std::cout << "Base::foo(int)" << std::endl; }
    void foo(double x) { std::cout << "Base::foo(double)" << std::endl; }
};

class Derived : public Base {
public:
    void foo(int x) { std::cout << "Derived::foo(int)" << std::endl; }
    // Base::foo(double) теперь скрыта!
};

int main() {
    Derived d;
    d.foo(42);    // OK: Derived::foo(int)
    d.foo(3.14);  // Вызовет Derived::foo(int) -- неявное преобразование!
    // d.Base::foo(3.14); // Можно вызвать явно
}
```

Чтобы вернуть перегрузки базового класса в область видимости:

```cpp
class Derived : public Base {
public:
    using Base::foo;  // "воскрешаем" все foo из Base
    void foo(int x) { std::cout << "Derived::foo(int)" << std::endl; }
};

int main() {
    Derived d;
    d.foo(42);    // Derived::foo(int)
    d.foo(3.14);  // Base::foo(double) -- теперь доступна!
}
```

> Пример: [05_name_hiding](05_name_hiding)

---

## 6. Срезка объектов (object slicing)

При копировании объекта производного класса в переменную базового типа "срезается" производная часть:

```cpp
class Base {
public:
    int x = 10;
    virtual void print() const { std::cout << "Base: " << x << std::endl; }
};

class Derived : public Base {
public:
    int y = 20;
    void print() const override { std::cout << "Derived: " << x << ", " << y << std::endl; }
};

int main() {
    Derived d;
    d.print();  // "Derived: 10, 20"

    Base b = d;  // СРЕЗКА! Копируется только Base-часть
    b.print();   // "Base: 10" -- полиморфизм потерян, y потерян

    // Правильно -- через указатель или ссылку:
    Base& ref = d;
    ref.print();  // "Derived: 10, 20" -- полиморфизм работает

    Base* ptr = &d;
    ptr->print(); // "Derived: 10, 20"
}
```

**Правило:** передавайте полиморфные объекты по указателю или ссылке, никогда по значению.

---

## 7. Приватное наследование vs композиция

Приватное наследование реализует отношение "реализовано посредством" (implemented-in-terms-of):

```cpp
// Приватное наследование
class Stack : private std::vector<int> {
public:
    void push(int val) { push_back(val); }
    void pop()         { std::vector<int>::pop_back(); }
    int top() const    { return back(); }
    bool empty() const { return std::vector<int>::empty(); }
    // Остальные методы vector недоступны пользователю Stack
};

// Эквивалент через композицию (предпочтительный вариант)
class Stack2 {
    std::vector<int> data;  // композиция
public:
    void push(int val) { data.push_back(val); }
    void pop()         { data.pop_back(); }
    int top() const    { return data.back(); }
    bool empty() const { return data.empty(); }
};
```

**Когда приватное наследование оправдано:**
- Нужен доступ к `protected`-членам базового класса
- Нужно переопределить виртуальные функции
- Оптимизация пустого базового класса (EBO)

Во всех остальных случаях -- предпочитайте композицию.

---

## 8. Спецификатор `final`

`final` запрещает наследование от класса или переопределение метода:

```cpp
class Framework final {
    // От этого класса нельзя наследоваться
public:
    void run() { std::cout << "Framework::run" << std::endl; }
};

// class MyFramework : public Framework {}; // Ошибка компиляции!

class Base {
public:
    virtual void process() final {
        // Этот метод нельзя переопределить
        std::cout << "Base::process" << std::endl;
    }
    virtual void handle() { std::cout << "Base::handle" << std::endl; }
};

class Derived : public Base {
public:
    // void process() override {} // Ошибка: process() is final
    void handle() override { std::cout << "Derived::handle" << std::endl; }
};
```

> Пример: [08_final_specifier](08_final_specifier)

---

## 9. Принцип "предпочитайте композицию наследованию"

Наследование создаёт сильнейшую связь между классами. Используйте его только когда оно действительно отражает отношение "является" (is-a):

```cpp
// Плохо: круг НЕ "является" эллипсом в плане подстановки
class Ellipse {
public:
    virtual void setRadii(double rx, double ry) { /* ... */ }
};
class Circle : public Ellipse {
    // Нарушение LSP: setRadii(3, 5) делает круг не-кругом
};

// Лучше: композиция
class Car {
    Engine engine;           // Машина СОДЕРЖИТ двигатель
    std::vector<Wheel> wheels;
public:
    void start() { engine.ignite(); }
};
```

**Критерии выбора наследования:**
1. Отношение is-a проходит тест Лисков (LSP)
2. Нужен полиморфизм через виртуальные функции
3. Базовый класс спроектирован для наследования

---

## 10. Практические рекомендации

- Используйте `public`-наследование для отношения is-a
- Избегайте глубоких иерархий наследования (больше 3 уровней -- тревожный знак)
- Всегда инициализируйте базовый класс в списке инициализации
- Помните о порядке конструирования/разрушения
- Применяйте `override` при переопределении виртуальных функций
- Не забывайте о срезке при передаче по значению

---

## Типичные ошибки

1. **Забыли вызвать конструктор базового класса** -- будет вызван конструктор по умолчанию (если есть)
2. **Срезка в контейнерах:** `std::vector<Base>` вместо `std::vector<std::unique_ptr<Base>>`
3. **Name hiding:** добавили метод с тем же именем и потеряли перегрузки базового класса
4. **Публичное наследование при отношении has-a** -- нарушение семантики

---

## Вопросы для самопроверки

1. Чем отличается `public`-, `protected`- и `private`-наследование? Составьте таблицу доступа.
2. В каком порядке вызываются конструкторы и деструкторы при наследовании?
3. Что такое name hiding и как с ним бороться?
4. Что произойдёт при присваивании `Base b = derived;`? Как этого избежать?
5. Когда приватное наследование предпочтительнее композиции?
6. Зачем нужен спецификатор `final`?
7. Что наследуется, а что нет при `using Base::Base`?
8. Почему глубокие иерархии наследования -- это проблема?

---

## Практическое задание

Реализуйте иерархию геометрических фигур:

1. Базовый класс `Shape` с полями `x`, `y` (координаты центра) и методами `area()`, `perimeter()`, `info()`
2. Класс `Rectangle : public Shape` с полями `width`, `height`
3. Класс `Square : public Rectangle` -- обоснуйте, корректно ли это с точки зрения LSP
4. Продемонстрируйте:
   - Порядок конструирования/разрушения (вывод в консоль)
   - Срезку объекта и способ её избежать
   - Использование `using Base::Base` для наследования конструкторов
   - Применение `final` к одному из классов
