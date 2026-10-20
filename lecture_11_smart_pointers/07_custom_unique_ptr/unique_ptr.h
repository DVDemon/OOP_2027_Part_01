// Учебная реализация unique_ptr: эксклюзивное владение ресурсом.
#ifndef LECTURE13_CUSTOM_UNIQUE_PTR_H
#define LECTURE13_CUSTOM_UNIQUE_PTR_H

#include <utility>  // std::swap, std::forward

namespace edu {

template <typename T>
class UniquePtr {
public:
    // Конструктор: захватываем владение сырым указателем.
    explicit UniquePtr(T* p = nullptr) noexcept : ptr_(p) {}

    // Копирование запрещено — владелец может быть только один.
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // Перемещение: «крадём» ресурс у источника.
    UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    // Деструктор освобождает ресурс.
    ~UniquePtr() { delete ptr_; }

    // Операторы доступа.
    T& operator*() const { return *ptr_; }
    T* operator->() const noexcept { return ptr_; }
    T* get() const noexcept { return ptr_; }

    // Проверка на nullptr.
    explicit operator bool() const noexcept { return ptr_ != nullptr; }

    // Отказаться от владения: вернуть ресурс, сам стать пустым.
    T* release() noexcept {
        T* tmp = ptr_;
        ptr_ = nullptr;
        return tmp;
    }

    // Освободить текущий ресурс и (опционально) захватить новый.
    void reset(T* p = nullptr) noexcept {
        if (ptr_ != p) {
            delete ptr_;
            ptr_ = p;
        }
    }

    void swap(UniquePtr& other) noexcept { std::swap(ptr_, other.ptr_); }

private:
    T* ptr_;
};

// Аналог std::make_unique.
template <typename T, typename... Args>
UniquePtr<T> makeUnique(Args&&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

}  // namespace edu

#endif  // LECTURE13_CUSTOM_UNIQUE_PTR_H
