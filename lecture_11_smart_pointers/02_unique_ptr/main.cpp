// Пример 02: std::unique_ptr — эксклюзивное владение.
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

int main() {
    // --- Создание: make_unique предпочтительнее ---
    auto p1 = std::make_unique<int>(42);            // рекомендуемый способ
    std::unique_ptr<int> p2(new int(7));            // допустимо, но менее безопасно
    std::cout << "*p1 = " << *p1 << ", *p2 = " << *p2 << '\n';

    // --- Основные операции ---
    int* raw = p1.get();          // сырой указатель, владения НЕ передаёт
    std::cout << "raw value = " << *raw << '\n';

    p2.reset(new int(100));       // освободить старый ресурс, владеть новым
    std::cout << "*p2 после reset = " << *p2 << '\n';

    p2.reset();                   // освободить ресурс, p2 == nullptr
    std::cout << "p2 пуст? " << (p2 ? "нет" : "да") << '\n';

    // --- Копирование запрещено, перемещение разрешено ---
    auto a = std::make_unique<std::string>("ресурс");
    // auto bad = a;              // ОШИБКА: unique_ptr нельзя копировать
    auto b = std::move(a);        // OK: владение переходит к b, a == nullptr
    std::cout << "*b = " << *b << ", a пуст? " << (a ? "нет" : "да") << '\n';

    // --- Массив ---
    auto arr = std::make_unique<int[]>(3);
    arr[0] = 1; arr[1] = 2; arr[2] = 3;
    std::cout << "arr: " << arr[0] << ' ' << arr[1] << ' ' << arr[2] << '\n';

    // --- Пользовательский deleter: владение ресурсом C-библиотеки (FILE*) ---
    auto fileDeleter = [](std::FILE* f) {
        if (f) {
            std::cout << "deleter: закрываем файл\n";
            std::fclose(f);
        }
    };
    {
        std::unique_ptr<std::FILE, decltype(fileDeleter)>
            file(std::fopen("unique_ptr_demo.tmp", "w"), fileDeleter);
        if (file) {
            std::fputs("demo", file.get());
            std::cout << "файл открыт и записан\n";
        }
    }  // здесь сработает deleter — файл закроется автоматически
    std::remove("unique_ptr_demo.tmp");

    return 0;
}
