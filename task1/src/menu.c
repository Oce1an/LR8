#include "menu.h"

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void printMainMenu() {
    printf("\n========== BOOK LIBRARY ==========\n");
    printf("1. Input books (different methods)\n");
    printf("2. View all books\n");
    printf("3. Add books to existing array\n");
    printf("4. Search books\n");
    printf("5. Delete books\n");
    printf("6. Modify books\n");
    printf("7. Sort books\n");
    printf("8. File operations\n");
    printf("9. Special task: books published after given year\n");
    printf("0. Exit\n");
    printf("=====================================\n");
    printf("Select action: ");
}

void printFileMenu() {
    printf("\n========== FILE OPERATIONS ==========\n");
    printf("1. Save books to file\n");
    printf("2. Load books from file\n");
    printf("3. Update book in file\n");
    printf("4. View books from file\n");
    printf("5. Delete book from file\n");
    printf("0. Back to main menu\n");
    printf("======================================\n");
    printf("Select action: ");
}

int showBookInputMenu() {
    printf("\n=== BOOK INPUT METHODS ===\n");
    printf("1. Input fixed number of books\n");
    printf("2. Input until condition\n");
    printf("3. Interactive input\n");
    printf("0. Back to main menu\n");
    printf("Select action: ");

    int choice;
    scanf("%d", &choice);
    clearInputBuffer();

    return choice;
}

