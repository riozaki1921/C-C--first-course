#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <locale.h>
#include <iostream>
#define N 5
#define _CRT_SECURE_NO_WARNINGS
using namespace std;
void initmas(int (*matr)[N]) {
    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++)
            cout << matr[i][k];
    }
}
void printmatr(int *matr , int n) {
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            cout << matr[i][k] << "\t";  
        }
        cout<<"\n"<<"\n";
    }
    printf("\n");
}
//void arithmetic(int* matr, int n, double* p) {
//    int sum;
//    for (int k = 0; k < n; k++) {
//        for (int i = 0; i < n; i++) {
//            if ((i + 1) % n)
//                sum += matr[i][k];
//
//            else {
//                *p = sum / n;
//                p++;
//            }
//        }
//    }
//}
void task1(int *matrptr) {
    double sred[N];
    /*arithmetic(&matrptr, N, sred);*/
}
int main() {
    int choice;
    int dig[N][N];
    setlocale(LC_ALL, "Rus");
    srand(time(0));
    printmatr(dig);
    do {
        printf("\n=== ВЫБЕРИТЕ ЗАДАНИЕ ===\n");
        cin >> choice;
        printf("\n");
        switch (choice) {
        case 1:
            printf("ЗАДАНИЕ 1:\n");
            task1();
            break;
        case 2:
            printf("ЗАДАНИЕ 2:\n");
            break;
        default:
            printf("Неверный выбор! Попробуйте снова.\n");
        }
    } while (choice != 0);
    return 0;
}