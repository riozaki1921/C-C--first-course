#include <stdio.h>
#include <time.h>
#include <locale>
#include <iostream>
#define N 10 
#define M 5
#define MIN_N 1
using namespace std;
void matrprint1(int C[][M], int STR, int STL) {
    for (int i = 0; i < STR; i++){
        for (int j = 0; j < STL; j++){
            C[i][j] = rand() % 199 - 100;
            cout << C[i][j] << "\t";
        }
        cout << "\n\n";
    }
}
int MIN(int C[][M], int STR, int STL) {
    int min;
    for (int i = STR/2; i < STR; i++) {
        min = C[i][MIN_N];
        for (int j = (STR / 2); j < STR; j++) {
            if (min < 0)
                min = C[j][MIN_N];
            if 
        }
        if (C[i][MIN_N] > 0) {
            if (C[STR / 2][MIN_N]< C[i][MIN_N]) 
                if (C[STR / 2][MIN_N]<0)
                    min = C[i][MIN_N];
            if (min > C[i][MIN_N]){
                min = C[i][MIN_N];
            }
        } 
    if (min < 0)
        min = -1;
    }
    return min;
}

int main() {
    setlocale(LC_ALL, "Rus");
    srand(time(0));
    int A[N][M];
    matrprint1(A, N, M);
    cout << "------МИНИМАЛЬНОЕ ПОЛОЖИТЕЛЬНОЕ ЗНАЧЕНИЕ В НАЖНЕЙ ПОЛОВИНЕ МАТРИЦЫ В "<< MIN_N+1<<" СТОЛБЦЕ------\n";
    int q = MIN(A, N, M);
    if (q == -1){
        cout << "Нет положительных\n";
    }
    else {
        cout << q << "\n";
    }
    return 0;
}