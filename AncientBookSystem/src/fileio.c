#include "fileio.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FIELD_COUNT 13

static void appendLoadedNode(Book **head, Book *node) {
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

Book *loadFromFile(const char *filename) {
    FILE *fp;
    Book *head = NULL;
    char line[512];
    int lineNo = 0;
    int count = 0;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("未找到数据文件 %s，系统将从空数据开始。\n", filename);
        return NULL;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        char *fields[FIELD_COUNT];
        char *token;
        Book *node;
        int i = 0;

        lineNo++;
        trimNewline(line);

        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }

        token = strtok(line, "|");
        while (token != NULL && i < FIELD_COUNT) {
            fields[i] = token;
            i++;
            token = strtok(NULL, "|");
        }

        if (i != FIELD_COUNT) {
            printf("第 %d 行字段数量不正确，已跳过。\n", lineNo);
            continue;
        }

        node = createBookNode();
        if (node == NULL) {
            printf("内存分配失败，停止继续读取文件。\n");
            break;
        }

        safeStringCopy(node->id, fields[0], sizeof(node->id));
        safeStringCopy(node->title, fields[1], sizeof(node->title));
        safeStringCopy(node->author, fields[2], sizeof(node->author));
        safeStringCopy(node->country, fields[3], sizeof(node->country));
        safeStringCopy(node->category, fields[4], sizeof(node->category));
        safeStringCopy(node->period, fields[5], sizeof(node->period));
        safeStringCopy(node->condition, fields[6], sizeof(node->condition));
        safeStringCopy(node->collectionDate, fields[7], sizeof(node->collectionDate));
        safeStringCopy(node->source, fields[8], sizeof(node->source));
        safeStringCopy(node->shelfCode, fields[9], sizeof(node->shelfCode));
        node->price = atof(fields[10]);
        node->stock = atoi(fields[11]);
        node->sold = atoi(fields[12]);

        if (node->price < 0 || node->stock < 0 || node->sold < 0 ||
            !isValidDate(node->collectionDate)) {
            printf("第 %d 行存在非法价格、库存、销量或日期，已跳过。\n", lineNo);
            free(node);
            continue;
        }

        if (isIdExists(head, node->id)) {
            printf("第 %d 行编号 %s 重复，已跳过。\n", lineNo, node->id);
            free(node);
            continue;
        }

        appendLoadedNode(&head, node);
        count++;
    }

    fclose(fp);
    printf("已从 %s 读取 %d 条古籍记录。\n", filename, count);
    return head;
}

int saveToFile(Book *head, const char *filename) {
    FILE *fp;
    Book *p;
    int count = 0;

    fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("无法打开文件 %s 进行写入。\n", filename);
        return 0;
    }

    p = head;
    while (p != NULL) {
        fprintf(fp, "%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%.2f|%d|%d\n",
                p->id, p->title, p->author, p->country, p->category,
                p->period, p->condition, p->collectionDate, p->source,
                p->shelfCode, p->price, p->stock, p->sold);
        count++;
        p = p->next;
    }

    fclose(fp);
    printf("已保存 %d 条古籍记录到 %s。\n", count, filename);
    return 1;
}

int appendSaleLog(const char *filename, const Book *book, int quantity, double amount) {
    FILE *fp;
    time_t now;
    struct tm *localTime;
    char timeText[32];

    if (book == NULL) {
        return 0;
    }

    fp = fopen(filename, "a");
    if (fp == NULL) {
        printf("提示：销售记录文件 %s 写入失败，但库存数据已更新。\n", filename);
        return 0;
    }

    now = time(NULL);
    localTime = localtime(&now);
    if (localTime != NULL) {
        strftime(timeText, sizeof(timeText), "%Y-%m-%d %H:%M:%S", localTime);
    } else {
        safeStringCopy(timeText, "unknown-time", sizeof(timeText));
    }

    fprintf(fp, "%s|%s|%s|%d|%.2f|%.2f\n",
            timeText, book->id, book->title, quantity, book->price, amount);

    fclose(fp);
    return 1;
}
