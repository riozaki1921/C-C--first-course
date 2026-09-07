#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* replace_substring(const char* str, const char* old, const char* newq) {
    if (!str || !old || !newq)
        return NULL;

    int old_len = strlen(old);
    int new_len = strlen(newq);
    int count = 0;

    const char* tmp = str;
    while ((tmp = strstr(tmp, old))) {
        count++;
        tmp += old_len;
    }

    int result_len = strlen(str) + count * (new_len - old_len) + 1;
    char* result = (char*)malloc(result_len);
    if (!result) return NULL;

    char* current = result;
    tmp = str;
    while (*tmp) {
        if (strstr(tmp, old) == tmp) {
            strcpy(current, newq);
            current += new_len;
            tmp += old_len;
        }
        else {
            *current++ = *tmp++;
        }
    }
    *current = '\0';
    return result;
}

int main() {
    setlocale(0, "Russian");
    char buff[80];
    char* str1, * str2;
    char* old_sub, * new_sub;

    printf("Введите первую строку: ");
    gets_s(buff);
    str1 = (char*)malloc(strlen(buff) + 1);
    strcpy(str1, buff);

    printf("Введите вторую строку: ");
    gets_s(buff);
    str2 = (char*)malloc(strlen(buff) + 1);
    strcpy(str2, buff);

    printf("Введите подстроку для замены (первая строка): ");
    gets_s(buff);
    old_sub = (char*)malloc(strlen(buff) + 1);
    strcpy(old_sub, buff);

    printf("Введите новую подстроку (первая строка): ");
    gets_s(buff);
    new_sub = (char*)malloc(strlen(buff) + 1);
    strcpy(new_sub, buff);

    char* new_str1 = replace_substring(str1, old_sub, new_sub);
    if (new_str1) {
        printf("Результат (строка 1): %s\n", new_str1);
        free(new_str1);
    }

    printf("Введите подстроку для замены (вторая строка): ");
    gets_s(buff);
    free(old_sub);
    old_sub = (char*)malloc(strlen(buff) + 1);
    strcpy(old_sub, buff);

    printf("Введите новую подстроку (вторая строка): ");
    gets_s(buff);
    free(new_sub);
    new_sub = (char*)malloc(strlen(buff) + 1);
    strcpy(new_sub, buff);

    char* new_str2 = replace_substring(str2, old_sub, new_sub);
    if (new_str2) {
        printf("Результат (строка 2): %s\n", new_str2);
        free(new_str2);
    }

    free(str1);
    free(str2);
    free(old_sub);
    free(new_sub);

    system("pause");
    return 0;
}