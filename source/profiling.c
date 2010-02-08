/***************************************************************************************************

Scalable Compute Library - $SC_VERSION_TAG$ <$SC_ID_TAG$>

Copyright (c) 2010, Derek Gerstmann <derek.gerstmann[|AT|]uwa.edu.au> 
The University of Western Australia. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

***************************************************************************************************/

#include "profiling.h"

/**************************************************************************************************/

#if defined(SC_PLATFORM_WINDOWS)

	#define scSubtractTimeInSeconds(delta, freq, a, b) do {					\
			delta = (b).LowPart ? (((double)(a).QuadPart - (double)(b).QuadPart) / (double)(freq).QuadPart) : 0.0; \
		} while (0)
		
	#define scAssignTimeValue(dst, src) do { 								\
			(dst) = (src)c; 												\
		} while (0)

#elif defined(SC_PLATFORM_MACOSX)

	#define scGetCurrentTime(dst) do { 										\
        	(dst) = mach_absolute_time(); 									\
		} while (0)
		
	#define scSubtractTimeInSeconds(delta, freq, a, b) do {					\
			sc_time difference = (a) - (b);									\
			delta = 1e-9 * (double) (freq).numer / (double) (freq).denom;	\
			delta = delta * difference;										\
		} while (0)
		
	#define scAssignTimeValue(dst, src) do { 								\
			(dst) = (src);	 												\
		} while (0)

#else
	#define scSubtractTimeInSeconds(delta, freq, a, b) do {					\
			(void) (freq);													\
			delta = ((double)(a).tv_sec + 1.0e-6 * (double)(a).tv_usec);	\
			delta -= ((double)(b).tv_sec + 1.0e-6 * (double)(b).tv_usec);   \
		} while (0)
		
	#define scAssignTimeValue(dst, src) do { 								\
			(dst).tv_sec = (src).tv_sec; 									\
			(dst).tv_usec = (src).tv_usec; 									\
		} while (0)
		
#endif

/**************************************************************************************************/

typedef struct _sc_profiling_info_t {
	sc_time						counters[3];
	sc_frequency				frequency;
	double						conversion;
	sc_bool						initialized;
} sc_profiling_info_t;

/**************************************************************************************************/

sc_frequency 
scGetTimerFrequencyForHost(
	sc_session handle, sc_status *status)
{
	(void)handle;
	if(status)(*status) = SC_SUCCESS;

	sc_frequency frequency;
	sc_profiling_info_t* profiling = (sc_profiling_info_t*)scGetProfilingInfo(handle, NULL);
	if(profiling && profiling->initialized)
		return profiling->frequency;
		
#if defined(SC_PLATFORM_WINDOWS)

	QueryPerformanceFrequency(&(frequency));							

#elif defined(SC_PLATFORM_MACOSX)

	mach_timebase_info(&(frequency));
	
#elif defined(SC_PLATFORM_LINUX)

	frequency = 1.0;

#endif

	if(profiling)
	{
		profiling->frequency = frequency;
		profiling->initialized = SC_TRUE;
	}
	return frequency;
}

/**************************************************************************************************/

sc_time
scGetCurrentTimeForHost(
	sc_session handle, sc_status *status)
{
	(void)handle;
	if(status)(*status) = SC_SUCCESS;
	
	sc_time current;
	
#if defined(SC_PLATFORM_WINDOWS)

	QueryPerformanceCounter(&(current));							

#elif defined(SC_PLATFORM_MACOSX)

	current = mach_absolute_time();
	
#elif defined(SC_PLATFORM_LINUX)

	gettimeofday&(current), NULL);

#endif
	return current;
}

/**************************************************************************************************/

sc_double
scGetTimeDeltaForHost(
	sc_session handle, 
	sc_uint counter,
	sc_status *status)
{
	sc_time current;
	sc_frequency frequency;
    sc_double delta = 0.0;

	sc_profiling_info_t* profiling = (sc_profiling_info_t*)scGetProfilingInfo(handle, NULL);
	if(!profiling)
	{
		if(status) (*status) = SC_INVALID_PROFILING_INFO;
		scWarning(handle, "Profiling not enabled! Unable to get time delta for host!\n");
		return 0.0;
	}
	
	current = scGetCurrentTimeForHost(handle, status);
	frequency = scGetTimerFrequencyForHost(handle, status);

	if(counter < 3)
	{
		scSubtractTimeInSeconds(delta, frequency, current, profiling->counters[counter]);
		scAssignTimeValue(profiling->counters[counter], current);
		if(status)(*status) = SC_SUCCESS;
	}
	else
	{
		if(status)(*status) = SC_INVALID_VALUE;
	}
	return delta;
} 

/**************************************************************************************************/

