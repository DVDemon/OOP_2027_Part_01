# 02. Виртуальные функции

**Пункт плана:** 2 — виртуальные функции: синтаксис и семантика.

## Что показывает пример

* Ключевое слово `virtual` включает **позднее связывание** (late binding):
  реализация выбирается в runtime по фактическому типу объекта.
* Полиморфизм работает только через **ссылку или указатель** на базовый класс.
* Невиртуальный метод, вызывающий виртуальный, тоже диспетчеризуется
  на версию потомка.

## Запуск

```bash
./build/02_virtual_functions
```

## Плохо → Хорошо

**Плохо** — без `virtual` срабатывает раннее связывание по статическому типу:

```cpp
class Animal { public: void speak() const { std::cout << "..."; } };  // НЕ virtual
class Cat : public Animal { public: void speak() const { std::cout << "Мяу"; } };

void makeNoise(const Animal& a) { a.speak(); }  // ВСЕГДА Animal::speak — "..."
```

**Хорошо** — `virtual` + `override` дают полиморфное поведение:

```cpp
class Animal { public: virtual void speak() const { std::cout << "..."; } };
class Cat : public Animal { public: void speak() const override { std::cout << "Мяу"; } };

void makeNoise(const Animal& a) { a.speak(); }  // для Cat выведет "Мяу"
```

**Почему:** без `virtual` компилятор подставляет вызов по типу переменной
(`Animal`), а не по реальному объекту. Передача по значению дополнительно
вызвала бы срезку (slicing). Полиморфизм требует `virtual` и ссылку/указатель.
