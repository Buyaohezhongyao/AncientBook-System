#include "utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clearInputBuffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

void trimNewline(char *str) {
    size_t len;

    if (str == NULL) {
        return;
    }

    len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
        len--;
    }
}

static void trimSpaces(char *str) {
    char *start;
    char *end;

    if (str == NULL || str[0] == '\0') {
        return;
    }

    start = str;
    while (*start != '\0' && isspace((unsigned char)*start)) {
        start++;
    }
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }

    if (str[0] == '\0') {
        return;
    }

    end = str + strlen(str) - 1;
    while (end >= str && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }
}

void safeStringCopy(char *dest, const char *src, int size) {
    if (dest == NULL || size <= 0) {
        return;
    }

    if (src == NULL) {
        dest[0] = '\0';
        return;
    }

    strncpy(dest, src, (size_t)size - 1);
    dest[size - 1] = '\0';
}

void getStringInput(const char *prompt, char *buffer, int size) {
    while (1) {
        if (prompt != NULL) {
            printf("%s", prompt);
        }

        if (fgets(buffer, size, stdin) == NULL) {
            buffer[0] = '\0';
            return;
        }

        if (strchr(buffer, '\n') == NULL) {
            clearInputBuffer();
        }

        trimNewline(buffer);
        trimSpaces(buffer);

        if (strlen(buffer) == 0) {
            printf("输入不能为空，请重新输入。\n");
            continue;
        }

        if (strchr(buffer, '|') != NULL) {
            printf("输入内容不能包含竖线字符 | ，请重新输入。\n");
            continue;
        }

        return;
    }
}

int getIntInput(const char *prompt) {
    char line[128];
    char *endPtr;
    long value;

    while (1) {
        if (prompt != NULL) {
            printf("%s", prompt);
        }

        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n未读取到有效输入，默认返回 0。\n");
            return 0;
        }

        if (strchr(line, '\n') == NULL) {
            clearInputBuffer();
        }

        trimNewline(line);
        trimSpaces(line);

        value = strtol(line, &endPtr, 10);
        while (*endPtr != '\0' && isspace((unsigned char)*endPtr)) {
            endPtr++;
        }

        if (line[0] != '\0' && *endPtr == '\0') {
            return (int)value;
        }

        printf("输入格式错误，请输入整数。\n");
    }
}

int getIntInRange(const char *prompt, int minValue, int maxValue) {
    int value;

    while (1) {
        value = getIntInput(prompt);
        if (value >= minValue && value <= maxValue) {
            return value;
        }
        printf("输入范围应为 %d 到 %d，请重新输入。\n", minValue, maxValue);
    }
}

double getDoubleInput(const char *prompt) {
    char line[128];
    char *endPtr;
    double value;

    while (1) {
        if (prompt != NULL) {
            printf("%s", prompt);
        }

        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n未读取到有效输入，默认返回 0。\n");
            return 0.0;
        }

        if (strchr(line, '\n') == NULL) {
            clearInputBuffer();
        }

        trimNewline(line);
        trimSpaces(line);

        value = strtod(line, &endPtr);
        while (*endPtr != '\0' && isspace((unsigned char)*endPtr)) {
            endPtr++;
        }

        if (line[0] != '\0' && *endPtr == '\0') {
            return value;
        }

        printf("输入格式错误，请输入数字。\n");
    }
}

int askYesNo(const char *prompt) {
    char answer[16];

    while (1) {
        printf("%s", prompt);
        if (fgets(answer, sizeof(answer), stdin) == NULL) {
            return 0;
        }

        if (strchr(answer, '\n') == NULL) {
            clearInputBuffer();
        }

        trimNewline(answer);
        trimSpaces(answer);

        if (strcmp(answer, "y") == 0 || strcmp(answer, "Y") == 0 ||
            strcmp(answer, "yes") == 0 || strcmp(answer, "YES") == 0 ||
            strcmp(answer, "是") == 0) {
            return 1;
        }

        if (strcmp(answer, "n") == 0 || strcmp(answer, "N") == 0 ||
            strcmp(answer, "no") == 0 || strcmp(answer, "NO") == 0 ||
            strcmp(answer, "否") == 0) {
            return 0;
        }

        printf("请输入 y 或 n。\n");
    }
}

static int isLeapYear(int year) {
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

int isValidDate(const char *date) {
    int year;
    int month;
    int day;
    int maxDay;
    int i;
    int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (date == NULL || strlen(date) != 10) {
        return 0;
    }

    for (i = 0; i < 10; i++) {
        if (i == 4 || i == 7) {
            if (date[i] != '-') {
                return 0;
            }
        } else if (!isdigit((unsigned char)date[i])) {
            return 0;
        }
    }

    year = (date[0] - '0') * 1000 + (date[1] - '0') * 100 +
           (date[2] - '0') * 10 + (date[3] - '0');
    month = (date[5] - '0') * 10 + (date[6] - '0');
    day = (date[8] - '0') * 10 + (date[9] - '0');

    if (year < 1900 || year > 2100 || month < 1 || month > 12) {
        return 0;
    }

    maxDay = daysInMonth[month - 1];
    if (month == 2 && isLeapYear(year)) {
        maxDay = 29;
    }

    return day >= 1 && day <= maxDay;
}

int compareDate(const char *date1, const char *date2) {
    return strcmp(date1, date2);
}

void printLine(char ch, int count) {
    int i;

    for (i = 0; i < count; i++) {
        putchar(ch);
    }
    putchar('\n');
}
