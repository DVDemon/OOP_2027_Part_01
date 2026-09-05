#pragma once

namespace mu {

// --- Объявления (прототипы) ---
// Сообщают компилятору имя, типы параметров и тип возврата. Тела нет.
// Определения находятся в math_utils.cpp (ровно по одному на программу — ODR).
int add(int a, int b);
int multiply(int a, int b);

}  // namespace mu
