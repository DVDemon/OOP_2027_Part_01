// Единица трансляции с ОПРЕДЕЛЕНИЕМ функции add.
// Во всей программе нешаблонная функция с внешней связью определяется
// ровно один раз — это требование ODR (One Definition Rule).
#include "math_utils.h"

namespace math_utils {

int add(int a, int b) {
    return a + b;
}

}  // namespace math_utils
