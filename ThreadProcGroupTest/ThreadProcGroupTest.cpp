// ThreadProcGroupTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <omp.h>

#include "ThreadProcGroup.h"

int main()
{
	std::cout << "Hello World!\n";

#pragma omp parallel
	{
		int totalThreads = omp_get_num_threads();
		int threadNo = omp_get_thread_num();

		if (threadNo == 0)
			std::cout << " OMP_NUM_THREADS: " << totalThreads << std::endl;

		// SETTHREADPROCESSORGROUP(&threadNo, &totalThreads);
	}

	double aa = 0.0;
#pragma omp parallel
	{
		double a = 0;
#pragma omp for
		for (int i = 0; i < 1000000; ++i) {
			for (int j = 0; j < 1000000; ++j) {
				for (int k = 0; k < 1000000; ++k) {
					a += j + i + 10 * j + k;
				}
			}
		}
#pragma omp critical
			{
				aa += a;
			}
	}
	return 1;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
