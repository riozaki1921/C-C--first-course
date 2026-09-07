#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#define N 20
using namespace std;

void initMatrix(int (*matrix)[N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            matrix[i][j] = rand() % 201 - 100;
    }
}

void printMatrix(int (*matrix)[N]) {
    cout << "==========Матрица " << N << "x" << N << ":==========\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            cout << matrix[i][j]<<"\t";
        cout << "\n\n";
    }
}

void calculateRowAverages(int (*matrix)[N], double* averages) {
    for (int i = 0; i < N; i++) {
        int sum = 0;
        for (int j = 0; j < N; j++)
            sum += matrix[i][j];
        averages[i] = (double)sum / N;
    }
}

void findMinMaxInShadedArea(int (*matrix)[N], int* min, int* max) {
    *min = matrix[0][0];
    *max = matrix[0][0];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - i - 1; j++) {
            if (matrix[i][j] < *min)
                *min = matrix[i][j];
            if (matrix[i][j] > *max)
                *max = matrix[i][j];
        }
    }
}

void printAverages(double* averages) {
    cout << "\nСредние арифметические значений строк:\n";
    cout << fixed << setprecision(2);
    for (int i = 0; i < N; i++)
        cout << "Строка " << i + 1 << ": " << averages[i] << "\n";
    cout << defaultfloat;
}

void printAveragesInLine(double* averages) {
    cout << "\nСредние значения строк матрицы:\n";
    cout << fixed << setprecision(2);
    for (int i = 0; i < N; i++)
        cout << averages[i] << "\t";
    cout << defaultfloat << "\n";
}

int main() {
    int matrix[N][N];
    double averages[N];
    int min, max;
    setlocale(LC_ALL, "Rus");
    srand(time(0));
    initMatrix(matrix);
    printMatrix(matrix);
    int choice;
    do {
        cout << "\n=== ВЫБЕРИТЕ ЗАДАНИЕ ===" << endl;
        cout << "1 - Средние арифметические строк" << endl;
        cout << "2 - Минимум и максимум в заштрихованной области" << endl;
        cout << "0 - Выход" << endl;
        cout << "Ваш выбор: ";
        cin >> choice;
        cout << "\n";
        switch (choice) {
        case 1:
            cout << "ЗАДАНИЕ 1:\n";
            calculateRowAverages(matrix, averages);
            printAveragesInLine(averages);
            break;
        case 2:
            cout << "ЗАДАНИЕ 2:\n";
            findMinMaxInShadedArea(matrix, &min, &max);
            cout << "Минимум в заштрихованной области: " << min << "\n";
            cout << "Максимум в заштрихованной области: " << max << "\n";
            break;
        case 0:
            cout << "Выход из программы.\n";
            break;
        default:
            cout << "Неверный выбор! Попробуйте снова.\n";
        }
    } while (choice != 0);
    return 0;
}