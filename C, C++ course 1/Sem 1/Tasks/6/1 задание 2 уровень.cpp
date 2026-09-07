#define _CRT_SECURE_NO_WARNINGS

#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void info(char* dpt, int* wordsinlines) {
    int space = 0, number = 0, control_characters = 0, len = strlen(dpt);
    
    for (int i = 0; i < len; i++) {
        if (isdigit(dpt[i]))
            number++;

        if (isspace(dpt[i]))
            space++;

        if (iscntrl(dpt[i]))
            control_characters++;
    }

    printf("Статистика: слов = %d, цифр= %d, пробелов= %d, управляющих= %d\n",
        *wordsinlines, number, space, control_characters);
}

int main() {
    int N;
    setlocale(0, "Rus");
    char buff[80];
    char stop[] = "and";
    scanf("%d", &N);

    char** dpt = (char**)malloc(sizeof(char*)*N);
    int i, k = 0;
    int* str_len = (int*)malloc(sizeof(int) * N);

    printf("Введите текст на латинице, для остановки введите 'and' (максимальное количество %d строк)\n", N);
    for (i = 0; i < N; i++) {
        gets_s(buff);
        if (strcmp(buff, stop) == 0) {
            k = i;
            break;
        }
        dpt[i] = (char*)malloc(strlen(buff) + 1);

        if (dpt[i] == NULL) {
            printf("Ошибка выделения памяти");
            break;
        }
        strcpy(dpt[i], buff);

        int words = 0;
        int len = strlen(dpt[i]);
        if (len > 0 && isalpha(dpt[i][0]))
            words = 1;
        for (int j = 0; j < len - 1; j++) {
            if (isspace(dpt[i][j]) && isalpha(dpt[i][j + 1]))
                words++;
        }
        str_len[i] = words;
    }

    if (k == 0)
        k = i;

    printf("\n==========================================================================\n");

    for (i = 0; i < k - 1; i++) {
        for (int j = i + 1; j < k; j++) {
            if (str_len[i] < str_len[j]) {
                int little_len = str_len[i];
                str_len[i] = str_len[j];
                str_len[j] = little_len;

                char* little_str = dpt[i];
                dpt[i] = dpt[j];
                dpt[j] = little_str;
            }
        }
    }

    printf("Строки отсортированные по количеству слов:\n\n");

    for (i = 0; i < k; i++) {
        printf("%s\n", dpt[i]);
        info(dpt[i], &str_len[i]);
        printf("\n");
    }

    for (i = 0; i < k; i++)
        free(dpt[i]);
    free(dpt);
    free(str_len);
    return 0;
}