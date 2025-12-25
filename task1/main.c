#include "src/menu.h"

int main() {
    printf("========================================\n");
    printf("  BOOK LIBRARY MANAGEMENT PROGRAM\n");
    printf("========================================\n");

    BookArray* library = NULL;

    handleMainMenu(&library);

    if (library != NULL) {
        freeBookArray(library);
    }

    printf("Program completed.\n");
    return 0;
}