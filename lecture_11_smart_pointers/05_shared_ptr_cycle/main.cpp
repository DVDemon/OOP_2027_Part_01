// Пример 05: циклические shared_ptr между двумя классами — утечка при удалении.
//
// Классы ссылаются друг на друга «сильными» указателями: автор владеет
// документом, документ — автором. Счётчики ссылок никогда не падают до нуля,
// поэтому удаление внешних shared_ptr не освобождает объекты: память остаётся
// занятой до конца программы — это и есть утечка.
//
// Обход — сделать обратную связь наблюдающей: владельцем остаётся одна сторона
// (документ владеет автором), а вторая ссылается через weak_ptr.
#include <iostream>
#include <memory>
#include <string>
#include <utility>

// ---------------------------------------------------------------------------
// ПЛОХО: сильные ссылки в обе стороны.
// Author::document_ -> Document::author_ -> Author: цикл владения.
// ---------------------------------------------------------------------------
namespace bad {

class Document;

class Author {
public:
    explicit Author(std::string name) : name_(std::move(name)) {
        ++alive;
        std::cout << "  Author(" << name_ << ") создан\n";
    }
    ~Author() {
        --alive;
        std::cout << "  ~Author(" << name_ << ")\n";
    }

    const std::string& name() const { return name_; }
    void link(const std::shared_ptr<Document>& document) { document_ = document; }

    static int alive;  // сколько объектов живо прямо сейчас

private:
    std::string name_;
    std::shared_ptr<Document> document_;  // сильная обратная связь
};

class Document {
public:
    Document(std::string title, std::shared_ptr<Author> author)
        : title_(std::move(title)), author_(std::move(author)) {
        ++alive;
        std::cout << "  Document(" << title_ << ") создан\n";
    }
    ~Document() {
        --alive;
        std::cout << "  ~Document(" << title_ << ")\n";
    }

    const std::string& title() const { return title_; }
    const std::string& author_name() const { return author_->name(); }

    static int alive;

private:
    std::string title_;
    std::shared_ptr<Author> author_;  // документ владеет автором
};

int Author::alive = 0;
int Document::alive = 0;

void run() {
    auto author = std::make_shared<Author>("Толстой");
    auto document = std::make_shared<Document>("Война и мир", author);
    author->link(document);  // документ -> автор -> документ

    std::cout << "  use_count(author) = " << author.use_count()
              << ", use_count(document) = " << document.use_count() << '\n';

    // Слабые указатели нужны только для проверки: объекты живы или нет.
    std::weak_ptr<Author> watch_author = author;
    std::weak_ptr<Document> watch_document = document;

    author.reset();
    document.reset();  // снаружи ссылок больше нет

    std::cout << "  ссылки снаружи сброшены: автор жив — "
              << (watch_author.expired() ? "нет" : "да")
              << ", документ жив — "
              << (watch_document.expired() ? "нет" : "да") << '\n';
}

}  // namespace bad

// ---------------------------------------------------------------------------
// ХОРОШО: владение одностороннее, обратная связь — weak_ptr.
// Document владеет Author; Author наблюдает за Document.
// ---------------------------------------------------------------------------
namespace good {

class Document;

class Author {
public:
    explicit Author(std::string name) : name_(std::move(name)) {
        ++alive;
        std::cout << "  Author(" << name_ << ") создан\n";
    }
    ~Author() {
        --alive;
        std::cout << "  ~Author(" << name_ << ")\n";
    }

    const std::string& name() const { return name_; }
    void link(const std::shared_ptr<Document>& document) { document_ = document; }

    // Временный владелец документа: пусто, если документ уже удалён.
    std::shared_ptr<Document> document() const { return document_.lock(); }

    static int alive;

private:
    std::string name_;
    std::weak_ptr<Document> document_;  // наблюдение без владения
};

class Document {
public:
    Document(std::string title, std::shared_ptr<Author> author)
        : title_(std::move(title)), author_(std::move(author)) {
        ++alive;
        std::cout << "  Document(" << title_ << ") создан\n";
    }
    ~Document() {
        --alive;
        std::cout << "  ~Document(" << title_ << ")\n";
    }

    const std::string& title() const { return title_; }
    const std::string& author_name() const { return author_->name(); }

    static int alive;

private:
    std::string title_;
    std::shared_ptr<Author> author_;  // единственный владелец автора
};

int Author::alive = 0;
int Document::alive = 0;

void run() {
    auto author = std::make_shared<Author>("Толстой");
    auto document = std::make_shared<Document>("Война и мир", author);
    author->link(document);  // weak_ptr цикл не создаёт

    std::cout << "  use_count(author) = " << author.use_count()
              << ", use_count(document) = " << document.use_count() << '\n';
    std::cout << "  документ видит автора: " << document->author_name() << '\n';

    std::weak_ptr<Author> watch_author = author;
    std::weak_ptr<Document> watch_document = document;

    author.reset();
    std::cout << "  после reset(author) автор жив — "
              << (watch_author.expired() ? "нет" : "да") << " (его держит документ)\n";

    document.reset();  // владелец ушёл — освобождаются оба
    std::cout << "  после reset(document): автор жив — "
              << (watch_author.expired() ? "нет" : "да")
              << ", документ жив — "
              << (watch_document.expired() ? "нет" : "да") << '\n';
}

}  // namespace good

int main() {
    std::cout << "--- ПЛОХО: shared_ptr в обе стороны ---\n";
    {
        bad::run();
    }  // деструкторы не вызываются: объекты держат друг друга
    std::cout << "  живых Author = " << bad::Author::alive
              << ", Document = " << bad::Document::alive
              << " — память не освобождена (утечка)\n";

    std::cout << "\n--- ХОРОШО: обратная связь через weak_ptr ---\n";
    {
        good::run();
    }  // владение одностороннее — деструкторы вызваны
    std::cout << "  живых Author = " << good::Author::alive
              << ", Document = " << good::Document::alive
              << " — память освобождена\n";

    return 0;
}
