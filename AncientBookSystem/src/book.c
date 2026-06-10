#include "book.h"
#include "fileio.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOW_STOCK_LIMIT 3
#define SALES_LOG_FILE "data/sales_log.txt"
#define MAX_GROUP_COUNT 100

typedef struct CountItem {
    char name[60];
    int count;
} CountItem;

Book *createBookNode(void) {
    Book *node = (Book *)malloc(sizeof(Book));
    if (node == NULL) {
        return NULL;
    }

    memset(node, 0, sizeof(Book));
    node->next = NULL;
    return node;
}

void freeList(Book *head) {
    Book *current = head;
    while (current != NULL) {
        Book *next = current->next;
        free(current);
        current = next;
    }
}

int isIdExists(Book *head, const char *id) {
    return findBookById(head, id) != NULL;
}

Book *findBookById(Book *head, const char *id) {
    Book *p = head;

    while (p != NULL) {
        if (strcmp(p->id, id) == 0) {
            return p;
        }
        p = p->next;
    }

    return NULL;
}

static void appendBook(Book **head, Book *node) {
    Book *p;

    if (*head == NULL) {
        *head = node;
        return;
    }

    p = *head;
    while (p->next != NULL) {
        p = p->next;
    }
    p->next = node;
}

static void readDateInput(const char *prompt, char *buffer, int size) {
    while (1) {
        getStringInput(prompt, buffer, size);
        if (isValidDate(buffer)) {
            return;
        }
        printf("日期格式或数值不正确，请按 YYYY-MM-DD 格式重新输入。\n");
    }
}

static double readPositivePrice(const char *prompt) {
    double value;

    while (1) {
        value = getDoubleInput(prompt);
        if (value > 0) {
            return value;
        }
        printf("价格必须大于 0，请重新输入。\n");
    }
}

static int readNonNegativeInt(const char *prompt) {
    int value;

    while (1) {
        value = getIntInput(prompt);
        if (value >= 0) {
            return value;
        }
        printf("数量不能为负数，请重新输入。\n");
    }
}

static int readPositiveInt(const char *prompt) {
    int value;

    while (1) {
        value = getIntInput(prompt);
        if (value > 0) {
            return value;
        }
        printf("数量必须大于 0，请重新输入。\n");
    }
}

void addBook(Book **head) {
    Book *node = createBookNode();

    if (node == NULL) {
        printf("内存分配失败，无法添加古籍。\n");
        return;
    }

    printf("\n========== 添加古籍信息 ==========\n");

    while (1) {
        getStringInput("古籍编号（如 B001）：", node->id, sizeof(node->id));
        if (isIdExists(*head, node->id)) {
            printf("编号 %s 已存在，请重新输入。\n", node->id);
        } else {
            break;
        }
    }

    getStringInput("书名：", node->title, sizeof(node->title));
    getStringInput("作者：", node->author, sizeof(node->author));
    getStringInput("国别/地区：", node->country, sizeof(node->country));
    getStringInput("类别：", node->category, sizeof(node->category));
    getStringInput("年代：", node->period, sizeof(node->period));
    getStringInput("品相：", node->condition, sizeof(node->condition));
    readDateInput("收录日期（YYYY-MM-DD）：", node->collectionDate, sizeof(node->collectionDate));
    getStringInput("来源：", node->source, sizeof(node->source));
    getStringInput("架位号：", node->shelfCode, sizeof(node->shelfCode));
    node->price = readPositivePrice("价格：");
    node->stock = readNonNegativeInt("库存数量：");
    node->sold = readNonNegativeInt("已售数量：");

    appendBook(head, node);
    printf("添加成功，古籍编号：%s。\n", node->id);
}

void displayBook(const Book *book) {
    if (book == NULL) {
        return;
    }

    printf("编号：%s\n", book->id);
    printf("书名：%s\n", book->title);
    printf("作者：%s\n", book->author);
    printf("国别/地区：%s\n", book->country);
    printf("类别：%s\n", book->category);
    printf("年代：%s\n", book->period);
    printf("品相：%s\n", book->condition);
    printf("收录日期：%s\n", book->collectionDate);
    printf("来源：%s\n", book->source);
    printf("架位号：%s\n", book->shelfCode);
    printf("价格：%.2f\n", book->price);
    printf("库存：%d\n", book->stock);
    printf("已售：%d\n", book->sold);
}

