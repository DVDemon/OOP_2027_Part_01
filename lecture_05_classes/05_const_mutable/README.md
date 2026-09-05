# 05. `const`-методы и `mutable`

**Пункт плана:** 5 — `const`-методы и `mutable`.

## Что показывает пример

* `const`-метод (`determinant`) обещает не менять наблюдаемое состояние;
  внутри него `this` имеет тип `const Matrix2x2*`.
* Через `const`-ссылку доступны только `const`-методы (см. `printDet`).
* `mutable`-поля (`cached_det_`, `det_valid_`, `compute_count_`) можно менять
  даже из `const`-метода — это узаконенное исключение для кэшей и счётчиков.
* Изменение данных (`setA`) инвалидирует кэш — определитель пересчитается.

## Сборка и запуск

```bash
cmake -S .. -B ../build       # из папки лекции
cmake --build ../build
./../build/05_const_mutable
```

## Плохо → Хорошо

**Плохо** — отказаться от `const` у метода-чтения, чтобы кэшировать результат:

```cpp
double determinant() {        // НЕ const, чтобы можно было писать в поля-кэш
    if (!det_valid_) { cached_det_ = a_*d_ - b_*c_; det_valid_ = true; }
    return cached_det_;
}
// тогда: const Matrix2x2& m; m.determinant();  // ОШИБКА — метод не const
```

**Почему плохо:** метод логически только читает, но из-за кэша становится
не-`const`. Его нельзя вызвать через `const`-ссылку — это ломает const-корректность
всего кода, который работает с объектом только на чтение.

**Хорошо** — оставить метод `const`, а поля кэша пометить `mutable`:

```cpp
mutable double cached_det_ = 0.0;
mutable bool det_valid_ = false;

double determinant() const {                 // остаётся const
    if (!det_valid_) { cached_det_ = a_*d_ - b_*c_; det_valid_ = true; }
    return cached_det_;
}
```

**Вывод:** помечайте `const` все методы, не меняющие наблюдаемое состояние.
Для технических полей (кэш, счётчик обращений, мьютекс) используйте `mutable` —
но не злоупотребляйте: `mutable` не должно менять видимое снаружи поведение.
