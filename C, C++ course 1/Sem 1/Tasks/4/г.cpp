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
	const int N = 6;
	int dig[N][N];
	int S1, S2;
	S1 = 0;
	S2 = 0;
	int k = N;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			dig[i][j] = rand() % 199 - 100;
			cout << dig[i][j] << '\t';
			if (dig[i][j] > 0, i <= k)
				S1 += dig[i][j];
			else
				S2 += dig[i][j];
			k--;
		}
		cout << "\n\n";
	}
	cout << "\nS1 = " << S1 << ", \n" << "S2 = " << S2 << endl;
	return(0);
}