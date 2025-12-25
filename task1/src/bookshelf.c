#include "bookshelf.h"

BookArray* createBookArray(int initialCapacity) {
    BookArray* array = (BookArray*)malloc(sizeof(BookArray));
    if (array == NULL) {
        printf("Memory allocation error for book array!\n");
        return NULL;
    }

    array->books = (Book*)malloc(initialCapacity * sizeof(Book));
    if (array->books == NULL) {
        free(array);
        printf("Memory allocation error for books!\n");
        return NULL;
    }

    array->size = 0;
    array->capacity = initialCapacity;
    return array;
}

void freeBookArray(BookArray* array) {
    if (array != NULL) {
        free(array->books);
        free(array);
    }
}

int addBook(BookArray* array, Book book) {
    if (array->size >= array->capacity) {
        // Увеличиваем емкость вдвое
        int newCapacity = array->capacity * 2;
        Book* newBooks = (Book*)realloc(array->books, newCapacity * sizeof(Book));
        if (newBooks == NULL) {
            printf("Memory reallocation error!\n");
            return 0;
        }
        array->books = newBooks;
        array->capacity = newCapacity;
    }

    array->books[array->size] = book;
    array->size++;
    return 1;
}

int insertBook(BookArray* array, Book book, int index) {
    if (index < 0 || index > array->size) {
        printf("Invalid index!\n");
        return 0;
    }

    if (array->size >= array->capacity) {
        int newCapacity = array->capacity * 2;
        Book* newBooks = (Book*)realloc(array->books, newCapacity * sizeof(Book));
        if (newBooks == NULL) return 0;
        array->books = newBooks;
        array->capacity = newCapacity;
    }

    // Сдвигаем элементы для вставки
    for (int i = array->size; i > index; i--) {
        array->books[i] = array->books[i - 1];
    }

    array->books[index] = book;
    array->size++;
    return 1;
}

int removeBook(BookArray* array, int index) {
    if (index < 0 || index >= array->size) {
        printf("Invalid index!\n");
        return 0;
    }

    for (int i = index; i < array->size - 1; i++) {
        array->books[i] = array->books[i + 1];
    }

    array->size--;
    return 1;
}

int updateBook(BookArray* array, int index, Book book) {
    if (index < 0 || index >= array->size) {
        printf("Invalid index!\n");
        return 0;
    }

    array->books[index] = book;
    return 1;
}

BookArray* inputBooksFixedCount(int count) {
    BookArray* array = createBookArray(count > 10 ? count : 10);

    printf("=== Input %d books ===\n", count);
    printf("Enter 'CANCEL' as title to stop early\n\n");

    for (int i = 0; i < count; i++) {
        printf("Book %d of %d:\n", i + 1, count);
        Book book = createBookFromInput(1); // Автогенерация ID

        // Проверка на отмену
        if (strcmp(book.title, "CANCEL") == 0) {
            printf("Input stopped after %d books.\n", i);
            break;
        }

        addBook(array, book);
    }

    return array;
}

BookArray* inputBooksUntilCondition() {
    BookArray* array = createBookArray(10);
    char stopTitle[100] = "STOP"; // Признак остановки

    printf("=== Input books (enter 'STOP' in title field to stop) ===\n");

    int bookCount = 0;
    while (1) {
        printf("\nBook %d:\n", ++bookCount);
        Book book = createBookFromInput(1);

        // Проверка условия остановки
        if (strcmp(book.title, stopTitle) == 0) {
            printf("Input completed by condition.\n");
            break;
        }

        addBook(array, book);

        // Диалог о продолжении ввода
        char choice;
        printf("Continue input? (y/n): ");
        scanf(" %c", &choice);
        getchar(); // Очистка буфера

        if (choice != 'y' && choice != 'Y') {
            break;
        }
    }

    return array;
}

BookArray* inputBooksInteractive() {
    BookArray* array = createBookArray(10);

    printf("=== Interactive book input ===\n");
    printf("Enter 'CANCEL' as title to stop\n\n");

    int bookCount = 0;

    while (1) {
        printf("\nBook %d:\n", ++bookCount);
        Book book = createBookFromInput(1);

        // Проверка на отмену
        if (strcmp(book.title, "CANCEL") == 0) {
            printf("Input stopped after %d books.\n", bookCount - 1);
            break;
        }

        addBook(array, book);

        // Диалог о продолжении ввода
        if (bookCount > 0) {
            char continueInput;
            printf("Add another book? (y/n): ");
            scanf(" %c", &continueInput);
            getchar();

            if (continueInput != 'y' && continueInput != 'Y') {
                break;
            }
        }
    }

    return array;
}

