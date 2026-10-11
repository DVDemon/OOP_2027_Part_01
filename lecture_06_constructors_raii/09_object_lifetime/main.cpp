// Пример 08: время жизни объектов.
//   - автоматическое (стек);
//   - динамическое (куча, лучше через RAII);
//   - статическое (живёт до конца программы);
//   - временные объекты (живут до конца полного выражения).
#include <iostream>
#include <memory>
#include <string>

struct Loud {
    std::string name;
    explicit Loud(std::string n) : name(std::move(n)) {
        std::cout << "  + " << name << " создан\n";
    }
    ~Loud() { std::cout << "  - " << name << " уничтожен\n"; }
};

// Статическое время жизни: создаётся при первом вызове, живёт до конца программы.
Loud& shared_config() {
    static Loud cfg("static-config");  // Magic Static (потокобезопасно с C++11)
    return cfg;
}

int main() {
    std::cout << "--- автоматическое (стек) ---\n";
    {
        Loud a("auto-a");  // создаётся на стеке
        // уничтожается при выходе из блока
    }

    std::cout << "--- динамическое (куча) ---\n";
    {
        // RAII вместо new/delete: удаление произойдёт автоматически.
        auto p = std::make_unique<Loud>("heap-unique");
        std::cout << "  работаем с " << p->name << '\n';
        // unique_ptr вызовет delete при выходе из блока
    }

    std::cout << "--- временный объект ---\n";
    // Временный Loud живёт до конца полного выражения (точки с запятой).
    std::cout << "  длина имени = " << Loud("temp").name.size() << '\n';

    std::cout << "--- статическое (первый доступ) ---\n";
    shared_config();
    std::cout << "  повторный доступ — объект уже создан\n";
    shared_config();

    std::cout << "--- конец main (статика уничтожится после) ---\n";
    return 0;
}
