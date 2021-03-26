// OpenMP_3_lab.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <omp.h>
#include <fstream>

#define N 2048

using namespace std;

int main()
{
    double a[N], b[N], c[N], d[N], f[N], x[N];

    double aTemp[N], bTemp[N], cTemp[N], fTemp[N];

    double t1, t2;

    // ініціалізація матриці
    for (int i = 0; i < N; i++) {
        a[i] = rand(); 
        b[i] = rand(); 
        c[i] = rand(); 
        d[i] = rand(); 
        f[i] = rand();
    }

    t1 = omp_get_wtime();

    // основний обчислювальний блок
    // прямий хід
    for (int m = 1; m < N; m *= 2){
        double alphaTemp = 0.0, betaTemp = 0.0;

        #pragma omp parallel for
        for (int tx = 0; tx < N; tx++)
        {
            aTemp[tx] = 0, 0; bTemp[tx] = 0, 0; cTemp[tx] = 0, 0; fTemp[tx] = 0, 0;

            if ((tx + 1) % (2 * m) == 0) {
                if (tx - m >= 0 && tx < N) {
                    alphaTemp = -a[tx] / b[tx - m];
                }

                if (tx + m < N) {
                    betaTemp = -c[tx] / b[tx + m];
                }

                if (tx - m >= 0) {
                    aTemp[tx] = alphaTemp * a[tx - m];
                }

                if (tx - m >= 0 && tx + m < N) {
                    bTemp[tx] = b[tx] + alphaTemp * c[tx - m] + betaTemp * a[tx + m];
                }

                if (tx + m < N) {
                    cTemp[tx] = betaTemp * c[tx + m];
                }

                if (tx - m >= 0 && tx + m < N) {
                    fTemp[tx] = f[tx] + alphaTemp * f[tx - m] + betaTemp * f[tx + m];
                }
            }

        }

        #pragma omp parallel for
        for (int tx = 0; tx < N; tx++)
        {
            if ((tx + 1) % (2 * m) == 0)
            {
                a[tx] = aTemp[tx]; 
                b[tx] = bTemp[tx]; 
                c[tx] = cTemp[tx]; 
                f[tx] = fTemp[tx];
            }
        }
    }

    // зворотний хід
    for (int m = N / 2; m >= 1; m /= 2)
    {

        #pragma omp parallel for
        for (int tx = 0; tx < N; tx++)
        {
            if ((tx + 1 + m) % (2 * m) == 0) {

                if (tx - m >= 0 && tx + m < N) {
                    x[tx] = (f[tx] - x[tx - m] * a[tx] - x[tx + m] * c[tx + m]) / b[tx];
                }

                if (tx - m < 0 && tx + m < N) {
                    x[tx] = (f[tx] - x[tx + m] * c[tx + m]) / b[tx];
                } 

                if (tx - m >= 0 && tx + m >= N) {
                    x[tx] = (f[tx] - x[tx - m] * a[tx]) / b[tx];
                }

                if (tx - m < 0 && tx + m >= N) {
                    x[tx] = f[tx] / b[tx];
                }
            }
        }

        if (m == 1) {
            break;
        }
    }

    t2 = omp_get_wtime();

    for (int i = 0; i < N; i++) {
        cout << x[i] << endl;
    }
}