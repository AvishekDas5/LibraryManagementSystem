#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QMessageBox>
#include <QDebug>

class Book {
public:
    std::string id;
    std::string title;
    std::string author;
    std::string status;
    std::string studentName;
    Book* next;

    Book(const std::string& id, const std::string& title, const std::string& author)
        : id(id), title(title), author(author), status("available"), studentName(""), next(nullptr) {}
};

class Library : public QWidget {
    Q_OBJECT
private:
    Book* head;

    QListWidget* bookListWidget;
    QLineEdit* idLineEdit;
    QLineEdit* titleLineEdit;
    QLineEdit* authorLineEdit;
    QLineEdit* studentNameLineEdit;

public:
    Library(QWidget* parent = nullptr) : QWidget(parent), head(nullptr) {
        QVBoxLayout* mainLayout = new QVBoxLayout(this);

        idLineEdit = new QLineEdit(this);
        titleLineEdit = new QLineEdit(this);
        authorLineEdit = new QLineEdit(this);
        studentNameLineEdit = new QLineEdit(this);

        QPushButton* addButton = new QPushButton("Add Book", this);
        connect(addButton, &QPushButton::clicked, this, &Library::addBook);

        QPushButton* issueButton = new QPushButton("Issue Book", this);
        connect(issueButton, &QPushButton::clicked, this, &Library::issueBook);

        QPushButton* returnButton = new QPushButton("Return Book", this);
        connect(returnButton, &QPushButton::clicked, this, &Library::returnBook);

        QPushButton* listButton = new QPushButton("List All Books", this);
        connect(listButton, &QPushButton::clicked, this, &Library::listAllBooks);

        QPushButton* searchButton = new QPushButton("Search Book", this);
        connect(searchButton, &QPushButton::clicked, this, &Library::searchBook);

        QPushButton* deleteButton = new QPushButton("Delete Book", this);
        connect(deleteButton, &QPushButton::clicked, this, &Library::deleteBook);

        bookListWidget = new QListWidget(this);

        mainLayout->addWidget(new QLabel("Library Management System", this));
        mainLayout->addWidget(new QLabel("Book ID:", this));
        mainLayout->addWidget(idLineEdit);
        mainLayout->addWidget(new QLabel("Title:", this));
        mainLayout->addWidget(titleLineEdit);
        mainLayout->addWidget(new QLabel("Author:", this));
        mainLayout->addWidget(authorLineEdit);
        mainLayout->addWidget(addButton);
        mainLayout->addWidget(new QLabel("Student Name:", this));
        mainLayout->addWidget(studentNameLineEdit);
        mainLayout->addWidget(issueButton);
        mainLayout->addWidget(returnButton);
        mainLayout->addWidget(listButton);
        mainLayout->addWidget(searchButton);
        mainLayout->addWidget(deleteButton);
        mainLayout->addWidget(new QLabel("Book List:", this));
        mainLayout->addWidget(bookListWidget);
    }

private slots:
    void addBook() {
        std::string id = idLineEdit->text().toStdString();
        std::string title = titleLineEdit->text().toStdString();
        std::string author = authorLineEdit->text().toStdString();

        if (findBookByID(id) != nullptr) {
            QMessageBox::information(this, "Duplicate Book", "Book with ID " + QString::fromStdString(id) + " already exists.");
            return;
        }

        Book* newBook = new Book(id, title, author);

        if (head == nullptr) {
            head = newBook;
        } else {
            Book* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newBook;
        }

        updateBookList();

        idLineEdit->clear();
        titleLineEdit->clear();
        authorLineEdit->clear();
    }