static void printTableHeader(void) {
    printLine('-', 150);
    printf("%-8s %-24s %-20s %-12s %-10s %-12s %-10s %-12s %-14s %-10s %10s %6s %6s\n",
           "编号", "书名", "作者", "国别", "类别", "年代", "品相",
           "收录日期", "来源", "架位", "价格", "库存", "已售");
    printLine('-', 150);
}

static void printTableRow(const Book *p) {
    printf("%-8s %-24s %-20s %-12s %-10s %-12s %-10s %-12s %-14s %-10s %10.2f %6d %6d\n",
           p->id, p->title, p->author, p->country, p->category,
           p->period, p->condition, p->collectionDate, p->source,
           p->shelfCode, p->price, p->stock, p->sold);
}

void displayAllBooks(Book *head) {
    Book *p = head;
    int count = 0;

    printf("\n========== 全部古籍信息 ==========\n");
    if (head == NULL) {
        printf("当前没有古籍记录。\n");
        return;
    }

    printTableHeader();
    while (p != NULL) {
        printTableRow(p);
        count++;
        p = p->next;
    }
    printLine('-', 150);
    printf("共显示 %d 条古籍记录。\n", count);
}

static void printSearchResult(Book *head, int (*match)(Book *, const char *), const char *keyword) {
    Book *p = head;
    int count = 0;

    if (head == NULL) {
        printf("当前没有古籍记录。\n");
        return;
    }

    printTableHeader();
    while (p != NULL) {
        if (match(p, keyword)) {
            printTableRow(p);
            count++;
        }
        p = p->next;
    }
    printLine('-', 150);

    if (count == 0) {
        printf("未找到符合条件的古籍记录。\n");
    } else {
        printf("共找到 %d 条符合条件的记录。\n", count);
    }
}

static int matchTitle(Book *book, const char *keyword) {
    return strstr(book->title, keyword) != NULL;
}

static int matchAuthor(Book *book, const char *keyword) {
    return strstr(book->author, keyword) != NULL;
}

static int matchCategory(Book *book, const char *keyword) {
    return strcmp(book->category, keyword) == 0;
}

static int matchCountry(Book *book, const char *keyword) {
    return strcmp(book->country, keyword) == 0;
}

static int matchDate(Book *book, const char *keyword) {
    return strcmp(book->collectionDate, keyword) == 0;
}

static int matchSource(Book *book, const char *keyword) {
    return strstr(book->source, keyword) != NULL;
}

static int matchShelfCode(Book *book, const char *keyword) {
    return strstr(book->shelfCode, keyword) != NULL;
}

void searchById(Book *head) {
    char id[20];
    Book *book;

    printf("\n========== 按编号查询 ==========\n");
    getStringInput("请输入古籍编号：", id, sizeof(id));
    book = findBookById(head, id);
    if (book == NULL) {
        printf("未找到编号为 %s 的古籍。\n", id);
    } else {
        displayBook(book);
    }
}

void searchByTitle(Book *head) {
    char keyword[80];
    printf("\n========== 按书名关键词查询 ==========\n");
    getStringInput("请输入书名关键词：", keyword, sizeof(keyword));
    printSearchResult(head, matchTitle, keyword);
}

void searchByAuthor(Book *head) {
    char keyword[50];
    printf("\n========== 按作者查询 ==========\n");
    getStringInput("请输入作者关键词：", keyword, sizeof(keyword));
    printSearchResult(head, matchAuthor, keyword);
}

void searchByCategory(Book *head) {
    char keyword[30];
    printf("\n========== 按类别查询 ==========\n");
    getStringInput("请输入类别：", keyword, sizeof(keyword));
    printSearchResult(head, matchCategory, keyword);
}

void searchByCountry(Book *head) {
    char keyword[30];
    printf("\n========== 按国别/地区查询 ==========\n");
    getStringInput("请输入国别/地区：", keyword, sizeof(keyword));
    printSearchResult(head, matchCountry, keyword);
}

void searchByDate(Book *head) {
    char date[20];
    printf("\n========== 按收录日期查询 ==========\n");
    readDateInput("请输入收录日期（YYYY-MM-DD）：", date, sizeof(date));
    printSearchResult(head, matchDate, date);
}

void searchBySource(Book *head) {
    char keyword[50];
    printf("\n========== 按来源查询 ==========\n");
    getStringInput("请输入来源关键词：", keyword, sizeof(keyword));
    printSearchResult(head, matchSource, keyword);
}

