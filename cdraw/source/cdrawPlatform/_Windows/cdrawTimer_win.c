/*
* Copyright 2023 Daniel S. Buckstein
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*/

/*
* cdrawTimer_win.c
* Windows implementation of high-precision timer.
*/

#include "cdraw/cdrawPlatform/cdrawTimer.h"
#if CDRAW_TARGET_WINDOWS
#include <Windows.h>


bool cdrawTimerInternalSysInit(ctime_t* const cps_out)
{
	return QueryPerformanceFrequency((PLARGE_INTEGER)cps_out);
}

bool cdrawTimerInternalSysStep(ctime_t* const t_out)
{
	return QueryPerformanceCounter((PLARGE_INTEGER)t_out);
}


#endif // #if CDRAW_TARGET_WINDOWS


#if !CDRAW_TARGET_WINDOWS // ****TO-DO: MOVE TO NEW PLATFORM SOURCE
#include <time.h>
#define sec2nanosec 1000000000
typedef struct timespec timespec;


bool cdrawTimerInternalSysInit(ctime_t* const cps_out)
{
	*cps_out = sec2nanosec;
	return true;
}

bool cdrawTimerInternalSysStep(ctime_t* const t_out)
{
	timespec ts;
	bool const result = clock_gettime(CLOCK_REALTIME, &ts);
	*t_out = (sec2nanosec * ts.tv_sec + ts.tv_nsec);
}


#endif // #if !CDRAW_TARGET_WINDOWS