# Лекция 10. Виртуальные функции и полиморфизм

## План лекции

1. Виды полиморфизма: ad-hoc, параметрический, подтипов
2. Виртуальные функции: синтаксис и семантика
3. Таблица виртуальных функций (vtable) -- устройство и диаграмма
4. Спецификаторы `override` и `final`
5. Чисто виртуальные функции, абстрактные классы, интерфейсы
6. Виртуальный деструктор (обязательно!)
7. `dynamic_cast`, RTTI и `typeid`
8. Виртуальные вызовы из конструктора/деструктора
9. Стоимость виртуальных вызовов и девиртуализация
10. Полиморфные коллекции (`vector<unique_ptr<Base>>`) и паттерны Factory Method, Strategy

---

## 1. Виды полиморфизма

**Ad-hoc полиморфизм** -- перегрузка функций и операторов:
```cpp
int    add(int a, int b)       { return a + b; }
double add(double a, double b) { return a + b; }
```

**Параметрический полиморфизм** -- шаблоны:
```cpp
template<typename T>
T add(T a, T b) { return a + b; }
```

**Полиморфизм подтипов (subtype)** -- через виртуальные функции. Это главная тема лекции:
```cpp
class Shape {
public:
    virtual double area() const = 0;
};
class Circle : public Shape {
    double r;
public:
    Circle(double r) : r(r) {}
    double area() const override { return 3.14159 * r * r; }
};
```

---

## 2. Виртуальные функции

Ключевое слово `virtual` включает механизм позднего связывания (late binding):

```cpp
class Animal {
public:
    virtual void speak() const {
        std::cout << "..." << std::endl;
    }
};

class Cat : public Animal {
public:
    void speak() const override {
        std::cout << "Мяу!" << std::endl;
    }
};

class Dog : public Animal {
public:
    void speak() const override {
        std::cout << "Гав!" << std::endl;
    }
};

void makeNoise(const Animal& animal) {
    animal.speak();  // Вызов определяется в runtime!
}

int main() {
    Cat c;
    Dog d;
    makeNoise(c);  // "Мяу!"
    makeNoise(d);  // "Гав!"
}
```

> Примеры: [09_PolymorphismWithVirtualFunctions](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection04_05/09_PolymorphismWithVirtualFunctions)

---

## 3. Таблица виртуальных функций (vtable)

Каждый класс с виртуальными функциями имеет vtable -- массив указателей на реализации виртуальных методов. Каждый объект содержит скрытый указатель `vptr` на vtable своего класса.

```
Объект Cat:             vtable Cat:
+----------+           +-------------------+
| vptr ----+---------->| &Cat::speak       |
| ...      |           | &Animal::eat      |
+----------+           +-------------------+

Объект Dog:             vtable Dog:
+----------+           +-------------------+
| vptr ----+---------->| &Dog::speak       |
| ...      |           | &Animal::eat      |
+----------+           +-------------------+
```

**Как происходит виртуальный вызов:**
```
animal.speak()
  -> animal.vptr           // получить указатель на vtable
  -> vptr[0]               // взять адрес speak() из таблицы
  -> вызвать по адресу     // косвенный вызов
```

Это добавляет один уровень косвенности по сравнению с обычным вызовом функции.

---

## 4. Спецификаторы `override` и `final`

`override` гарантирует, что функция действительно переопределяет виртуальную функцию базового класса:

```cpp
class Base {
public:
    virtual void process(int x) const {}
};

class Derived : public Base {
public:
    // void process(int x) {}          // НЕ override! (отсутствует const)
    // void process(double x) const {} // НЕ override! (другой тип параметра)
    void process(int x) const override {}  // OK -- компилятор проверит
};
```

`final` запрещает дальнейшее переопределение:

```cpp
class Middle : public Base {
public:
    void process(int x) const final {}  // дальше переопределять нельзя
};

class Bottom : public Middle {
public:
    // void process(int x) const override {} // Ошибка компиляции!
};
```

> Примеры: [11_Override](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection04_05/11_Override)

---

## 5. Чисто виртуальные функции, абстрактные классы, интерфейсы

**Чисто виртуальная функция** не имеет реализации (хотя может иметь!):

```cpp
class Shape {
public:
    virtual double area() const = 0;       // чисто виртуальная
    virtual double perimeter() const = 0;  // чисто виртуальная
    virtual void draw() const {
        std::cout << "Рисуем фигуру площадью " << area() << std::endl;
    }
    virtual ~Shape() = default;
};
// Shape s; // Ошибка: нельзя создать экземпляр абстрактного класса
```

**Абстрактный класс** -- класс с хотя бы одной чисто виртуальной функцией. Создавать объекты такого класса нельзя.

**Интерфейс** (в C++) -- абстрактный класс, содержащий только чисто виртуальные функции:

