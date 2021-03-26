// OpenMP_2_lab.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <omp.h>
#include <fstream>

#define SIZE 100

using namespace std;

int main()
{
    int i, j;
    double a[SIZE][SIZE], b[SIZE][SIZE], c[SIZE][SIZE];
    double start_time, end_time;
    struct timespec start, end;

    ofstream sum_file("vector.txt");

    #pragma omp parallel for schedule(dynamic,50) num_threads(8)
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            a[i][j] = (rand() % 100) * 1.0;
            b[i][j] = (rand() % 100 + 500) * 1.0;
        }
    }

    start_time = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic,50) num_threads(8) private(i,j) shared(a,b,c)
    {
        #pragma omp for  schedule(static)
        for (i = 0; i < SIZE; i++) for (j = 0; j < SIZE; j++) c[i][j] = a[i][j] + b[i][j];
    }
    end_time = omp_get_wtime();

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
             sum_file << "[" << i << "]" << "[" << j << "]: " << a[i][j] << " + " << b[i][j] << " = " << c[i][j] << "\n";
        }
    }

    cout << "Time: " << (end_time - start_time) << " second";

    sum_file.close();
}
