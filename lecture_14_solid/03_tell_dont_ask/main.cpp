// Пример 03: Tell, Don't Ask (TDA).
//
// Принцип: не спрашивай объект о его состоянии, чтобы принять решение, —
// скажи ему, что делать. Объект сам проверяет свои инварианты.
//
// ПЛОХО (Ask) — логика вытекла наружу, в вызывающий код:
//
//     void process_order(Order& order, Warehouse& wh) {
//         if (order.get_status() == Status::Paid) {   // Ask
//             if (order.get_total() > 0) {             // Ask
//                 order.set_status(Status::Shipped);   // Tell (но поздно)
//                 wh.ship(order.get_items());
//             }
//         }
//     }
//
// При изменении бизнес-правил пришлось бы править каждое такое место вызова.
//
// ХОРОШО (Tell) — поручаем действие объекту; инкапсуляция сохранена.

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

enum class Status { Created, Paid, Shipped };

class Warehouse {
public:
    void ship(const std::vector<std::string>& items) {
        std::cout << "Отгружено товаров: " << items.size() << '\n';
    }
};

class Order {
    Status status_ = Status::Created;
    double total_ = 0.0;
    std::vector<std::string> items_;

public:
    void add_item(std::string name, double price) {
        items_.push_back(std::move(name));
        total_ += price;
    }

    void mark_paid() {
        if (total_ <= 0.0) {
            throw std::logic_error("Нельзя оплатить пустой заказ");
        }
        status_ = Status::Paid;
    }

    // Tell: клиент просто говорит «отгрузи», а Order сам решает, можно ли.
    void ship(Warehouse& wh) {
        if (status_ != Status::Paid || total_ <= 0.0) {
            throw std::logic_error("Нельзя отгрузить неоплаченный заказ");
        }
        wh.ship(items_);
        status_ = Status::Shipped;
    }
};

int main() {
    Warehouse wh;

    Order order;
    order.add_item("Книга", 500.0);
    order.add_item("Ручка", 50.0);
    order.mark_paid();

    order.ship(wh);  // одна команда — без расспросов о состоянии

    try {
        order.ship(wh);  // повторная отгрузка нарушает инвариант
    } catch (const std::logic_error& e) {
        std::cout << "Ошибка: " << e.what() << '\n';
    }

    return 0;
}
