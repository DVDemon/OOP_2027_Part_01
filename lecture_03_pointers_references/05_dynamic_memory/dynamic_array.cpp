// Реализация ручного динамического массива.
// Каждый new[] парно освобождается delete[]. 
#include "dynamic_array.h"

namespace dyn {

int* create_array(std::size_t size) {
    int* data = new int[size]{};  // value-initialization -> все нули
    return data;
}

int* resize_array(int* old_array, std::size_t old_size, std::size_t new_size) {
    int* data = new int[new_size]{};
    std::size_t to_copy = (old_size < new_size) ? old_size : new_size;
    for (std::size_t i = 0; i < to_copy; ++i) {
        data[i] = old_array[i];
    }
    delete[] old_array;  // освобождаем старый блок
    return data;
}

int* insert_at(int* array, std::size_t& size, std::size_t pos, int value) {
    if (pos > size) {
        pos = size;  // вставка в конец, если позиция за границей
    }
    int* data = new int[size + 1]{};
    for (std::size_t i = 0; i < pos; ++i) {
        data[i] = array[i];
    }
    data[pos] = value;
    for (std::size_t i = pos; i < size; ++i) {
        data[i + 1] = array[i];
    }
    delete[] array;
    ++size;
    return data;
}

int* remove_at(int* array, std::size_t& size, std::size_t pos) {
    if (size == 0 || pos >= size) {
        return array;  // нечего удалять
    }
    int* data = new int[size - 1]{};
    for (std::size_t i = 0; i < pos; ++i) {
        data[i] = array[i];
    }
    for (std::size_t i = pos + 1; i < size; ++i) {
        data[i - 1] = array[i];
    }
    delete[] array;
    --size;
    return data;
}

void destroy_array(int* array) {
    delete[] array;  // delete[] корректно работает и с nullptr
}

}  // namespace dyn
