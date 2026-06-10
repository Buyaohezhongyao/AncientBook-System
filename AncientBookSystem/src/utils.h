#ifndef UTILS_H
#define UTILS_H

void clearInputBuffer(void);
void trimNewline(char *str);
void safeStringCopy(char *dest, const char *src, int size);

void getStringInput(const char *prompt, char *buffer, int size);
int getIntInput(const char *prompt);
int getIntInRange(const char *prompt, int minValue, int maxValue);
double getDoubleInput(const char *prompt);
int askYesNo(const char *prompt);

int isValidDate(const char *date);
int compareDate(const char *date1, const char *date2);
void printLine(char ch, int count);

#endif