Book createBookFromInput(int autoGenerateID) {
    Book book;
    char input[100];
    static int lastID = 1000;

    // Использование union для ID
    if (autoGenerateID) {
        book.id.numeric_id = ++lastID;
        printf("Auto-generated ID: %d\n", book.id.numeric_id);
    }
    else {
        int idChoice;
        printf("Choose ID type (1 - numeric, 2 - string, 0 to cancel): ");
        scanf("%d", &idChoice);
        getchar();

        if (idChoice == 0) {
            strcpy(book.title, "CANCEL");
            return book;
        }

        if (idChoice == 1) {
            printf("Enter numeric ID: ");
            scanf("%d", &book.id.numeric_id);
            getchar();
        }
        else if (idChoice == 2) {
            printf("Enter string ID (e.g., ISBN): ");
            fgets(book.id.string_id, 20, stdin);
            book.id.string_id[strcspn(book.id.string_id, "\n")] = '\0';
        }
        else {
            printf("Invalid choice, using numeric ID\n");
            book.id.numeric_id = ++lastID;
        }
    }

    printf("Author (or 'CANCEL' to stop): ");
    fgets(input, 100, stdin);
    input[strcspn(input, "\n")] = '\0';

    if (strcmp(input, "CANCEL") == 0) {
        strcpy(book.title, "CANCEL");
        return book;
    }
    strcpy(book.author, input);

    printf("Title (or 'CANCEL' to stop): ");
    fgets(input, 100, stdin);
    input[strcspn(input, "\n")] = '\0';

    if (strcmp(input, "CANCEL") == 0) {
        strcpy(book.title, "CANCEL");
        return book;
    }
    strcpy(book.title, input);

    printf("Year (0 to cancel): ");
    int year;
    scanf("%d", &year);
    getchar();

    if (year == 0) {
        strcpy(book.title, "CANCEL");
        return book;
    }
    book.year = year;

    printf("Publisher: ");
    fgets(input, 50, stdin);
    input[strcspn(input, "\n")] = '\0';
    strcpy(book.publisher, input);

    printf("Pages (0 to cancel): ");
    int pages;
    scanf("%d", &pages);
    getchar();

    if (pages == 0) {
        strcpy(book.title, "CANCEL");
        return book;
    }
    book.pages = pages;

    return book;
}

// Функция 2: Просмотр содержимого массива
void displayBooks(const BookArray* array) {
    if (array == NULL || array->size == 0) {
        printf("Book array is empty.\n");
        return;
    }

    printf("\n=== List of all books (%d items) ===\n", array->size);
    printf("=======================================================================================\n");
    printf("|  ID  | Author                  | Title                | Year | Publisher      | Pages |\n");
    printf("=======================================================================================\n");

    for (int i = 0; i < array->size; i++) {
        printf("| %-3d | %-22s| %-20s | %-4d | %-14s | %-6d|\n",
            array->books[i].id.numeric_id,
            array->books[i].author,
            array->books[i].title,
            array->books[i].year,
            array->books[i].publisher,
            array->books[i].pages);
    }
    printf("=======================================================================================\n");
}

// Функция 3: Дополнение существующего массива
void addBooksToExisting(BookArray* existingArray) {
    if (existingArray == NULL) {
        printf("Array not initialized!\n");
        return;
    }

    int additionalCount;
    printf("How many books to add? (0 to cancel): ");
    scanf("%d", &additionalCount);
    getchar();

    if (additionalCount == 0) {
        printf("Addition cancelled.\n");
        return;
    }

    if (additionalCount < 0) {
        printf("Invalid number! Must be positive.\n");
        return;
    }

    for (int i = 0; i < additionalCount; i++) {
        printf("\nBook %d (total number %d):\n",
            i + 1, existingArray->size + i + 1);

        // Предоставляем возможность отмены для каждой книги
        printf("Enter 'CANCEL' to stop adding books\n");
        Book book = createBookFromInput(1);

        // Проверка на отмену
        if (strcmp(book.title, "CANCEL") == 0) {
            printf("Book addition stopped by user.\n");
            break;
        }

        addBook(existingArray, book);
    }

    printf("\nAdded books. Total count: %d\n", existingArray->size);
}