```cpp
class ISerializable {
public:
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& data) = 0;
    virtual ~ISerializable() = default;
};

class IDrawable {
public:
    virtual void draw() const = 0;
    virtual ~IDrawable() = default;
};

class Circle : public Shape, public ISerializable {
    double r;
public:
    Circle(double r) : r(r) {}
    double area() const override { return 3.14159 * r * r; }
    double perimeter() const override { return 2 * 3.14159 * r; }
    std::string serialize() const override { return "Circle:" + std::to_string(r); }
    void deserialize(const std::string& data) override { /* ... */ }
};
```

> Примеры: [18_PureVirtualFunctionsAndAbstractClasses](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection04_05/18_PureVirtualFunctionsAndAbstractClasses)

---

## 6. Виртуальный деструктор (обязательно!)

Если класс используется полиморфно, его деструктор **обязан** быть виртуальным:

```cpp
class Base {
public:
    ~Base() { std::cout << "~Base" << std::endl; }  // НЕ виртуальный!
};

class Derived : public Base {
    int* data;
public:
    Derived() : data(new int[1000]) {}
    ~Derived() { delete[] data; std::cout << "~Derived" << std::endl; }
};

int main() {
    Base* ptr = new Derived();
    delete ptr;  // УТЕЧКА! Вызывается только ~Base(), data не освобождён
}
```

**Исправление:**

```cpp
class Base {
public:
    virtual ~Base() { std::cout << "~Base" << std::endl; }
};
// Теперь delete ptr вызовет ~Derived(), затем ~Base()
```

**Правило:** Если в классе есть хотя бы одна виртуальная функция, деструктор должен быть виртуальным. Если класс не предназначен для наследования, рассмотрите `final`.

---

## 7. `dynamic_cast`, RTTI и `typeid`

`dynamic_cast` выполняет безопасное приведение указателей/ссылок по иерархии наследования:

```cpp
class Base {
public:
    virtual ~Base() = default;
};
class Derived1 : public Base {
public:
    void special1() { std::cout << "Derived1::special1" << std::endl; }
};
class Derived2 : public Base {
public:
    void special2() { std::cout << "Derived2::special2" << std::endl; }
};

void process(Base* b) {
    if (auto* d1 = dynamic_cast<Derived1*>(b)) {
        d1->special1();  // безопасно: b действительно Derived1
    } else if (auto* d2 = dynamic_cast<Derived2*>(b)) {
        d2->special2();
    } else {
        std::cout << "Неизвестный тип" << std::endl;
    }
    // Для ссылок: при неудаче бросает std::bad_cast
    // Derived1& ref = dynamic_cast<Derived1&>(*b);
}
```

`typeid` возвращает информацию о типе во время выполнения:

```cpp
#include <typeinfo>

void identify(const Base& b) {
    std::cout << typeid(b).name() << std::endl;
    if (typeid(b) == typeid(Derived1)) {
        std::cout << "Это Derived1" << std::endl;
    }
}
```

> Примеры: [16_DynamicCasts](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection04_05/16_DynamicCasts), [17_TypeIdOperator](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection04_05/17_TypeIdOperator)

**Внимание:** частое использование `dynamic_cast` -- признак плохого дизайна. Предпочитайте виртуальные функции.

---

## 8. Виртуальные вызовы из конструктора и деструктора

Во время конструирования/разрушения объекта виртуальные вызовы **не** работают полиморфно:

```cpp
class Base {
public:
    Base() {
        init();  // Всегда вызовет Base::init(), даже для Derived!
    }
    virtual void init() {
        std::cout << "Base::init" << std::endl;
    }
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    void init() override {
        std::cout << "Derived::init" << std::endl;
    }
};

int main() {
    Derived d;  // Выведет "Base::init", а не "Derived::init"!
}
```

**Причина:** во время выполнения конструктора `Base`, объект ещё "не стал" `Derived`. Его `vptr` указывает на vtable `Base`. Это защитный механизм -- производная часть объекта ещё не инициализирована.

---

## 9. Стоимость виртуальных вызовов и девиртуализация

Виртуальный вызов дороже обычного из-за:
- Дополнительной косвенности (чтение vptr, чтение vtable, вызов)
- Невозможности инлайнинга (компилятор не знает, какая функция будет вызвана)
- Cache miss при обращении к vtable

Однако современные компиляторы выполняют **девиртуализацию** -- если компилятор может доказать конкретный тип объекта:

```cpp
void example() {
    Derived d;
    d.speak();        // Компилятор знает тип -- прямой вызов (девиртуализация)

    Derived* p = new Derived();
    p->speak();       // Тоже может быть девиртуализирован

    Base& ref = d;
    ref.speak();      // Компилятор может девиртуализировать (зависит от контекста)
}
```

`final` помогает компилятору девиртуализировать вызовы, повышая производительность.

> Примеры: [12_InheritanceAndPolymorphismWithStaticMembers](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection04_05/12_InheritanceAndPolymorphismWithStaticMembers)