    Book* findBookByID(const std::string& id) {
        Book* temp = head;
        while (temp != nullptr) {
            if (temp->id == id) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    void listAllBooks() {
        Book* temp = head;
        if (temp == nullptr) {
            QMessageBox::information(this, "No Books", "No books in the library.");
            return;
        }

        while (temp != nullptr) {
            QString bookInfo = "ID: " + QString::fromStdString(temp->id) +
                               ", Title: " + QString::fromStdString(temp->title) +
                               ", Author: " + QString::fromStdString(temp->author) +
                               ", Status: " + QString::fromStdString(temp->status);
            if (temp->status == "issued") {
                bookInfo += ", Student Name: " + QString::fromStdString(temp->studentName);
            }
            new QListWidgetItem(bookInfo, bookListWidget);
            temp = temp->next;
        }
    }

    void issueBook() {
        std::string id = idLineEdit->text().toStdString();
        std::string studentName = studentNameLineEdit->text().toStdString();

        Book* book = findBookByID(id);
        if (book != nullptr) {
            if (book->status == "available") {
                book->status = "issued";
                book->studentName = studentName;
                QMessageBox::information(this, "Book Issued", "Book " + QString::fromStdString(book->title) + " issued to " + QString::fromStdString(studentName) + ".");
            } else {
                QMessageBox::information(this, "Already Issued", "Book " + QString::fromStdString(book->title) + " is already issued.");
            }
        } else {
            QMessageBox::information(this, "Book Not Found", "Book with ID " + QString::fromStdString(id) + " not found.");
        }

        updateBookList();
    }

    void returnBook() {
        std::string id = idLineEdit->text().toStdString();

        Book* book = findBookByID(id);
        if (book != nullptr) {
            if (book->status == "issued") {
                book->status = "available";
                book->studentName = "";
                QMessageBox::information(this, "Book Returned", "Book " + QString::fromStdString(book->title) + " returned successfully.");
            } else {
                QMessageBox::information(this, "Already Available", "Book " + QString::fromStdString(book->title) + " is already available.");
            }
        } else {
            QMessageBox::information(this, "Book Not Found", "Book with ID " + QString::fromStdString(id) + " not found.");
        }

        updateBookList();
    }

    void deleteBook() {
        std::string id = idLineEdit->text().toStdString();

        Book* temp = head;
        Book* prev = nullptr;

        while (temp != nullptr) {
            if (temp->id == id) {
                if (prev == nullptr) {
                    head = temp->next;
                } else {
                    prev->next = temp->next;
                }
                delete temp;
                QMessageBox::information(this, "Book Deleted", "Book with ID " + QString::fromStdString(id) + " deleted successfully.");
                updateBookList();
                return;
            }

            prev = temp;
            temp = temp->next;
        }

        QMessageBox::information(this, "Book Not Found", "Book with ID " + QString::fromStdString(id) + " not found.");
    }

    void searchBook() {
        std::string keyword = idLineEdit->text().toStdString();
        Book* foundBook = findBookByID(keyword);

        if (foundBook != nullptr) {
            QMessageBox::information(this, "Book Found",
                                     "ID: " + QString::fromStdString(foundBook->id) +
                                         ", Title: " + QString::fromStdString(foundBook->title) +
                                         ", Author: " + QString::fromStdString(foundBook->author));
        } else {
            QMessageBox::information(this, "Book Not Found", "Book with ID " + QString::fromStdString(keyword) + " not found.");
        }
    }

    void updateBookList() {
        bookListWidget->clear();

        Book* temp = head;
        while (temp != nullptr) {
            QString bookInfo = "ID: " + QString::fromStdString(temp->id) +
                               ", Title: " + QString::fromStdString(temp->title) +
                               ", Author: " + QString::fromStdString(temp->author) +
                               ", Status: " + QString::fromStdString(temp->status);
            if (temp->status == "issued") {
                bookInfo += ", Student Name: " + QString::fromStdString(temp->studentName);
            }
            new QListWidgetItem(bookInfo, bookListWidget);
            temp = temp->next;
        }
    }
public:
    ~Library() {
        Book* temp = head;
        while (temp != nullptr) {
            Book* nextBook = temp->next;
            delete temp;
            temp = nextBook;
        }
    }
};

#include "main.moc"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Library library;
    library.show();

    return app.exec();
}


