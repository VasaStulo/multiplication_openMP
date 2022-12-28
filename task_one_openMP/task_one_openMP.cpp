#include <iostream>
#include <omp.h>
#include <time.h>
#include <math.h>
#include <Windows.h>
#include <iomanip>
#include <stdlib.h>
using namespace std;

// Функция стандартного умножения матриц
void multiplication_standart(float** A, float** B, float** C, int N) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			C[i][j] = 0.0;
			for (int k = 0; k < N; k++)
				C[i][j] += A[i][k] * B[k][j];
		}
	}
}

// Функция параллельного умножения матриц (OpenMP)
void multiplication_parallel(float** A, float** B, float** C, int N) {
#pragma omp parallel for shared(A, B, C)
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			C[i][j] = 0.0;
			for (int k = 0; k < N; k++) C[i][j] += A[i][k] * B[k][j];
		}
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");

	int i, j, k, N;

	N = 2000;
	float** a, ** b, ** c;
	a = new float* [N];
	for (i = 0; i < N; i++)
		a[i] = new float[N];
	b = new float* [N];
	for (i = 0; i < N; i++)
		b[i] = new float[N];
	c = new float* [N];
	for (i = 0; i < N; i++)
		c[i] = new float[N];
	float t1, t2, time_start, time_final;

	// Инициализация матриц
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			a[i][j] = b[i][j] = tan(i) * sin(j);

	time_start = omp_get_wtime();
	multiplication_standart(a, b, c, N);
	time_final = omp_get_wtime();
	float standartTime = time_final - time_start;
	cout << "Время стандартного умножения = " << standartTime << "с" << endl;

	int num_of_threads;
	int max_threads = omp_get_num_procs(); // возвращает количество процессоров, доступных при вызове функции
	float times[9] = { standartTime };

	cout << "В системе доступно " << max_threads << " потоков" << endl;
	for (int i = 1; i <= max_threads; i++) {
		omp_set_num_threads(i);

		cout << "Начато параллельное умножение матриц в режиме с кол-вом потоков " << i << endl;

		t1 = omp_get_wtime();
		multiplication_parallel(a, b, c, N);
		t2 = omp_get_wtime();
		float timeCalculate = t2 - t1;
		times[i] = timeCalculate;
		cout << "Время параллельного умножения = " << timeCalculate << "с" << endl;
		cout << "" << endl;
	}

	// Выводим таблицу с результатами
	cout << "-------------------------------------------------------\n";
	
	std::cout << "КОЛИЧЕСТВО ПОТОКОВ " << std::setw(25) << "ВРЕМЯ ВЫПОЛНЕНИЯ" << std::setw(2) << std::endl;

	std::cout << std::setw(18) << "базовый вариант" << std::setw(16) << times[0] << "c" << "\n" << std::endl;

	for (int i = 1; i < 9; i++)
		std::cout << std::setw(10) << i << std::setw(25) << times[i] << "c" << "\n" << std::endl;

	cout << "-------------------------------------------------------\n";

	
}