void searchByShelfCode(Book *head) {
    char keyword[30];
    printf("\n========== 按架位号查询 ==========\n");
    getStringInput("请输入架位号关键词：", keyword, sizeof(keyword));
    printSearchResult(head, matchShelfCode, keyword);
}

static void printSearchMenu(void) {
    printf("\n========== 查询古籍信息 ==========\n");
    printf("1. 按编号精确查询\n");
    printf("2. 按书名关键词查询\n");
    printf("3. 按作者查询\n");
    printf("4. 按类别查询\n");
    printf("5. 按国别/地区查询\n");
    printf("6. 按收录日期查询\n");
    printf("7. 按来源查询\n");
    printf("8. 按架位号查询\n");
    printf("0. 返回主菜单\n");
}

void searchBookMenu(Book *head) {
    int choice;

    do {
        printSearchMenu();
        choice = getIntInRange("请选择查询方式：", 0, 8);

        switch (choice) {
            case 1:
                searchById(head);
                break;
            case 2:
                searchByTitle(head);
                break;
            case 3:
                searchByAuthor(head);
                break;
            case 4:
                searchByCategory(head);
                break;
            case 5:
                searchByCountry(head);
                break;
            case 6:
                searchByDate(head);
                break;
            case 7:
                searchBySource(head);
                break;
            case 8:
                searchByShelfCode(head);
                break;
            default:
                break;
        }
    } while (choice != 0);
}

static void printModifyMenu(void) {
    printf("\n可修改字段：\n");
    printf("1. 书名\n");
    printf("2. 作者\n");
    printf("3. 国别/地区\n");
    printf("4. 类别\n");
    printf("5. 年代\n");
    printf("6. 品相\n");
    printf("7. 收录日期\n");
    printf("8. 来源\n");
    printf("9. 架位号\n");
    printf("10. 价格\n");
    printf("11. 库存数量\n");
    printf("12. 已售数量\n");
    printf("0. 完成修改\n");
}

void modifyBook(Book *head) {
    char id[20];
    Book *book;
    int choice;

    printf("\n========== 修改古籍信息 ==========\n");
    if (head == NULL) {
        printf("当前没有古籍记录。\n");
        return;
    }

    getStringInput("请输入需要修改的古籍编号：", id, sizeof(id));
    book = findBookById(head, id);
    if (book == NULL) {
        printf("未找到编号为 %s 的古籍。\n", id);
        return;
    }

    printf("\n修改前信息：\n");
    displayBook(book);

    do {
        printModifyMenu();
        choice = getIntInRange("请选择要修改的字段：", 0, 12);

        switch (choice) {
            case 1:
                getStringInput("新的书名：", book->title, sizeof(book->title));
                break;
            case 2:
                getStringInput("新的作者：", book->author, sizeof(book->author));
                break;
            case 3:
                getStringInput("新的国别/地区：", book->country, sizeof(book->country));
                break;
            case 4:
                getStringInput("新的类别：", book->category, sizeof(book->category));
                break;
            case 5:
                getStringInput("新的年代：", book->period, sizeof(book->period));
                break;
            case 6:
                getStringInput("新的品相：", book->condition, sizeof(book->condition));
                break;
            case 7:
                readDateInput("新的收录日期（YYYY-MM-DD）：", book->collectionDate, sizeof(book->collectionDate));
                break;
            case 8:
                getStringInput("新的来源：", book->source, sizeof(book->source));
                break;
            case 9:
                getStringInput("新的架位号：", book->shelfCode, sizeof(book->shelfCode));
                break;
            case 10:
                book->price = readPositivePrice("新的价格：");
                break;
            case 11:
                book->stock = readNonNegativeInt("新的库存数量：");
                break;
            case 12:
                book->sold = readNonNegativeInt("新的已售数量：");
                break;
            default:
                break;
        }

        if (choice != 0) {
            printf("字段修改完成。\n");
        }
    } while (choice != 0);

    printf("\n修改后信息：\n");
    displayBook(book);
}

void deleteBook(Book **head) {
    char id[20];
    Book *current;
    Book *previous = NULL;

    printf("\n========== 删除古籍信息 ==========\n");
    if (head == NULL || *head == NULL) {
        printf("当前没有古籍记录。\n");
        return;
    }

    getStringInput("请输入需要删除的古籍编号：", id, sizeof(id));

    current = *head;
    while (current != NULL && strcmp(current->id, id) != 0) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("未找到编号为 %s 的古籍。\n", id);
        return;
    }

    printf("\n即将删除以下古籍：\n");
    displayBook(current);

    if (!askYesNo("确认删除？(y/n)：")) {
        printf("已取消删除。\n");
        return;
    }

    if (previous == NULL) {
        *head = current->next;
    } else {
        previous->next = current->next;
    }

    free(current);
    printf("删除成功。\n");
}

