#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <clocale>
#include <ctime>
#define _CRT_SECURE_NO_WARNINGS
using namespace std;
int main()
{
	setlocale(LC_ALL, "Rus");
	srand(time(NULL));
	const int N = 8;
	int dig[N][N];
	double mas[N];
	printf("--------------------------Матрица--------------------------\n");
	int S1, sum;
	S1 = 7;
	sum = 0;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			dig[i][j] = rand() % 199 - 100;
			cout << dig[i][j] << "\t";
			if (j <= S1)
				sum += dig[i][j];
		}
		S1--;
		cout << endl << "\n\n";
	}
	cout << "Сумма элементов в заштрихованной области = " << sum << endl;
	cout << "\nМассив, содержащий средние арифметические значения строк:\n" << endl;
	for (int i = 0; i < N; i++)
	{
		int k;
		k = 0;
			for (int j = 0; j < N; j++)
			{
				k += dig[i][j];
			}
		mas[i] = (double)k / N;
		cout << setw(8) << mas[i];
	}
	cout << "\n";
	return(0);
}