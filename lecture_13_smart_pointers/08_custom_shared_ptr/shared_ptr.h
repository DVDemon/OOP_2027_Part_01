// Учебная реализация shared_ptr: разделяемое владение через подсчёт ссылок.
// Внимание: учебная версия НЕ потокобезопасна (см. README).
#ifndef LECTURE13_CUSTOM_SHARED_PTR_H
#define LECTURE13_CUSTOM_SHARED_PTR_H

#include <cstddef>  // std::size_t

namespace edu {

template <typename T>
class SharedPtr {
public:
    // Конструктор: создаём счётчик ссылок, если есть ресурс.
    explicit SharedPtr(T* p = nullptr)
        : ptr_(p), ref_count_(p ? new std::size_t(1) : nullptr) {}

    // Копирование: разделяем ресурс, увеличиваем счётчик.
    SharedPtr(const SharedPtr& other) noexcept
        : ptr_(other.ptr_), ref_count_(other.ref_count_) {
        if (ref_count_) ++(*ref_count_);
    }

    // Перемещение: «крадём» ресурс и счётчик.
    SharedPtr(SharedPtr&& other) noexcept
        : ptr_(other.ptr_), ref_count_(other.ref_count_) {
        other.ptr_ = nullptr;
        other.ref_count_ = nullptr;
    }

    ~SharedPtr() { release(); }

    SharedPtr& operator=(const SharedPtr& other) noexcept {
        if (this != &other) {
            release();
            ptr_ = other.ptr_;
            ref_count_ = other.ref_count_;
            if (ref_count_) ++(*ref_count_);
        }
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            release();
            ptr_ = other.ptr_;
            ref_count_ = other.ref_count_;
            other.ptr_ = nullptr;
            other.ref_count_ = nullptr;
        }
        return *this;
    }

    // Операторы доступа.
    T& operator*() const { return *ptr_; }
    T* operator->() const noexcept { return ptr_; }
    T* get() const noexcept { return ptr_; }

    std::size_t use_count() const noexcept {
        return ref_count_ ? *ref_count_ : 0;
    }
    explicit operator bool() const noexcept { return ptr_ != nullptr; }

    // Отпустить текущий ресурс и (опционально) захватить новый.
    void reset(T* p = nullptr) {
        release();
        ptr_ = p;
        ref_count_ = p ? new std::size_t(1) : nullptr;
    }

private:
    // Уменьшить счётчик; при достижении нуля — освободить объект и счётчик.
    void release() noexcept {
        if (ref_count_) {
            --(*ref_count_);
            if (*ref_count_ == 0) {
                delete ptr_;
                delete ref_count_;
            }
        }
        ptr_ = nullptr;
        ref_count_ = nullptr;
    }

    T* ptr_;
    std::size_t* ref_count_;
};

}  // namespace edu

#endif  // LECTURE13_CUSTOM_SHARED_PTR_H
