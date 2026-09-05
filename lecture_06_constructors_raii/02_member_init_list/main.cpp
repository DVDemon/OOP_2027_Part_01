// Пример 02: список инициализации членов (MIL) и порядок инициализации.
//   - const-поля и ссылки можно инициализировать ТОЛЬКО через MIL;
//   - поля инициализируются в порядке ОБЪЯВЛЕНИЯ, а не в порядке MIL.
#include <iostream>
#include <string>

class Student {
    const int id_;            // const — только через MIL
    std::string& group_ref_;  // ссылка — только через MIL
    std::string name_;
    double gpa_;

public:
    Student(int id, std::string& group, const std::string& name, double gpa)
        : id_(id)             // объявлен 1-м -> инициализируется 1-м
        , group_ref_(group)   // 2-м
        , name_(name)         // 3-м
        , gpa_(gpa)           // 4-м
    {
        // Тело конструктора — для дополнительной логики/проверок.
    }

    void print() const {
        std::cout << "id=" << id_ << " group=" << group_ref_
                  << " name=" << name_ << " gpa=" << gpa_ << '\n';
    }
};

// Демонстрация порядка инициализации: поля печатают себя в конструкторе.
struct Tracer {
    explicit Tracer(const char* tag) {
        std::cout << "  инициализирован " << tag << '\n';
    }
};

class Ordered {
    Tracer a_;   // объявлен 1-м
    Tracer b_;   // объявлен 2-м
    Tracer c_;   // объявлен 3-м

public:
    // Намеренно «перепутанный» порядок в MIL.
    // Реальный порядок задаётся объявлением: a_, b_, c_.
    Ordered() : c_("c_"), b_("b_"), a_("a_") {
        std::cout << "  тело Ordered()\n";
    }
};

int main() {
    std::cout << "--- MIL: const и ссылка ---\n";
    std::string group = "BO-101";
    Student s(42, group, "Иванов", 4.5);
    s.print();

    group = "BO-202";  // меняем строку — ссылка group_ref_ видит изменение
    std::cout << "после изменения group: ";
    s.print();

    std::cout << "--- порядок инициализации = порядок объявления ---\n";
    Ordered o;
    (void)o;

    return 0;
}