void sellBook(Book *head) {
    char id[20];
    int quantity;
    double amount;
    Book *book;

    printf("\n========== 销售管理 ==========\n");
    if (head == NULL) {
        printf("当前没有古籍记录。\n");
        return;
    }

    getStringInput("请输入销售古籍编号：", id, sizeof(id));
    book = findBookById(head, id);
    if (book == NULL) {
        printf("未找到编号为 %s 的古籍。\n", id);
        return;
    }

    displayBook(book);
    quantity = readPositiveInt("请输入销售数量：");

    if (quantity > book->stock) {
        printf("销售失败：库存不足。当前库存为 %d。\n", book->stock);
        return;
    }

    book->stock -= quantity;
    book->sold += quantity;
    amount = book->price * quantity;

    printf("销售成功，本次销售金额：%.2f。\n", amount);
    printf("更新后库存：%d，累计已售：%d。\n", book->stock, book->sold);
    appendSaleLog(SALES_LOG_FILE, book, quantity, amount);
}

void purchaseBook(Book *head) {
    char id[20];
    int quantity;
    Book *book;

    printf("\n========== 进货管理 ==========\n");
    if (head == NULL) {
        printf("当前没有古籍记录。\n");
        return;
    }

    getStringInput("请输入进货古籍编号：", id, sizeof(id));
    book = findBookById(head, id);
    if (book == NULL) {
        printf("未找到编号为 %s 的古籍。\n", id);
        return;
    }

    quantity = readPositiveInt("请输入进货数量：");
    book->stock += quantity;
    printf("进货成功，%s 当前库存为 %d。\n", book->title, book->stock);
}

static int compareBook(const Book *a, const Book *b, int field) {
    if (field == 1) {
        if (a->price > b->price) {
            return 1;
        }
        if (a->price < b->price) {
            return -1;
        }
        return strcmp(a->id, b->id);
    }

    if (field == 2) {
        if (a->stock > b->stock) {
            return 1;
        }
        if (a->stock < b->stock) {
            return -1;
        }
        return strcmp(a->id, b->id);
    }

    if (field == 3) {
        if (a->sold > b->sold) {
            return 1;
        }
        if (a->sold < b->sold) {
            return -1;
        }
        return strcmp(a->id, b->id);
    }

    return strcmp(a->id, b->id);
}

static void swapBookData(Book *a, Book *b) {
    Book temp;
    Book *nextA;
    Book *nextB;

    temp = *a;
    nextA = a->next;
    nextB = b->next;

    *a = *b;
    *b = temp;

    a->next = nextA;
    b->next = nextB;
}

void sortBooks(Book **head) {
    int field;
    int order;
    int swapped;

    printf("\n========== 排序显示 ==========\n");
    if (head == NULL || *head == NULL) {
        printf("当前没有古籍记录。\n");
        return;
    }

    printf("排序字段：\n");
    printf("1. 价格\n");
    printf("2. 库存\n");
    printf("3. 销量\n");
    printf("4. 编号\n");
    field = getIntInRange("请选择排序字段：", 1, 4);

    printf("排序方式：\n");
    printf("1. 升序\n");
    printf("2. 降序\n");
    order = getIntInRange("请选择排序方式：", 1, 2);

    do {
        Book *p = *head;
        swapped = 0;

        while (p != NULL && p->next != NULL) {
            int cmp = compareBook(p, p->next, field);
            if ((order == 1 && cmp > 0) || (order == 2 && cmp < 0)) {
                swapBookData(p, p->next);
                swapped = 1;
            }
            p = p->next;
        }
    } while (swapped);

    printf("排序完成，结果如下：\n");
    displayAllBooks(*head);
}

static void addCountItem(CountItem items[], int *size, const char *name) {
    int i;

    for (i = 0; i < *size; i++) {
        if (strcmp(items[i].name, name) == 0) {
            items[i].count++;
            return;
        }
    }

    if (*size < MAX_GROUP_COUNT) {
        safeStringCopy(items[*size].name, name, sizeof(items[*size].name));
        items[*size].count = 1;
        (*size)++;
    }
}

