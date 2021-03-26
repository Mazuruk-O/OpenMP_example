// OpenMP_4_lab.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define N 64

using namespace std;

int main() {
	double a[N * N], f[N], x[N], xLast[N];
	double sum = 0;
	double eps = 0.001;
	double t1, t2;

	// ініціалізація матриці
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			a[i * N + j] = rand();
		}
	}

	for (int i = 0; i < N; i++) {
		cout << a[i] << endl;
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i != j) {
				a[i * N + i] += a[i * N + j];
			} 
		}
	}

	for (int i = 0; i < N; i++) {
		f[i] = rand();
	}

	t1 = omp_get_wtime();

	// основний обчислювальний блок
	#pragma omp parallel for
	for (int tx = 0; tx < N; tx++) {
		xLast[tx] = f[tx] / a[tx * N + tx];
	}

	do {
		#pragma omp parallel for
		for (int tx = 0; tx < N; tx++) {
			
			x[tx] = f[tx];

			for (int j = 0; j < N; j++) {
				if (tx != j) {
					x[tx] -= a[tx * N + j] * xLast[j];
				} 
			}

			x[tx] /= a[tx * N + tx];
		}

		sum = 0;

		#pragma omp parallel for reduction(+: sum)
		for (int tx = 0; tx < N; tx++) {
			sum += fabs(x[tx] - xLast[tx]);
		}

		#pragma omp parallel for
		for (int tx = 0; tx < N; tx++) {
			xLast[tx] = x[tx];
		}
	} while (sum > eps);

	t2 = omp_get_wtime();

	return 0;
}
