#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char change_str(const char* str, const char* sub_o, const char* sub_n) {
	char new_str = NULL;
	printf(strstr(str, sub_o));

	return "qqq";
}

int main() {
	setlocale(0, "Rus");
	char buff[80];
	char* str1 = NULL, * str2 = NULL;
	char* old_sub = NULL, * new_sub = NULL;
	char result;

	printf("Введите первую строку: ");
	gets_s(buff);
	str1 = (char*)malloc(strlen(buff) + 1);
	strcpy(str1, buff);

	printf("Введите вторую строку: ");
	gets_s(buff);
	str2 = (char*)malloc(strlen(buff) + 1);
	strcpy(str2, buff);

	printf("Введите заменяемую подстроку: ");
	gets_s(buff);
	old_sub = (char*)malloc(strlen(buff) + 1);
	strcpy(old_sub, buff);

	printf("Введите подстроку которой заменить: ");
	gets_s(buff);
	new_sub = (char*)malloc(strlen(buff) + 1);
	strcpy(new_sub, buff);

	result = change_str(str1, old_sub, new_sub);
	free(str1);
	free(str2);
	return 0;
}
