// Пример 06: RAII и исключения.
//
// RAII (Resource Acquisition Is Initialization): ресурс захватывается в
// конструкторе и освобождается в деструкторе. При раскрутке стека деструктор
// вызывается автоматически — утечки невозможны.
#include <iostream>
#include <memory>
#include <stdexcept>

void risky_operation() {
    throw std::runtime_error("Сбой во время операции");
}

// ХОРОШО: RAII через unique_ptr — освобождение гарантировано.
void good_raii() {
    auto buffer = std::make_unique<int[]>(100);  // владелец памяти
    buffer[0] = 42;
    risky_operation();  // если бросит — buffer освободится при раскрутке стека
    // delete не нужен — управляет unique_ptr
}

// Свой RAII-класс: демонстрируем, что деструктор срабатывает при исключении.
class FileHandle {
    std::string name_;
public:
    explicit FileHandle(std::string name) : name_(std::move(name)) {
        std::cout << "  Открыт файл:  " << name_ << '\n';
    }
    ~FileHandle() {
        std::cout << "  Закрыт файл:  " << name_ << '\n';  // вызовется всегда
    }
};

void process() {
    FileHandle file("data.txt");  // RAII: закроется в любом случае
    risky_operation();            // бросает — но файл будет закрыт
}

int main() {
    try {
        good_raii();
    } catch (const std::exception& e) {
        std::cout << "good_raii: память освобождена, поймано: " << e.what() << '\n';
    }

    try {
        process();
    } catch (const std::exception& e) {
        std::cout << "process: файл закрыт деструктором, поймано: " << e.what() << '\n';
    }

    return 0;
}
