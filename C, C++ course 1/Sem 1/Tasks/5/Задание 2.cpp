#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <ctype.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <time.h>
using namespace std;

double* sum_columns(double** matrix, int N, int M) {
    double* sums = new double[M]();

    for (int j = 0; j < M; j++) {
        if (j < M / 2) {
            for (int i = 0; i < N / 2; i++)
                sums[j] += matrix[i][j];
        }
        else {
            for (int i = N / 2; i < N; i++)
                sums[j] += matrix[i][j];
        }
    }
    return sums;
}

int main() {
    setlocale(0, "Rus");
    srand(time(NULL));
    int N, M;
    cout << "Введите N и M: ";
    cin >> N >> M;
    int j, i;
    double** matrix = new double* [N];
    for (i = 0; i < N; i++) {
        matrix[i] = new double[M];
        for (j = 0; j < M; j++) {
            matrix[i][j] = rand() % 100 / 10.0;
            cout << setw(6) << matrix[i][j];
        }
        cout << "\n\n";
    }

    double* col_sums = sum_columns(matrix, N, M);

    cout << "Суммы столбцов:\n";
    for (int j = 0; j < M; j++)
        cout << "Столбец " << j + 1 << ": " << col_sums[j] << "\n";

    delete[] col_sums;
    for (i = 0; i < N; i++)
        delete[] matrix[i];
    delete[] matrix;

    system("pause");
    return 0;
}
