#ifndef BOOK_H
#define BOOK_H

typedef struct Book {
    char id[20];
    char title[80];
    char author[50];
    char country[30];
    char category[30];
    char period[30];
    char condition[30];
    char collectionDate[20];
    char source[50];
    char shelfCode[30];
    double price;
    int stock;
    int sold;
    struct Book *next;
} Book;

Book *createBookNode(void);
void freeList(Book *head);

int isIdExists(Book *head, const char *id);
void addBook(Book **head);
void displayBook(const Book *book);
void displayAllBooks(Book *head);
Book *findBookById(Book *head, const char *id);

void searchBookMenu(Book *head);
void searchById(Book *head);
void searchByTitle(Book *head);
void searchByAuthor(Book *head);
void searchByCategory(Book *head);
void searchByCountry(Book *head);
void searchByDate(Book *head);
void searchBySource(Book *head);
void searchByShelfCode(Book *head);

void modifyBook(Book *head);
void deleteBook(Book **head);

void sellBook(Book *head);
void purchaseBook(Book *head);

void sortBooks(Book **head);
void statisticsMenu(Book *head);
void statistics(Book *head);
void lowStockWarning(Book *head);
void showRecentCollections(Book *head);

#endif
