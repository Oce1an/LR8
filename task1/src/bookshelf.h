#ifndef BOOK_H
#define BOOK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef union {
    int numeric_id;
    char string_id[20];
} BookID;

typedef struct {
    BookID id;
    char author[100];
    char title[100];
    int year;
    char publisher[50];
    int pages;
} Book;

typedef struct {
    Book* books;
    int size;
    int capacity;
} BookArray;

BookArray* createBookArray(int initialCapacity);
void freeBookArray(BookArray* array);
int addBook(BookArray* array, Book book);
int insertBook(BookArray* array, Book book, int index);
int removeBook(BookArray* array, int index);
int updateBook(BookArray* array, int index, Book book);

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

void printBook(const Book* book);
Book createBookFromInput(int autoGenerateID);

#endif