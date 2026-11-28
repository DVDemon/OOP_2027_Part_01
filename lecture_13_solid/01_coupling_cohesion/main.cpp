// Пример 01: связанность (coupling) и связность (cohesion).
//
// ПЛОХО (высокая связанность) — класс Report напрямую содержит конкретные
// Database и PdfFormatter. Любое изменение этих классов ломает Report,
// а подменить источник данных или формат вывода невозможно:
//
//     class Report {
//         Database db_;        // прямая зависимость от конкретной БД
//         PdfFormatter fmt_;   // прямая зависимость от конкретного формата
//     public:
//         void generate() {
//             auto data = db_.query("SELECT * FROM sales");
//             fmt_.render(data);
//         }
//     };
//
// ХОРОШО (слабая связанность, высокая связность) — Report зависит только
// от абстракций IDataSource и IFormatter. Каждый класс делает ровно одно
// дело (high cohesion), а конкретные реализации подставляются снаружи.

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// --- Абстракции (контракты), от которых зависит бизнес-логика ---

class IDataSource {
public:
    virtual ~IDataSource() = default;
    virtual std::vector<std::string> fetch() const = 0;
};

class IFormatter {
public:
    virtual ~IFormatter() = default;
    virtual void render(const std::vector<std::string>& data) const = 0;
};

// --- Конкретные реализации (низкоуровневые детали) ---

class SalesDataSource : public IDataSource {
public:
    std::vector<std::string> fetch() const override {
        return {"Январь: 100", "Февраль: 120", "Март: 95"};
    }
};

class PlainTextFormatter : public IFormatter {
public:
    void render(const std::vector<std::string>& data) const override {
        std::cout << "=== Отчёт о продажах ===\n";
        for (const auto& row : data) {
            std::cout << "  " << row << '\n';
        }
    }
};

// --- Высокосвязный класс со слабой связанностью ---
// Report делает одно дело: соединяет источник данных и форматтер.
class Report {
    const IDataSource& source_;
    const IFormatter& formatter_;

public:
    Report(const IDataSource& src, const IFormatter& fmt)
        : source_(src), formatter_(fmt) {}

    void generate() const {
        const auto data = source_.fetch();
        formatter_.render(data);
    }
};

int main() {
    SalesDataSource source;
    PlainTextFormatter formatter;

    // Подставляем зависимости снаружи. Чтобы сменить БД или формат,
    // достаточно передать другую реализацию — Report не меняется.
    Report report(source, formatter);
    report.generate();

    return 0;
}
