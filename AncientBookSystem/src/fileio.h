#ifndef FILEIO_H
#define FILEIO_H

#include "book.h"

Book *loadFromFile(const char *filename);
int saveToFile(Book *head, const char *filename);
int appendSaleLog(const char *filename, const Book *book, int quantity, double amount);

#endif
