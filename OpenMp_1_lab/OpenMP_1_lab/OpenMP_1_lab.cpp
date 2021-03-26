// OpenMP_1_lab.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <omp.h>
#include <fstream>

#define SIZE 500

using namespace std;

int main()
{
    int i;
    double a[SIZE], b[SIZE], c[SIZE];
    double start_time, end_time;
    struct timespec start, end;

    ofstream sum_file("vector.txt");
    ofstream thread_file("thread.txt");

    #pragma omp parallel for schedule(static,10) num_threads(8)
    for (i = 0; i < SIZE; i++) {
        a[i] = (rand() % 100) * 1.0;
        b[i] = (rand() % 100 + 500) * 1.0;
    }
    
    start_time = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic,10) num_threads(8)
    for (i = 0; i < SIZE; i++) {
        c[i] = a[i] + b[i];
        #pragma omp critical
        {
            sum_file << "[" << i << "]: " << a[i] << " + " << b[i] << " = " << c[i] << "\n";
            thread_file << omp_get_thread_num() << "\n";
        }
    }
    end_time = omp_get_wtime();

    cout << "Time: " << (end_time - start_time) << " second";

    sum_file.close();
    thread_file.close();
}
