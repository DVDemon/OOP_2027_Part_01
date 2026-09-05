// Пример 07: потоковый ввод-вывод и манипуляторы.
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

int main() {
    std::cout << "=== Точность и форма вывода double ===\n";
    double pi = 3.141592653589793;
    std::cout << "по умолчанию : " << pi << '\n';
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "fixed(2)     : " << pi << '\n';
    std::cout << std::scientific;
    std::cout << "scientific   : " << pi << '\n';
    std::cout.unsetf(std::ios::floatfield);  // сброс формата обратно

    std::cout << "\n=== Ширина поля и выравнивание ===\n";
    std::cout << '[' << std::setw(10) << std::right << 42 << "]\n";
    std::cout << '[' << std::setw(10) << std::left << 42 << "]\n";
    std::cout << '[' << std::setfill('0') << std::setw(5) << 42 << "]\n";
    std::cout << std::setfill(' ');  // вернуть заполнитель-пробел

    std::cout << "\n=== Системы счисления ===\n";
    int num = 255;
    std::cout << "dec : " << std::dec << num << '\n';
    std::cout << "hex : " << std::hex << num << '\n';
    std::cout << "oct : " << std::oct << num << '\n';
    std::cout << "0x  : " << std::showbase << std::hex << num << '\n';
    std::cout << std::dec << std::noshowbase;  // вернуть десятичный вид

    std::cout << "\n=== Логические значения ===\n";
    std::cout << "boolalpha: " << std::boolalpha << true << ' ' << false << '\n';
    std::cout << std::noboolalpha;

    std::cout << "\n=== Чтение и проверка корректности ввода ===\n";
    // Чтобы пример запускался без интерактивного ввода, читаем из
    // istringstream. С реальным вводом это был бы std::cin.
    std::istringstream input("42 not_a_number");

    int value = 0;
    if (input >> value) {  // ХОРОШО: проверяем результат операции >>
        std::cout << "прочитано число: " << value << '\n';
    } else {
        std::cout << "ошибка ввода первого числа\n";
    }

    int bad = 0;
    if (input >> bad) {
        std::cout << "не должно случиться: " << bad << '\n';
    } else {
        std::cout << "ввод 'not_a_number' как int отвергнут (поток в failed-состоянии)\n";
        input.clear();  // сбросить флаг ошибки, иначе поток непригоден дальше
    }

    std::cout << "\n=== >> и getline: ловушка с '\\n' ===\n";
    std::istringstream src("7\nИван Петров");
    int n = 0;
    src >> n;            // читает 7, но '\n' остаётся в буфере
    std::string line;
    src.ignore();        // ХОРОШО: пропускаем оставшийся '\n' перед getline
    std::getline(src, line);
    std::cout << "n=" << n << ", строка='" << line << "'\n";

    return 0;
}
