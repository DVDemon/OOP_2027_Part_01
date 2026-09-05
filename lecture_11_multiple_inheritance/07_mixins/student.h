#pragma once

#include <string>

#include "mixins.h"

// Student подмешивает (mixin) обе примеси через множественное наследование.
// Это безопасный вид MI: базы-примеси не содержат собственных данных и не
// образуют ромб. Достаточно реализовать toString(), == и < — остальное
// (print, !=, >, <=, >=) примеси дают сами.
class Student : public Printable<Student>, public Comparable<Student> {
    std::string name_;
    double gpa_;

public:
    Student(std::string name, double gpa)
        : name_(std::move(name)), gpa_(gpa) {}

    std::string toString() const {
        return name_ + " (GPA: " + std::to_string(gpa_) + ")";
    }

    // Сравнение студентов по среднему баллу.
    bool operator==(const Student& other) const { return gpa_ == other.gpa_; }
    bool operator<(const Student& other) const { return gpa_ < other.gpa_; }

    double gpa() const { return gpa_; }
};
