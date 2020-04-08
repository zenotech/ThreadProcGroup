
#include "pch.h"
#include <iostream>
#include <cassert>
#define _WIN32_WINNT 0x0601
#include <Windows.h>

#include "ThreadProcGroup.h"

/* 
 * This function should be used inside an OMP parallel region to change the 
 * thread affinity to use multiple processor groups
 * 
 * Note: Uses stdcall and capitalisation to support NAG Fortran linkage
 *       when compiling for x64 on Windows
 *
 * Fortran example

!$OMP PARALLEL
    CALL SETTHREADPROCESSORGROUP(OMP_GET_THREAD_NUM(), OMP_GET_NUM_THREADS())
!$OMP END PARALLEL

 *
 */
void SETTHREADPROCESSORGROUP(int *ompThreadNo, int *ompTotalThreads) {
	WORD procGroupCount = GetActiveProcessorGroupCount();
	DWORD procCount = GetActiveProcessorCount(0);

	// Debugging - comment out for release
	if(*ompThreadNo == 0)
		std::cout << " Processor Group: " << procGroupCount << " Processor Count: " << procCount << std::endl;

	if (*ompTotalThreads % procGroupCount != 0) {
		if (*ompThreadNo == 0)
			std::cout << " SetThreadProcessorGroup: Failed - OMP_NUM_THREADS not divisible by processor group" << std::endl;
		return;
	}

	if (*ompTotalThreads > procGroupCount*procCount) {
		if (*ompThreadNo == 0)
			std::cout << " SetThreadProcessorGroup: Failed - OMP_NUM_THREADS greater than total number of cores" << std::endl;
		return;
	}

	// If active processors > total threads do nothing
	if (procCount > *ompTotalThreads)
		return;

	// Only change affinity if more than 1 processor group
	if (procGroupCount > 1) {

		HANDLE thread = GetCurrentThread();

		// Distribute threads evenly across processor groups
		unsigned int threadsPerGroup = *ompTotalThreads / procGroupCount;

		unsigned int threadGroup = *ompThreadNo / threadsPerGroup;

		GROUP_AFFINITY groupAffinity;
		GROUP_AFFINITY previousAffinity;

		groupAffinity.Reserved[0] = 0;
		groupAffinity.Reserved[1] = 0;
		groupAffinity.Reserved[2] = 0;
		groupAffinity.Group = threadGroup;
		groupAffinity.Mask = (1ULL << (*ompThreadNo % threadsPerGroup));

		// Debugging - comment out for release
		if (*ompThreadNo % threadsPerGroup == 0) {
			std::cout << " Thread: " << *ompThreadNo << " using Proc Group: " << threadGroup << std::endl;
		}

		BOOL err = SetThreadGroupAffinity(thread, &groupAffinity, &previousAffinity);
		if (err == 0) {
			std::cout << " SetThreadProcessorGroup: Failed to set thread processor group affinity" << std::endl;
		}
	}
}
