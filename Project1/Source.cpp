#include <iostream>
#include <fstream>
#include <omp.h>
#include "windows.h"

using namespace std;

void writeFile(ifstream &file)
{
	int pos = file.tellg();

	file.seekg(0, ios_base::beg);

	int n;
	double eps;

	file >> n >> eps;

	double *b = new double[n];

	float **s = new float*[n];

	for (int i = 0; i < n; i++)
		s[i] = new float[n];

	for (int i = 0; i < n; i++)
		file >> b[i];

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			file >> s[i][j];

	cout << n << " " << eps << endl;

	for (int i = 0; i < n; i++)
		cout << b[i] << " ";

	cout << endl;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			cout << s[i][j] << " ";

		cout << endl;

	}

	file.seekg(pos + 1, ios_base::beg);

}

void Lab_2(string intput, string output)
{
	ifstream file(intput);

	if (file.is_open())
		cout << "Все ОК! Файл открыт!\n\n" << endl;
	else
	{
		cout << "Файл не открыт!\n\n" << endl;
		return;
	}

	int n;
	double eps;

	file >> n >> eps;

	double *b = new double[n];

	float **a = new float*[n];

	for (int i = 0; i < n; i++)
		a[i] = new float[n];

	for (int i = 0; i < n; i++)
		file >> b[i];

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			file >> a[i][j];

	double *x = new double[n];
	double timeWork[4] = { 0 };
	int countInteraction = 0;

	for (int countThread = 1, count = 0; countThread <= 8; countThread *= 2, count++)
	{
		double diff = 1;
		int countInt = 0;
		double start_time, end_time;
		omp_set_num_threads(countThread);

		for (int i = 0; i < n; i++)
		{
			x[i] = b[i];

		}

		start_time = omp_get_wtime();

		while (diff > eps)
		{
			double *xNext = new double[n];
			diff = 0;

			for (int i = 0; i < n; i++)
			{
				double xNext1 = 0;
				double xNext2 = 0;
				xNext[i] = b[i] / a[i][i];

#pragma omp parallel for reduction(+:xNext1)
				for (int j = 0; j < i - 1; j++)
				{
					xNext1 += (a[i][j] / a[i][i])*x[j];

				}

#pragma omp parallel for reduction(+:xNext2)
				for (int j = i + 1; j < n; j++)
				{
					xNext2 += (a[i][j] / a[i][i])*x[j];

				}

				xNext[i] -= xNext1 + xNext2;

				if (diff < abs(xNext[i] - x[i]))
				{
					diff = abs(xNext[i] - x[i]);

				}

			}

			for (int i = 0; i < n; i++)
			{
				x[i] = xNext[i];

			}

			countInt++;
			delete[] xNext;

		}

		end_time = omp_get_wtime();
		timeWork[count] = end_time - start_time;
		countInteraction = countInt;

	}
	ofstream fout(output);

	fout << countInteraction << endl;

	fout.precision(4);
	fout.setf(std::ios::fixed);

	for (int i = 0; i < n; i++)
	{
		fout << x[i] << " ";

	}

	fout << endl;

	for (int i = 0; i < 4; i++)
	{
		fout << timeWork[i] << " ";

	}

	delete[] b;
	delete[] x;

	for (int count = 0; count < n; count++)
		delete[] a[count];

}

string to_string(int n)
{
	char buf[40];
	sprintf_s(buf, "%d", n);
	return buf;
}

int main(int argc, char **argv)
{
	setlocale(LC_ALL, "rus");

	for (int i = 0; i < 9; i++)
	{
		Lab_2("D:\\Study\\Параллельное_программирование\\Project1\\Тесты\\input2_0" + to_string(i) + ".txt",
			"D:\\Study\\Параллельное_программирование\\Project1\\Ответы на Тесты\\output2_00" + to_string(i) + ".txt");
	}

	return EXIT_SUCCESS;
}