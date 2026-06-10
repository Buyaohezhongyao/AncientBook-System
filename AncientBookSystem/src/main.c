#include "book.h"
#include "fileio.h"
#include "utils.h"

#include <stdio.h>

#define DATA_FILE "data/books.txt"

void printMenu(void) {
    printf("\n");
    printLine('=', 48);
    printf("      中外古籍二手书管理系统\n");
    printLine('=', 48);
    printf("1. 添加古籍信息\n");
    printf("2. 浏览全部古籍\n");
    printf("3. 查询古籍信息\n");
    printf("4. 修改古籍信息\n");
    printf("5. 删除古籍信息\n");
    printf("6. 销售管理\n");
    printf("7. 进货管理\n");
    printf("8. 排序显示\n");
    printf("9. 统计分析\n");
    printf("10. 手动保存\n");
    printf("0. 退出系统\n");
    printLine('=', 48);
}

int main(void) {
    Book *head;
    int choice;
    int running = 1;

    printf("欢迎使用中外古籍二手书管理系统。\n");
    head = loadFromFile(DATA_FILE);

    while (running) {
        printMenu();
        choice = getIntInRange("请输入菜单选项：", 0, 10);

        switch (choice) {
            case 1:
                addBook(&head);
                break;
            case 2:
                displayAllBooks(head);
                break;
            case 3:
                searchBookMenu(head);
                break;
            case 4:
                modifyBook(head);
                break;
            case 5:
                deleteBook(&head);
                break;
            case 6:
                sellBook(head);
                break;
            case 7:
                purchaseBook(head);
                break;
            case 8:
                sortBooks(&head);
                break;
            case 9:
                statisticsMenu(head);
                break;
            case 10:
                saveToFile(head, DATA_FILE);
                break;
            case 0:
                if (askYesNo("退出前是否保存数据？(y/n)：")) {
                    saveToFile(head, DATA_FILE);
                }
                printf("感谢使用，再见！\n");
                running = 0;
                break;
            default:
                break;
        }
    }

    freeList(head);
    return 0;
}
