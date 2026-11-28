# 09. DIP — принцип инверсии зависимостей

**Пункт плана:** 9 — DIP.

> «Модули верхнего уровня не должны зависеть от модулей нижнего уровня. Оба
> должны зависеть от абстракций».

## Что показывает пример

`UserService` (верхний уровень) зависит от абстракции `IUserRepository`, а не
от конкретной БД. Реализации (`MySqlUserRepository`, `MongoUserRepository`)
подставляются в Composition Root. Файлы:

* `user_service.h` — абстракция `IUserRepository` и бизнес-логика `UserService`;
* `main.cpp` — конкретные реализации и сборка зависимостей в `main`.

## Сборка и запуск

```bash
cmake -S .. -B ../build       # из папки лекции
cmake --build ../build
./../build/09_dip
```

Цели: `09_dip` — демонстрация в консоли.

## Плохо → Хорошо

**Плохо** — жёсткая зависимость от конкретной БД:

```cpp
class UserService {
    MySqlDatabase db_;   // зависимость от реализации
public:
    void create_user(const std::string& name) { db_.save(name); }
};
```

**Почему плохо:** `UserService` нельзя использовать с другой БД и невозможно
протестировать без реальной MySQL. Конструкция `new ConcreteClass` внутри
бизнес-логики намертво привязывает её к инфраструктуре.

**Хорошо** — зависимость от абстракции + инъекция:

```cpp
class IUserRepository { public: virtual void save(const std::string&) = 0; /*...*/ };

class UserService {
    std::unique_ptr<IUserRepository> repo_;
public:
    explicit UserService(std::unique_ptr<IUserRepository> repo)
        : repo_(std::move(repo)) {}
    void create_user(const std::string& name) { repo_->save(name); }
};

// Composition Root (main):
UserService service(std::make_unique<MySqlUserRepository>());
```

**Почему хорошо:** абстракция `IUserRepository` принадлежит слою бизнес-логики;
инфраструктура зависит от неё. Направление зависимости инвертировано, реализацию
легко подменить (в т.ч. фейком для теста).

> **DIP vs DI:** DIP — это направление зависимости (на абстракцию, владеемую
> верхним слоем). Dependency Injection — лишь техника передачи зависимости
> (через конструктор). DI без абстракции не даёт инверсии.
