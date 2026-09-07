#include <stdio.h>
#include <time.h>
#include <locale>
#include <iostream>
#define N1 5 
#define N2 5
using namespace std;
void matrprint1(int C[], int element){
	for (int i = 0; i < element; i++){
		C[i] = rand() % 199 - 99;
		cout << C[i] << "\t";
	}
	cout << "\n";
}
int pol(int C[], int element)
{
	for (int i = element-1; i > 0; i--){
		if (C[i] > 0){
			return  i;
		}
	}
    return -1;
}
int MIN(int C[], int w){
	int min=C[0];
	for (int i=0; i < w; i++){
		if (min > C[i])
			min = C[i];
	}
    return min;
}
int main(){
	setlocale(LC_ALL, "Rus");
	srand(time(0));
	int A[N1], B[N2];
    cout << "------------МАССИВ А------------\n";
    matrprint1(A, N1);
    int q = pol(A, N1);
    cout << "-------ПОСЛЕДНИЙ ПОЛОЖИТЕЛЬНЫЙ ЭЛЕМЕНТ МАССИВА А-------\n";
    cout << A[q] << "\n";
    if (q > 0) {
        cout << "МИНИМАЛЬНОЕ ЗНАЧЕНИЕ ЭЛЕМЕНТА В ПЕРВОЙ ЧАСТИ МАССИВА\n"
            << MIN(A, q) << "\n";
    }
    else {
        cout << "В массиве нет положительных элементов\n";
    }
    cout << "==============================================================================\n";
    cout << "------------МАССИВ B------------\n";
    matrprint1(B, N2);
    int z = pol(B, N2);
    cout << "-------ПОСЛЕДНИЙ ПОЛОЖИТЕЛЬНЫЙ ЭЛЕМЕНТ МАССИВА B-------\n";
    cout << B[z] << "\n";
    if (z > 0) {
        cout << "МИНИМАЛЬНОЕ ЗНАЧЕНИЕ ЭЛЕМЕНТА В ПЕРВОЙ ЧАСТИ МАССИВА\n"
            << MIN(B, z) << "\n";
    }
    else {
        cout << "В массиве нет положительных элементов\n";
    }
    return 0;
}