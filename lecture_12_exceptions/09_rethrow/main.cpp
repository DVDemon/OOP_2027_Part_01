// Пример 09: повторный выброс — throw; vs throw e; (срезка!).
//
// throw;   — пробрасывает текущее исключение БЕЗ изменения (правильно).
// throw e; — бросает КОПИЮ по статическому типу e — производный класс срезается.
#include <iostream>

class BaseException {
public:
    virtual ~BaseException() = default;
    virtual const char* name() const { return "BaseException"; }
};

class DerivedException : public BaseException {
public:
    const char* name() const override { return "DerivedException"; }
};

// ПРАВИЛЬНО: throw; сохраняет динамический тип DerivedException.
void rethrow_correct() {
    try {
        throw DerivedException();
    } catch (const BaseException& e) {
        std::cout << "  ловим как " << e.name() << ", делаем throw;\n";
        throw;  // пробрасываем оригинал — это всё ещё DerivedException
    }
}

// ОШИБКА: throw e; копирует e как BaseException — срезка.
void rethrow_wrong() {
    try {
        throw DerivedException();
    } catch (const BaseException& e) {
        std::cout << "  ловим как " << e.name() << ", делаем throw e;\n";
        throw e;  // СРЕЗКА: бросается копия BaseException
    }
}

int main() {
    std::cout << "rethrow_correct():\n";
    try {
        rethrow_correct();
    } catch (const DerivedException& e) {
        std::cout << "  поймали как " << e.name() << " (тип сохранён)\n";
    } catch (const BaseException& e) {
        std::cout << "  поймали как " << e.name() << " (НЕ ожидалось)\n";
    }

    std::cout << "rethrow_wrong():\n";
    try {
        rethrow_wrong();
    } catch (const DerivedException& e) {
        std::cout << "  поймали как " << e.name() << " (НЕ попадём сюда)\n";
    } catch (const BaseException& e) {
        std::cout << "  поймали как " << e.name() << " (срезка до базового типа)\n";
    }

    return 0;
}
