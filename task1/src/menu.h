#ifndef MENU_H
#define MENU_H

#include "bookshelf.h"
#include "file_operations.h"

void printMainMenu();
void printFileMenu();
void handleMainMenu(BookArray** array);
void handleFileMenu(BookArray** array);
void clearInputBuffer();

#endif