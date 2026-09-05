// Пример 01: коды ошибок vs исключения.
//
// Показываем два подхода к сообщению об ошибке и почему исключения
// сложнее «потерять».
#include <iostream>
#include <stdexcept>
#include <string>

// --- Подход 1: код возврата (C-стиль) ---
// Сигнатура «загрязнена»: полезный результат отдаётся через выходной параметр,
// а возврат — это код ошибки, который легко забыть проверить.
enum class ParseStatus { Ok, Empty, NotANumber };

ParseStatus parse_positive(const std::string& text, int& out) {
    if (text.empty()) return ParseStatus::Empty;
    try {
        int value = std::stoi(text);
        if (value <= 0) return ParseStatus::NotANumber;
        out = value;
        return ParseStatus::Ok;
    } catch (...) {
        return ParseStatus::NotANumber;
    }
}

// --- Подход 2: исключение (C++ стиль) ---
// Полезный результат возвращается напрямую; об ошибке нельзя «забыть»:
// непойманное исключение раскрутит стек и завершит программу.
int parse_positive_ex(const std::string& text) {
    if (text.empty())
        throw std::invalid_argument("Пустая строка");
    int value = std::stoi(text);  // сам бросит, если это не число
    if (value <= 0)
        throw std::out_of_range("Ожидалось положительное число: " + text);
    return value;
}

int main() {
    // --- Коды ошибок: проверка обязательна вручную ---
    int parsed = 0;
    ParseStatus status = parse_positive("42", parsed);
    if (status == ParseStatus::Ok) {
        std::cout << "Код возврата: разобрано " << parsed << '\n';
    } else {
        std::cout << "Код возврата: ошибка разбора\n";
    }

    // Опасность: если просто не проверить status, ошибка останется незамеченной,
    // а `parsed` будет содержать мусор/старое значение.

    // --- Исключения: ошибку нельзя проигнорировать молча ---
    try {
        std::cout << "Исключение: разобрано " << parse_positive_ex("100") << '\n';
        std::cout << "Исключение: разобрано " << parse_positive_ex("xyz") << '\n';
        std::cout << "Эта строка не выполнится\n";
    } catch (const std::exception& e) {
        std::cout << "Исключение поймано: " << e.what() << '\n';
    }

    return 0;
}