static void printCountItems(const char *title, CountItem items[], int size) {
    int i;

    printf("\n%s\n", title);
    printLine('-', 36);
    printf("%-20s %8s\n", "项目", "数量");
    printLine('-', 36);
    for (i = 0; i < size; i++) {
        printf("%-20s %8d\n", items[i].name, items[i].count);
    }
    printLine('-', 36);
}

void statistics(Book *head) {
    Book *p = head;
    int bookCount = 0;
    int totalStock = 0;
    int totalSold = 0;
    double stockValue = 0.0;
    double estimatedSales = 0.0;
    Book *maxSold = NULL;
    Book *minStock = NULL;
    CountItem sourceItems[MAX_GROUP_COUNT];
    CountItem categoryItems[MAX_GROUP_COUNT];
    int sourceSize = 0;
    int categorySize = 0;

    printf("\n========== 综合统计 ==========\n");
    if (head == NULL) {
        printf("当前没有古籍记录。\n");
        return;
    }

    while (p != NULL) {
        bookCount++;
        totalStock += p->stock;
        totalSold += p->sold;
        stockValue += p->price * p->stock;
        estimatedSales += p->price * p->sold;

        if (maxSold == NULL || p->sold > maxSold->sold) {
            maxSold = p;
        }
        if (minStock == NULL || p->stock < minStock->stock) {
            minStock = p;
        }

        addCountItem(sourceItems, &sourceSize, p->source);
        addCountItem(categoryItems, &categorySize, p->category);
        p = p->next;
    }

    printf("古籍种类总数：%d\n", bookCount);
    printf("总库存数量：%d\n", totalStock);
    printf("总库存估值：%.2f\n", stockValue);
    printf("总销量：%d\n", totalSold);
    printf("估算销售总额：%.2f\n", estimatedSales);

    if (maxSold != NULL) {
        printf("销量最高的古籍：%s（%s），已售 %d 本。\n",
               maxSold->title, maxSold->id, maxSold->sold);
    }
    if (minStock != NULL) {
        printf("库存最低的古籍：%s（%s），库存 %d 本。\n",
               minStock->title, minStock->id, minStock->stock);
    }

    printCountItems("按来源统计古籍数量", sourceItems, sourceSize);
    printCountItems("按类别统计古籍数量", categoryItems, categorySize);
}

void lowStockWarning(Book *head) {
    Book *p = head;
    int count = 0;

    printf("\n========== 低库存预警 ==========\n");
    if (head == NULL) {
        printf("当前没有古籍记录。\n");
        return;
    }

    printTableHeader();
    while (p != NULL) {
        if (p->stock <= LOW_STOCK_LIMIT) {
            printTableRow(p);
            count++;
        }
        p = p->next;
    }
    printLine('-', 150);

    if (count == 0) {
        printf("没有库存小于等于 %d 的古籍。\n", LOW_STOCK_LIMIT);
    } else {
        printf("共有 %d 种古籍库存小于等于 %d，请关注补货。\n", count, LOW_STOCK_LIMIT);
    }
}

void showRecentCollections(Book *head) {
    char date[20];
    Book *p = head;
    int count = 0;

    printf("\n========== 新收录古籍查询 ==========\n");
    if (head == NULL) {
        printf("当前没有古籍记录。\n");
        return;
    }

    readDateInput("请输入起始日期（显示该日期之后收录的古籍，YYYY-MM-DD）：", date, sizeof(date));

    printTableHeader();
    while (p != NULL) {
        if (compareDate(p->collectionDate, date) > 0) {
            printTableRow(p);
            count++;
        }
        p = p->next;
    }
    printLine('-', 150);

    if (count == 0) {
        printf("该日期之后没有新收录古籍。\n");
    } else {
        printf("共找到 %d 条新收录古籍记录。\n", count);
    }
}

static void printStatisticsMenu(void) {
    printf("\n========== 统计分析 ==========\n");
    printf("1. 综合统计\n");
    printf("2. 低库存预警\n");
    printf("3. 显示某日期之后新收录的古籍\n");
    printf("0. 返回主菜单\n");
}

void statisticsMenu(Book *head) {
    int choice;

    do {
        printStatisticsMenu();
        choice = getIntInRange("请选择统计功能：", 0, 3);

        switch (choice) {
            case 1:
                statistics(head);
                break;
            case 2:
                lowStockWarning(head);
                break;
            case 3:
                showRecentCollections(head);
                break;
            default:
                break;
        }
    } while (choice != 0);
}