void handleMainMenu(BookArray** array) {
    int choice;
    char buffer[100];
    int year;

    do {
        printMainMenu();
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
        case 1:
        {
            int inputMethod;
            do {
                inputMethod = showBookInputMenu();

                switch (inputMethod) {
                case 0: // Возврат в главное меню
                    printf("Returning to main menu...\n");
                    break;

                case 1: // Фиксированное количество
                    printf("How many books to input? (0 to cancel): ");
                    int count;
                    scanf("%d", &count);
                    clearInputBuffer();

                    if (count == 0) {
                        printf("Input cancelled.\n");
                        break;
                    }

                    if (count > 0) {
                        if (*array != NULL) {
                            freeBookArray(*array);
                        }
                        *array = inputBooksFixedCount(count);
                    }
                    else {
                        printf("Invalid number!\n");
                    }
                    break;

                case 2: // До условия
                    printf("This method will stop when you enter 'STOP' as book title.\n");
                    printf("Continue? (y/n): ");
                    char confirm;
                    scanf(" %c", &confirm);
                    clearInputBuffer();

                    if (confirm == 'y' || confirm == 'Y') {
                        if (*array != NULL) {
                            freeBookArray(*array);
                        }
                        *array = inputBooksUntilCondition();
                    }
                    else {
                        printf("Cancelled.\n");
                    }
                    break;

                case 3: // Интерактивный ввод
                    printf("Interactive input - you can add books one by one.\n");
                    printf("Continue? (y/n): ");
                    scanf(" %c", &confirm);
                    clearInputBuffer();

                    if (confirm == 'y' || confirm == 'Y') {
                        if (*array != NULL) {
                            freeBookArray(*array);
                        }
                        *array = inputBooksInteractive();
                    }
                    else {
                        printf("Cancelled.\n");
                    }
                    break;

                default:
                    printf("Invalid choice! Please try again.\n");
                }

            } while (inputMethod != 0);
        }
        break;

        case 2: // Просмотр книг
            if (*array != NULL) {
                displayBooks(*array);
            }
            else {
                printf("Book array not created!\n");
            }
            break;

        case 3: // Дополнение массива
            if (*array != NULL) {
                printf("\n=== ADDING TO EXISTING ARRAY ===\n");
                printf("Current book count: %d\n", (*array)->size);
                printf("Enter 0 to cancel\n");

                addBooksToExisting(*array);
            }
            else {
                printf("First create book array!\n");
            }
            break;

        case 4: // Поиск книг
            if (*array != NULL) {
                int searchChoice;
                do {
                    printf("\n=== BOOK SEARCH ===\n");
                    printf("1. By author\n");
                    printf("2. By year\n");
                    printf("3. By publisher\n");
                    printf("0. Back to main menu\n");
                    printf("Select search criteria (0 to return): ");

                    scanf("%d", &searchChoice);
                    clearInputBuffer();

                    switch (searchChoice) {
                    case 0:
                        printf("Returning to main menu...\n");
                        break;
                    case 1:
                        printf("Enter author surname: ");
                        fgets(buffer, 100, stdin);
                        buffer[strcspn(buffer, "\n")] = '\0';
                        findBooksByAuthor(*array, buffer);
                        break;
                    case 2:
                        printf("Enter publication year: ");
                        scanf("%d", &year);
                        clearInputBuffer();
                        findBooksByYear(*array, year);
                        break;
                    case 3:
                        printf("Enter publisher: ");
                        fgets(buffer, 100, stdin);
                        buffer[strcspn(buffer, "\n")] = '\0';
                        findBooksByPublisher(*array, buffer);
                        break;
                    default:
                        printf("Invalid choice!\n");
                    }
                } while (searchChoice != 0);
            }
            else {
                printf("Book array not created!\n");
            }
            break;

        case 5: // Удаление книг
            if (*array != NULL) {
                printf("\n=== BOOK DELETION ===\n");
                printf("Enter 0 in field name to cancel\n");

                printf("Delete by (author/title/publisher/year, 0 to cancel): ");
                fgets(buffer, 100, stdin);
                buffer[strcspn(buffer, "\n")] = '\0';

                if (strcmp(buffer, "0") == 0) {
                    printf("Deletion cancelled.\n");
                    break;
                }

                printf("Enter search value: ");
                char value[100];
                fgets(value, 100, stdin);
                value[strcspn(value, "\n")] = '\0';

                deleteBookByField(*array, buffer, value);
            }
            else {
                printf("Book array not created!\n");
            }
            break;

        case 6: // Изменение книг
            if (*array != NULL) {
                printf("\n=== BOOK MODIFICATION ===\n");
                printf("Enter 0 in field name to cancel\n");

                printf("Find books for modification by (author/title, 0 to cancel): ");
                fgets(buffer, 100, stdin);
                buffer[strcspn(buffer, "\n")] = '\0';

                if (strcmp(buffer, "0") == 0) {
                    printf("Modification cancelled.\n");
                    break;
                }

                printf("Enter search value: ");
                char value[100];
                fgets(value, 100, stdin);
                value[strcspn(value, "\n")] = '\0';

                modifyBookByField(*array, buffer, value);
            }
            else {
                printf("Book array not created!\n");
            }
            break;

        case 7: // Сортировка
            if (*array != NULL) {
                int sortChoice;
                do {
                    printf("\n=== BOOK SORTING ===\n");
                    printf("1. By year (Shell sort)\n");
                    printf("2. By author (alphabetical)\n");
                    printf("0. Back to main menu\n");
                    printf("Select sorting method (0 to return): ");

                    scanf("%d", &sortChoice);
                    clearInputBuffer();

                    switch (sortChoice) {
                    case 0:
                        printf("Returning to main menu...\n");
                        break;
                    case 1:
                        shellSortByYear(*array);
                        break;
                    case 2:
                        shellSortByAuthor(*array);
                        break;
                    default:
                        printf("Invalid choice!\n");
                    }
                } while (sortChoice != 0);
            }
            else {
                printf("Book array not created!\n");
            }
            break;

        case 8: // Работа с файлами
            handleFileMenu(array);
            break;

        case 9: // Специальная задача
            if (*array != NULL) {
                printf("Enter year (0 to cancel): ");
                scanf("%d", &year);
                clearInputBuffer();

                if (year == 0) {
                    printf("Operation cancelled.\n");
                    break;
                }

                // Сортируем по году
                shellSortByYear(*array);

                // Выводим книги, изданные после заданного года
                printf("\n=== BOOKS PUBLISHED AFTER %d ===\n", year);
                printf("(sorted by publication year)\n");

                int found = 0;
                for (int i = 0; i < (*array)->size; i++) {
                    if ((*array)->books[i].year > year) {
                        printf("Author: %-20s | Title: %-30s | Year: %d\n",
                            (*array)->books[i].author,
                            (*array)->books[i].title,
                            (*array)->books[i].year);
                        found = 1;
                    }
                }

                if (!found) {
                    printf("No books published after %d found.\n", year);
                }
            }
            else {
                printf("Book array not created!\n");
            }
            break;

        case 0: // Выход
            printf("Exiting program...\n");
            break;

        default:
            printf("Invalid choice! Try again.\n");
        }

    } while (choice != 0);
}

