// Пример 05: SRP — принцип единственной ответственности.
//
// «У класса должна быть только одна причина для изменения» — Роберт Мартин.
//
// ПЛОХО (этап 1) — God Object: Employee знает про бизнес-логику, БД, отчёты и UI:
//
//     class Employee {
//         std::string name_;  double salary_;
//     public:
//         double calculate_pay() const;          // бизнес-логика
//         void   save_to_db(Database&);          // персистентность
//         std::string generate_report() const;   // отчётность
//         void   display(Screen&);               // UI
//     };
//
// Четыре причины для изменения => четыре источника поломок.
//
// ХОРОШО (этап 4) — каждая ответственность вынесена в свой класс:
//   * Employee               — чистая доменная модель;
//   * EmployeeRepository      — хранение;
//   * EmployeeReportGenerator — формирование отчёта;
//   * EmployeeView            — вывод на экран.

#include <iostream>
#include <string>
#include <utility>

// --- Доменная модель: только бизнес-логика и данные ---
class Employee {
    std::string name_;
    double salary_;

public:
    Employee(std::string name, double salary)
        : name_(std::move(name)), salary_(salary) {}

    double calculate_pay() const { return salary_ * 1.13; }
    const std::string& name() const { return name_; }
    double salary() const { return salary_; }
};

// --- Инфраструктура: хранение (имитация БД) ---
class EmployeeRepository {
public:
    void save(const Employee& emp) {
        std::cout << "[DB] INSERT INTO employees (name) VALUES ('"
                  << emp.name() << "')\n";
    }
};

// --- Представление: формирование отчёта ---
class EmployeeReportGenerator {
public:
    std::string generate(const Employee& emp) const {
        return "Сотрудник: " + emp.name() +
               ", к выплате: " + std::to_string(emp.calculate_pay());
    }
};

// --- Представление: вывод на экран ---
class EmployeeView {
public:
    void display(const std::string& report) const {
        std::cout << "[Экран] " << report << '\n';
    }
};

int main() {
    Employee emp("Иванов", 100000.0);

    EmployeeRepository repo;
    EmployeeReportGenerator reporter;
    EmployeeView view;

    repo.save(emp);
    const std::string report = reporter.generate(emp);
    view.display(report);

    // У каждого класса ровно одна причина для изменения.
    return 0;
}
