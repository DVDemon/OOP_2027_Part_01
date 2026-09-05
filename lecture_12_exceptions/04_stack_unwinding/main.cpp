// Пример 04: раскрутка стека (stack unwinding).
//
// При выбросе исключения все локальные объекты разрушаются в обратном
// порядке создания, пока не найдётся подходящий catch.
#include <iostream>
#include <stdexcept>
#include <string>

class Logger {
    std::string name_;
public:
    explicit Logger(std::string name) : name_(std::move(name)) {
        std::cout << "  Создан:   " << name_ << '\n';
    }
    ~Logger() {
        std::cout << "  Разрушен: " << name_ << '\n';
    }
};

void func3() {
    Logger l3("L3");
    throw std::runtime_error("Ошибка в func3");
    // l3 будет разрушен при раскрутке стека, хотя сюда мы не дойдём.
}

void func2() {
    Logger l2("L2");
    func3();  // исключение проходит сквозь func2, разрушая l2
}

void func1() {
    Logger l1("L1");
    func2();  // исключение проходит сквозь func1, разрушая l1
}

int main() {
    try {
        func1();
    }
    catch (const std::exception& e) {
        std::cout << "Поймано: " << e.what() << '\n';
    }
    // Ожидаемый вывод:
    //   Создан:   L1
    //   Создан:   L2
    //   Создан:   L3
    //   Разрушен: L3   <- раскрутка стека
    //   Разрушен: L2   <- раскрутка стека
    //   Разрушен: L1   <- раскрутка стека
    //   Поймано: Ошибка в func3
    return 0;
}
