// Пример 08: вложенные классы и enum class.
#include <iostream>
#include <stdexcept>
#include <string_view>

// Односвязный список: вложенный класс Node скрыт внутри, а enum class Error
// задаёт перечень ошибок без засорения глобального пространства имён.
class LinkedList {
public:
    // enum class (scoped enum): нет неявного приведения к int,
    // имена доступны как LinkedList::Error::Empty.
    enum class Error { Empty, OutOfRange, InvalidArg };

    static std::string_view describe(Error e) {
        switch (e) {
            case Error::Empty:      return "список пуст";
            case Error::OutOfRange: return "индекс вне диапазона";
            case Error::InvalidArg: return "недопустимый аргумент";
        }
        return "неизвестная ошибка";
    }

private:
    // Вложенный класс — деталь реализации, не виден снаружи LinkedList.
    struct Node {
        int data;
        Node* next;
        Node(int d, Node* n = nullptr) : data(d), next(n) {}
    };

    Node* head_ = nullptr;
    std::size_t size_ = 0;

public:
    ~LinkedList() {
        while (head_) {
            Node* next = head_->next;
            delete head_;
            head_ = next;
        }
    }

    void pushFront(int val) {
        head_ = new Node(val, head_);
        ++size_;
    }

    int front() const {
        if (!head_) {
            throw Error::Empty;  // бросаем значение enum class
        }
        return head_->data;
    }

    std::size_t size() const { return size_; }
};

int main() {
    LinkedList list;
    list.pushFront(10);
    list.pushFront(20);
    list.pushFront(30);

    std::cout << "size  = " << list.size() << '\n';   // 3
    std::cout << "front = " << list.front() << '\n';  // 30

    // enum class не приводится к int неявно — нужен static_cast.
    LinkedList::Error e = LinkedList::Error::OutOfRange;
    std::cout << "Error::OutOfRange = " << static_cast<int>(e)
              << " (" << LinkedList::describe(e) << ")\n";

    // Демонстрация обработки исключения с enum class.
    LinkedList empty;
    try {
        empty.front();
    } catch (LinkedList::Error err) {
        std::cout << "поймана ошибка: " << LinkedList::describe(err) << '\n';
    }

    return 0;
}