---

## 10. Полиморфные коллекции и паттерны

### Полиморфные коллекции

Для хранения полиморфных объектов используйте `std::unique_ptr`:

```cpp
#include <memory>
#include <vector>

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Rectangle>(3.0, 4.0));
    shapes.push_back(std::make_unique<Triangle>(3.0, 4.0, 5.0));

    double totalArea = 0;
    for (const auto& shape : shapes) {
        shape->draw();
        totalArea += shape->area();
        std::cout << "Площадь: " << shape->area() << std::endl;
    }
    std::cout << "Общая площадь: " << totalArea << std::endl;
}
```

> Примеры: [10_PolymorphicObjectsStoredInCollections](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection04_05/10_PolymorphicObjectsStoredInCollections)

### Паттерн Factory Method

Фабричный метод инкапсулирует создание объектов:

```cpp
class Document {
public:
    virtual void open() = 0;
    virtual void save() = 0;
    virtual ~Document() = default;
};

class PDFDocument : public Document {
public:
    void open() override { std::cout << "Открываем PDF" << std::endl; }
    void save() override { std::cout << "Сохраняем PDF" << std::endl; }
};

class WordDocument : public Document {
public:
    void open() override { std::cout << "Открываем Word" << std::endl; }
    void save() override { std::cout << "Сохраняем Word" << std::endl; }
};

// Фабричный метод
std::unique_ptr<Document> createDocument(const std::string& type) {
    if (type == "pdf")  return std::make_unique<PDFDocument>();
    if (type == "word") return std::make_unique<WordDocument>();
    throw std::invalid_argument("Неизвестный тип документа: " + type);
}

int main() {
    auto doc = createDocument("pdf");
    doc->open();
    doc->save();
}
```

> Примеры: [20_FactoryMethod](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection10_11/20_FactoryMethod)

### Паттерн Strategy

Стратегия позволяет выбирать алгоритм во время выполнения:

```cpp
class SortStrategy {
public:
    virtual void sort(std::vector<int>& data) = 0;
    virtual ~SortStrategy() = default;
};

class BubbleSort : public SortStrategy {
public:
    void sort(std::vector<int>& data) override {
        for (size_t i = 0; i < data.size(); ++i)
            for (size_t j = 0; j + 1 < data.size() - i; ++j)
                if (data[j] > data[j+1])
                    std::swap(data[j], data[j+1]);
        std::cout << "Отсортировано пузырьком" << std::endl;
    }
};

class QuickSort : public SortStrategy {
public:
    void sort(std::vector<int>& data) override {
        std::sort(data.begin(), data.end());
        std::cout << "Отсортировано быстрой сортировкой" << std::endl;
    }
};

class Sorter {
    std::unique_ptr<SortStrategy> strategy;
public:
    void setStrategy(std::unique_ptr<SortStrategy> s) {
        strategy = std::move(s);
    }
    void performSort(std::vector<int>& data) {
        if (strategy) strategy->sort(data);
    }
};
```

---

## Типичные ошибки

1. **Забыли виртуальный деструктор** -- утечка памяти при `delete` через указатель на базовый класс
2. **Вызов виртуальной функции из конструктора** -- вызовется версия базового класса
3. **Забыли `override`** -- создали новую функцию вместо переопределения (опечатка в сигнатуре)
4. **Срезка в контейнерах** -- `vector<Base>` вместо `vector<unique_ptr<Base>>`
5. **Злоупотребление `dynamic_cast`** -- признак плохого полиморфного дизайна

---

## Вопросы для самопроверки

1. Чем отличаются три вида полиморфизма? Приведите пример каждого.
2. Нарисуйте vtable для иерархии из трёх классов. Что происходит при виртуальном вызове?
3. Зачем нужен `override`? Какую ошибку он предотвращает?
4. Почему деструктор полиморфного класса должен быть виртуальным?
5. Что произойдёт при вызове виртуальной функции из конструктора? Почему?
6. Чем `dynamic_cast` для указателей отличается от `dynamic_cast` для ссылок?
7. Как `final` помогает оптимизации?
8. Как правильно хранить полиморфные объекты в контейнере?

---

## Практическое задание

Реализуйте систему рисования фигур:

1. Абстрактный класс `Shape` с методами `area()`, `perimeter()`, `draw()`, `clone()`
2. Классы `Circle`, `Rectangle`, `Triangle`, наследующие `Shape`
3. Фабричный метод `createShape(const std::string& type, ...params...)`
4. Класс `Canvas`, хранящий `vector<unique_ptr<Shape>>`, с методами:
   - `addShape(unique_ptr<Shape>)` -- добавление фигуры
   - `totalArea()` -- суммарная площадь
   - `drawAll()` -- отрисовка всех фигур
5. Продемонстрируйте полиморфное поведение в `main()`
