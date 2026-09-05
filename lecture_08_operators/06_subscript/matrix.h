#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>

// Матрица с синтаксисом m[i][j]. Демонстрирует две версии operator[]:
// для модифицируемого (non-const) и для const доступа.
// Промежуточный класс-прокси Row позволяет получить второй [].
class Matrix {
    std::vector<double> data_;
    std::size_t rows_;
    std::size_t cols_;

public:
    Matrix(std::size_t r, std::size_t c)
        : data_(r * c, 0.0), rows_(r), cols_(c) {}

    std::size_t rows() const { return rows_; }
    std::size_t cols() const { return cols_; }

    // Прокси-строка для записи и чтения: m[i][j] = ...
    class Row {
        double* data_;
        std::size_t cols_;
    public:
        Row(double* d, std::size_t c) : data_(d), cols_(c) {}
        double& operator[](std::size_t j) {
            if (j >= cols_) throw std::out_of_range("column index");
            return data_[j];
        }
    };

    // Прокси-строка только для чтения (const-доступ).
    class ConstRow {
        const double* data_;
        std::size_t cols_;
    public:
        ConstRow(const double* d, std::size_t c) : data_(d), cols_(c) {}
        const double& operator[](std::size_t j) const {
            if (j >= cols_) throw std::out_of_range("column index");
            return data_[j];
        }
    };

    // non-const: разрешает запись.
    Row operator[](std::size_t i) {
        if (i >= rows_) throw std::out_of_range("row index");
        return Row(data_.data() + i * cols_, cols_);
    }

    // const: только чтение (для const Matrix& и const-методов).
    ConstRow operator[](std::size_t i) const {
        if (i >= rows_) throw std::out_of_range("row index");
        return ConstRow(data_.data() + i * cols_, cols_);
    }
};
