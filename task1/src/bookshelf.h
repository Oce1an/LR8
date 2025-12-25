#ifndef BOOK_H
#define BOOK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Объединение для разных типов идентификаторов книги
typedef union {
    int numeric_id;     // Числовой идентификатор
    char string_id[20]; // Строковый идентификатор (например, ISBN)
} BookID;

// Структура для хранения информации о книге
typedef struct {
    BookID id;           // Регистрационный номер (union)
    char author[100];    // Автор
    char title[100];     // Название
    int year;           // Год издания
    char publisher[50]; // Издательство
    int pages;          // Количество страниц
} Book;

// Структура для динамического массива книг
typedef struct {
    Book* books;        // Указатель на массив структур
    int size;          // Текущее количество книг
    int capacity;      // Вместимость массива
} BookArray;

// Функции работы с массивом книг
BookArray* createBookArray(int initialCapacity);
void freeBookArray(BookArray* array);
int addBook(BookArray* array, Book book);
int insertBook(BookArray* array, Book book, int index);
int removeBook(BookArray* array, int index);
int updateBook(BookArray* array, int index, Book book);

// Основные функции согласно заданию
BookArray* inputBooksFixedCount(int count);
BookArray* inputBooksUntilCondition();
BookArray* inputBooksInteractive();
void displayBooks(const BookArray* array);
void addBooksToExisting(BookArray* existingArray);
void findBooksByYear(const BookArray* array, int targetYear);
void findBooksByAuthor(const BookArray* array, const char* author);
void findBooksByPublisher(const BookArray* array, const char* publisher);
int deleteBookByField(BookArray* array, const char* field, const char* value);
int modifyBookByField(BookArray* array, const char* field, const char* value);
void shellSortByYear(BookArray* array);
void shellSortByAuthor(BookArray* array);

// Вспомогательные функции
void printBook(const Book* book);
Book createBookFromInput(int autoGenerateID);

#endif // BOOK_H