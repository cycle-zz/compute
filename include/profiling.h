/***************************************************************************************************

Compute Engine - $CE_VERSION_TAG$ <$CE_ID_TAG$>

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

#ifndef __CE_PROFILING_H__
#define __CE_PROFILING_H__

/**************************************************************************************************/

#include "compute.h"

/**************************************************************************************************/

CE_EXTERN_C_BEGIN

/**************************************************************************************************/

#if defined(CE_PLATFORM_WINDOWS)
	/* Windows */
	typedef LARGE_INTEGER 		ce_time_t;
	typedef LARGE_INTEGER 		ce_frequency_t;

	#define ceGetTimerFrequency(dst) do {									\
			QueryPerformanceFrequency(&(dst));								\
		} while(0)
		
	#define ceGetCurrentTime(dst) do { 										\
        	QueryPerformanceCounter(&(dst)); 								\
		} while (0)
		
	#define ceSubtractTimeInSeconds(delta, freq, a, b) do {					\
			delta = (b).LowPart ? (((double)(a).QuadPart - (double)(b).QuadPart) / (double)(freq).QuadPart) : 0.0; \
		} while (0)
		
	#define ceAssignTimeValue(dst, src) do { 								\
			(dst) = (src)c; 												\
		} while (0)

#elif defined(CE_PLATFORM_MACOSX)
	/* OSX */
	#include <mach/mach_time.h>
	typedef uint64_t 					ce_time_t;
	typedef mach_timebase_info_data_t 	ce_frequency_t;

	#define ceGetTimerFrequency(dst) do {									\
			mach_timebase_info(&(dst));										\
		} while(0)
		
	#define ceGetCurrentTime(dst) do { 										\
        	(dst) = mach_absolute_time(); 									\
		} while (0)
		
	#define ceSubtractTimeInSeconds(delta, freq, a, b) do {					\
			ce_time_t difference = (a) - (b);								\
			delta = 1e-9 * (double) (freq).numer / (double) (freq).denom;	\
			delta = delta * difference;										\
		} while (0)
		
	#define ceAssignTimeValue(dst, src) do { 								\
			(dst) = (src);	 												\
		} while (0)

#else
	/* Linux */
	typedef struct timeval 		ce_time_t;
	typedef double 				ce_frequency_t;

	#define ceGetTimerFrequency(freq) do {									\
			(void) (freq);													\
		} while(0)
		
	#define ceGetCurrentTime(dst) do { 										\
        	gettimeofday(&(dst), NULL); 									\
		} while (0)
		
	#define ceSubtractTimeInSeconds(delta, freq, a, b) do {					\
			(void) (freq);													\
			delta = ((double)(a).tv_sec + 1.0e-6 * (double)(a).tv_usec);	\
			delta -= ((double)(b).tv_sec + 1.0e-6 * (double)(b).tv_usec);   \
		} while (0)
		
	#define ceAssignTimeValue(dst, src) do { 								\
			(dst).tv_sec = (src).tv_sec; 									\
			(dst).tv_usec = (src).tv_usec; 									\
		} while (0)
		
#endif

/**************************************************************************************************/

typedef struct _ce_profiling_info_t {
	ce_time_t					counters[3];
	ce_frequency_t				frequency;
	double						conversion;
} ce_profiling_info_t;

/**************************************************************************************************/

CE_EXTERN_C_END

#endif /* __CE_PROFILING_H__ */

