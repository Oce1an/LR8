#include "file_operations.h"

int saveBooksToFile(const BookArray* array, const char* filename) {
    if (array == NULL || filename == NULL) {
        return 0;
    }

    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return 0;
    }

    fwrite(&array->size, sizeof(int), 1, file);

    for (int i = 0; i < array->size; i++) {
        fwrite(&array->books[i], sizeof(Book), 1, file);
    }

    fclose(file);
    printf("Books successfully saved to file '%s' (%d records)\n",
        filename, array->size);
    return 1;
}

BookArray* loadBooksFromFile(const char* filename) {
    if (filename == NULL) {
        return NULL;
    }

    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("File '%s' not found or error opening!\n", filename);
        return NULL;
    }

    int bookCount;
    if (fread(&bookCount, sizeof(int), 1, file) != 1) {
        printf("Error reading book count!\n");
        fclose(file);
        return NULL;
    }

    BookArray* array = createBookArray(bookCount);
    if (array == NULL) {
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < bookCount; i++) {
        Book book;
        if (fread(&book, sizeof(Book), 1, file) != 1) {
            printf("Error reading book %d!\n", i + 1);
            freeBookArray(array);
            fclose(file);
            return NULL;
        }
        addBook(array, book);
    }

    fclose(file);
    printf("Loaded %d books from file '%s'\n", bookCount, filename);
    return array;
}

int updateBookInFile(const char* filename, int bookIndex, const Book* newBook) {
    if (filename == NULL || newBook == NULL) {
        return 0;
    }

    FILE* file = fopen(filename, "r+b");
    if (file == NULL) {
        printf("Error opening file for update!\n");
        return 0;
    }

    int bookCount;
    if (fread(&bookCount, sizeof(int), 1, file) != 1) {
        printf("Error reading book count!\n");
        fclose(file);
        return 0;
    }

    if (bookIndex < 0 || bookIndex >= bookCount) {
        printf("Invalid book index! Valid range: 0-%d\n",
            bookCount - 1);
        fclose(file);
        return 0;
    }

    long offset = sizeof(int) + bookIndex * sizeof(Book);
    if (fseek(file, offset, SEEK_SET) != 0) {
        printf("Error positioning in file!\n");
        fclose(file);
        return 0;
    }

    if (fwrite(newBook, sizeof(Book), 1, file) != 1) {
        printf("Error writing book!\n");
        fclose(file);
        return 0;
    }

    fclose(file);
    printf("Book with index %d successfully updated in file.\n", bookIndex);
    return 1;
}

int deleteBookFromFile(const char* filename, int bookIndex) {
    BookArray* array = loadBooksFromFile(filename);
    if (array == NULL) {
        return 0;
    }

    if (!removeBook(array, bookIndex)) {
        freeBookArray(array);
        return 0;
    }

    int result = saveBooksToFile(array, filename);
    freeBookArray(array);

    return result;
}

void displayBooksFromFile(const char* filename) {
    BookArray* array = loadBooksFromFile(filename);
    if (array != NULL) {
        displayBooks(array);
        freeBookArray(array);
    }
}