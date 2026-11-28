# 08. ISP — принцип разделения интерфейсов

**Пункт плана:** 8 — ISP.

> «Клиенты не должны зависеть от интерфейсов, которые они не используют».

## Что показывает пример

«Толстый» интерфейс `IMachine` разделён на узкие роли `IPrinter`, `IScanner`,
`IFax`. `SimplePrinter` реализует только `IPrinter`, а `MultiFunctionDevice` —
все три роли через множественное наследование интерфейсов.

## Сборка и запуск

```bash
cmake -S .. -B ../build       # из папки лекции
cmake --build ../build
./../build/08_isp
```

## Плохо → Хорошо

**Плохо** — один «толстый» интерфейс:

```cpp
class IMachine {
public:
    virtual void print(const Document&) = 0;
    virtual void scan(const Document&)  = 0;
    virtual void fax(const Document&)   = 0;
};

class SimplePrinter : public IMachine {
    void print(const Document&) override { /* OK */ }
    void scan(const Document&)  override { throw std::runtime_error("не умею"); }
    void fax(const Document&)   override { throw std::runtime_error("не умею"); }
};
```

**Почему плохо:** `SimplePrinter` вынужден реализовывать методы, которые ему не
нужны. Заглушки с исключениями — верный признак нарушения ISP: клиент зависит
от того, чем не пользуется, и контракт класса лжёт о его возможностях.

**Хорошо** — узкие интерфейсы-роли:

```cpp
class IPrinter { public: virtual void print(const Document&) = 0; /* ... */ };
class IScanner { public: virtual void scan(const Document&)  = 0; /* ... */ };
class IFax     { public: virtual void fax(const Document&)   = 0; /* ... */ };

class SimplePrinter : public IPrinter { /* только print */ };
class MultiFunctionDevice : public IPrinter, public IScanner, public IFax { /*...*/ };
```

**Почему хорошо:** каждый класс реализует только то, что умеет; клиент зависит
ровно от нужной роли. ISP — это, по сути, SRP применительно к интерфейсам.
