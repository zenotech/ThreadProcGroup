#pragma once

#ifdef THREADPROCGROUP_EXPORTS
#define THREADPROCGROUP_API __declspec(dllexport)
#else
#define THREADPROCGROUP_API __declspec(dllimport)
#endif

extern "C" THREADPROCGROUP_API void SETTHREADPROCESSORGROUP(int *ompThreadNo, int *ompTotalThreads);

extern "C" THREADPROCGROUP_API void TEST();