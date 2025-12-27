#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "bookshelf.h"

int saveBooksToFile(const BookArray* array, const char* filename);
BookArray* loadBooksFromFile(const char* filename);
int updateBookInFile(const char* filename, int bookIndex, const Book* newBook);
int deleteBookFromFile(const char* filename, int bookIndex);
void displayBooksFromFile(const char* filename);

#endif