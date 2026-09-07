#include <cstdlib>
#include <iostream>
#include <clocale>
#include <ctime>
#define _CRT_SECURE_NO_WARNINGS
using namespace std;
int main()
{
	setlocale(LC_ALL, "Rus");
	srand(time(NULL));
	printf("Введите четное кол-во элементов в массиве: ");
	int N;
	int S1, S2;
	S1 = 0;
	S2 = 0;
	cin >> N;
	if (N % 2) {
		printf("\nВведенное число должно быть четным. Повторите попытку\n");
		return 1;
	}
	int* dig = new int[N];
	cout << "\n";
	for (int i = 0; i < N; i++)
	{
		dig[i] = rand() % 199 - 99;
		cout << dig[i] << "\t";
		if (i <= N / 2 && dig[i] > 0)
			S1 += dig[i];
		else
			if (i >= N / 2 && dig[i] < 0)
				S2 += dig[i];
	}
	delete[] dig;
	cout << "\n----------------------------------------------";
	cout<< "\nCумма положительных элементов массива в первой половине массива = " << S1;
	cout<<"\nCумму отрицательных элементов массива во второй половине массива = " << S2<<"\n";
	return(0);
}