void handleFileMenu(BookArray** array) {
    int choice;
    char filename[100];

    do {
        printFileMenu();
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
        case 1: // Сохранение в файл
            if (*array != NULL) {
                printf("Enter filename (0 to cancel): ");
                fgets(filename, 100, stdin);
                filename[strcspn(filename, "\n")] = '\0';

                if (strcmp(filename, "0") == 0) {
                    printf("Save cancelled.\n");
                    break;
                }

                saveBooksToFile(*array, filename);
            }
            else {
                printf("No data to save!\n");
            }
            break;

        case 2: // Загрузка из файла
            printf("Enter filename (0 to cancel): ");
            fgets(filename, 100, stdin);
            filename[strcspn(filename, "\n")] = '\0';

            if (strcmp(filename, "0") == 0) {
                printf("Load cancelled.\n");
                break;
            }

            if (*array != NULL) {
                char confirm;
                printf("Current data will be lost. Continue? (y/n): ");
                scanf(" %c", &confirm);
                clearInputBuffer();

                if (confirm == 'y' || confirm == 'Y') {
                    freeBookArray(*array);
                    *array = loadBooksFromFile(filename);
                }
                else {
                    printf("Load cancelled.\n");
                }
            }
            else {
                *array = loadBooksFromFile(filename);
            }
            break;

        case 3: // Обновление в файле
            printf("Enter filename (0 to cancel): ");
            fgets(filename, 100, stdin);
            filename[strcspn(filename, "\n")] = '\0';

            if (strcmp(filename, "0") == 0) {
                printf("Update cancelled.\n");
                break;
            }

            if (*array != NULL) {
                int index;
                printf("Enter book index to update (-1 to cancel): ");
                scanf("%d", &index);
                clearInputBuffer();

                if (index == -1) {
                    printf("Update cancelled.\n");
                    break;
                }

                if (index >= 0 && index < (*array)->size) {
                    updateBookInFile(filename, index, &(*array)->books[index]);
                }
                else {
                    printf("Invalid index!\n");
                }
            }
            else {
                printf("First load or create book array!\n");
            }
            break;

        case 4: // Просмотр из файла
            printf("Enter filename (0 to cancel): ");
            fgets(filename, 100, stdin);
            filename[strcspn(filename, "\n")] = '\0';

            if (strcmp(filename, "0") == 0) {
                printf("View cancelled.\n");
                break;
            }

            displayBooksFromFile(filename);
            break;

        case 5: // Удаление из файла
            printf("Enter filename (0 to cancel): ");
            fgets(filename, 100, stdin);
            filename[strcspn(filename, "\n")] = '\0';

            if (strcmp(filename, "0") == 0) {
                printf("Delete cancelled.\n");
                break;
            }

            int index;
            printf("Enter book index to delete (-1 to cancel): ");
            scanf("%d", &index);
            clearInputBuffer();

            if (index == -1) {
                printf("Delete cancelled.\n");
                break;
            }

            deleteBookFromFile(filename, index);
            break;

        case 0: // Назад
            printf("Returning to main menu...\n");
            break;

        default:
            printf("Invalid choice!\n");
        }

    } while (choice != 0);
}