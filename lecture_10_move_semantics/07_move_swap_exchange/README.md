# 07. std::move, std::swap, std::exchange

**Пункт плана:** 7 — `std::move`, `std::swap` через move, `std::exchange`.

## Что показывает пример

* `std::move` приводит lvalue к rvalue, разрешая перемещение.
* `std::swap` обменивает объекты через три move-операции.
* `std::exchange` присваивает новое значение и возвращает старое.
* Применение `std::exchange` для обнуления источника одним выражением.

## Запуск

```bash
./build/07_move_swap_exchange
```

## Что есть что

```cpp
// std::move — это просто приведение типа, НЕ перемещение:
template <class T>
constexpr std::remove_reference_t<T>&& move(T&& x) noexcept {
    return static_cast<std::remove_reference_t<T>&&>(x);
}

// std::swap через move — 3 перемещения вместо 3 копирований:
template <class T>
void swap(T& a, T& b) noexcept(/* ... */) {
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}

// std::exchange — заменить и вернуть старое значение:
template <class T, class U = T>
T exchange(T& obj, U&& new_value) {
    T old = std::move(obj);
    obj = std::forward<U>(new_value);
    return old;
}
```

## Плохо → Хорошо

**Плохо** — обнулять источник в перемещающем конструкторе двумя шагами на
каждое поле, легко забыть один из них:

```cpp
String(String&& o) noexcept : data_(o.data_), size_(o.size_) {
    o.data_ = nullptr;   // забыли -> double free
    o.size_ = 0;
}
```

**Почему плохо:** «забрать значение» и «обнулить источник» разнесены, есть
риск пропустить обнуление какого-нибудь поля.

**Хорошо** — `std::exchange` делает оба действия в одном выражении прямо в
списке инициализации:

```cpp
String(String&& o) noexcept
    : data_(std::exchange(o.data_, nullptr))   // забрать data_ и обнулить
    , size_(std::exchange(o.size_, 0)) {}      // забрать size_ и обнулить
```

> Помните: `std::move` ничего не перемещает сам по себе — он лишь разрешает
> компилятору выбрать перемещающую перегрузку. Если перемещающего
> конструктора нет, всё равно будет копирование.
