// Ручной динамический массив целых чисел (без std::vector).
// Учебный пример к пункту 5 и практическому заданию лекции 3.
#ifndef LECTURE_03_DYNAMIC_ARRAY_H
#define LECTURE_03_DYNAMIC_ARRAY_H

#include <cstddef>

namespace dyn {

// Создать массив заданного размера, заполненный нулями.
int* create_array(std::size_t size);

// Изменить размер массива (аналог realloc), сохранив данные.
// Возвращает новый блок; старый освобождается.
int* resize_array(int* old_array, std::size_t old_size, std::size_t new_size);

// Вставить value в позицию pos, сдвинув остальные вправо. size увеличивается на 1.
int* insert_at(int* array, std::size_t& size, std::size_t pos, int value);

// Удалить элемент из позиции pos, сдвинув остальные влево. size уменьшается на 1.
int* remove_at(int* array, std::size_t& size, std::size_t pos);

// Освободить память.
void destroy_array(int* array);

}  // namespace dyn

#endif  // LECTURE_03_DYNAMIC_ARRAY_H