void findBooksByYear(const BookArray* array, int targetYear) {
    if (array == NULL || array->size == 0) {
        printf("Book array is empty.\n");
        return;
    }

    printf("\n=== Books published after %d ===\n", targetYear);
    int found = 0;

    for (int i = 0; i < array->size; i++) {
        if (array->books[i].year > targetYear) {
            printBook(&array->books[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("No books published after %d found.\n", targetYear);
    }
}

void findBooksByAuthor(const BookArray* array, const char* author) {
    if (array == NULL || array->size == 0) {
        printf("Book array is empty.\n");
        return;
    }

    printf("\n=== Books by author '%s' ===\n", author);
    int found = 0;

    for (int i = 0; i < array->size; i++) {
        if (strstr(array->books[i].author, author) != NULL) {
            printBook(&array->books[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("No books by author '%s' found.\n", author);
    }
}

void findBooksByPublisher(const BookArray* array, const char* publisher) {
    if (array == NULL || array->size == 0) {
        printf("Book array is empty.\n");
        return;
    }

    printf("\n=== Books by publisher '%s' ===\n", publisher);
    int found = 0;

    for (int i = 0; i < array->size; i++) {
        if (strcmp(array->books[i].publisher, publisher) == 0) {
            printBook(&array->books[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("No books by publisher '%s' found.\n", publisher);
    }
}

int deleteBookByField(BookArray* array, const char* field, const char* value) {
    if (array == NULL || array->size == 0) {
        printf("Book array is empty.\n");
        return 0;
    }

    int deleted = 0;

    for (int i = 0; i < array->size; i++) {
        int match = 0;

        if (strcmp(field, "author") == 0) {
            match = (strstr(array->books[i].author, value) != NULL);
        }
        else if (strcmp(field, "title") == 0) {
            match = (strstr(array->books[i].title, value) != NULL);
        }
        else if (strcmp(field, "publisher") == 0) {
            match = (strcmp(array->books[i].publisher, value) == 0);
        }
        else if (strcmp(field, "year") == 0) {
            int year = atoi(value);
            match = (array->books[i].year == year);
        }

        if (match) {
            printf("Deleted book: ");
            printBook(&array->books[i]);
            removeBook(array, i);
            i--; // Корректировка индекса после удаления
            deleted++;
        }
    }

    printf("Deleted books: %d\n", deleted);
    return deleted;
}

int modifyBookByField(BookArray* array, const char* field, const char* value) {
    if (array == NULL || array->size == 0) {
        printf("Book array is empty.\n");
        return 0;
    }

    int modified = 0;

    for (int i = 0; i < array->size; i++) {
        int match = 0;

        if (strcmp(field, "author") == 0) {
            match = (strstr(array->books[i].author, value) != NULL);
        }
        else if (strcmp(field, "title") == 0) {
            match = (strstr(array->books[i].title, value) != NULL);
        }

        if (match) {
            printf("Found book for modification:\n");
            printBook(&array->books[i]);

            printf("\nEnter new data:\n");
            Book newBook = createBookFromInput(0); // Ручной ввод ID

            char confirm;
            printf("Confirm modification? (y/n): ");
            scanf(" %c", &confirm);
            getchar();

            if (confirm == 'y' || confirm == 'Y') {
                updateBook(array, i, newBook);
                printf("Book modified.\n");
                modified++;
            }
        }
    }

    printf("Modified books: %d\n", modified);
    return modified;
}

void shellSortByYear(BookArray* array) {
    if (array == NULL || array->size < 2) {
        return;
    }

    int n = array->size;

    // Используем последовательность Кнута
    int gap = 1;
    while (gap < n / 3) {
        gap = gap * 3 + 1;
    }

    while (gap > 0) {
        for (int i = gap; i < n; i++) {
            Book temp = array->books[i];
            int j = i;

            while (j >= gap && array->books[j - gap].year > temp.year) {
                array->books[j] = array->books[j - gap];
                j -= gap;
            }

            array->books[j] = temp;
        }
        gap /= 3;
    }

    printf("Array sorted by year (ascending).\n");
}

// Альтернативная сортировка по автору
void shellSortByAuthor(BookArray* array) {
    if (array == NULL || array->size < 2) {
        return;
    }

    int n = array->size;
    int gap = n / 2;

    while (gap > 0) {
        for (int i = gap; i < n; i++) {
            Book temp = array->books[i];
            int j = i;

            while (j >= gap && strcmp(array->books[j - gap].author, temp.author) > 0) {
                array->books[j] = array->books[j - gap];
                j -= gap;
            }

            array->books[j] = temp;
        }
        gap /= 2;
    }

    printf("Array sorted by author (alphabetical order).\n");
}

void printBook(const Book* book) {
    printf("ID: %d, Author: %s, Title: %s, Year: %d, Publisher: %s, Pages: %d\n",
        book->id.numeric_id,
        book->author,
        book->title,
        book->year,
        book->publisher,
        book->pages);
}