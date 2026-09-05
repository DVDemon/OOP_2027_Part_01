# 09. ServerApplication: жизненный цикл приложения

**Пункт плана:** 9 — `ServerApplication`: жизненный цикл приложения.

> **Требует POCO** (Foundation, Net, Util). Цель `09_server_application`
> собирается только при `Poco_FOUND`. Без POCO раздел читается как обзор.

## Что показывает пример

Фазы жизненного цикла `Poco::Util::ServerApplication` **без** HTTP-сервера —
только последовательность вызовов:

```
            ┌───────────────────┐
            │   Конструктор     │
            └────────┬──────────┘
                     │
            ┌────────▼──────────┐
            │   initialize()    │  ← конфигурация, логирование
            └────────┬──────────┘
                     │
            ┌────────▼──────────┐
            │      main()       │  ← основная логика
            │ waitForTermination│  ← блокируется до Ctrl+C / SIGTERM
            │     Request()     │
            └────────┬──────────┘
                     │
            ┌────────▼──────────┐
            │  uninitialize()   │  ← освобождение ресурсов
            └───────────────────┘
```

`POCO_SERVER_MAIN(LifecycleApp)` разворачивается в `main()`, создаёт приложение и
вызывает `run(argc, argv)`, который проходит фазы по порядку. Это **RAII на уровне
приложения**: ресурсы захватываются в `initialize`, освобождаются в `uninitialize`.

## Сборка и запуск

```bash
# нужен установленный POCO: brew install poco
cmake -S . -B build && cmake --build build

./build/09_server_application --help     # печатает справку (опции через Util)
./build/09_server_application            # работает; Ctrl+C завершает
```

Ожидаемый вывод при обычном запуске:

```
[initialize] приложение инициализировано
[main] приложение запущено; ждём Ctrl+C (SIGINT/SIGTERM)
^C
[main] получен сигнал завершения, выходим
[uninitialize] ресурсы освобождены
```

## Плохо → Хорошо

**Плохо** — переопределить `initialize`/`uninitialize` и забыть вызвать базовую
реализацию: конфигурация и логирование останутся ненастроенными.

```cpp
void initialize(Application& self) override {
    loadConfiguration();   // забыли ServerApplication::initialize(self);
}
```

**Хорошо** — всегда вызывать базовую реализацию:

```cpp
void initialize(Application& self) override {
    loadConfiguration();
    ServerApplication::initialize(self);
}
```
