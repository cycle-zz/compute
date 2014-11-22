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

#ifndef __SC_PLATFORM_H__
#define __SC_PLATFORM_H__

/* Platform  **************************************************************************************/

#if defined(__APPLE__) && !defined(SC_PLATFORM_MACOSX)
	#define SC_PLATFORM_MACOSX	(1)
#elif (defined (_WIN32) || defined(__WIN32__)) && defined(_MSC_VER) && !defined(SC_PLATFORM_WINDOWS)
	#define SC_PLATFORM_WINDOWS	(1)
#elif (defined(linux) || defined(__linux) || defined (__linux__)) && !defined(SC_PLATFORM_LINUX)
	#define SC_PLATFORM_LINUX	(1)
#endif

#if !(defined(__i386__) || defined(__amd64__))
	#define SC_ARCH_PPC		(1)
#endif

#if (defined(__i386__) || defined(__amd64__))
	#define SC_ARCH_X86		(1)
	#if defined(__amd64__)
		#define SC_64BIT	(1)
	#else
		#define SC_32BIT	(1)
	#endif
#endif

/* C Declaration **********************************************************************************/

#if !defined(SC_EXTERN_C_BEGIN)
	#if defined(__cplusplus)
		#define SC_EXTERN_C_BEGIN extern SC_API_EXPORT "C" {
		#define SC_EXTERN_C_END   }
	#else
		#define SC_EXTERN_C_BEGIN
		#define SC_EXTERN_C_END
	#endif
#endif

/* Restrict Declaration ***************************************************************************/

#if !defined(__cplusplus) && defined(__GNUC__)
	#define SC_RESTRICT			restrict
#else
	#define SC_RESTRICT
#endif

/**************************************************************************************************/

SC_EXTERN_C_BEGIN

/* Export Declaration *****************************************************************************/

#if defined(SC_PLATFORM_WINDOWS)
	#if defined(SC_SHARED_LIB_TARGET) && defined(SC_SHARED_LIB_EXPORT)
		#define SC_API_EXPORT 				__declspec(dllexport)
	#elif defined(SC_SHARED_LIB_TARGET)
		#define SC_API_EXPORT 				__declspec(dllimport)
	#endif
#else
	#define SC_API_EXPORT
#endif

/* Inline Declaration *****************************************************************************/

#if !defined(SC_INLINE)
	#if defined(__GNUC__) && (__GNUC__ == 4) && !defined(DEBUG)
		#define SC_INLINE 			static __inline__ __attribute__((always_inline))
	#elif defined(__GNUC__)
		#define SC_INLINE 			static __inline__
	#elif defined(__cplusplus)
		#define SC_INLINE 			static inline
	#elif defined(_MSC_VER)
		#define SC_INLINE 			static __inline
	#elif defined(__WIN32__)
		#define SC_INLINE			static __inline__
	#endif
#endif

/* alignment detection *********************************************************/

#if defined(__GNUC__)
	#define SC_DECLARE_ALIGNED(x)
	#define SC_ALIGN_ATTRIBUTE(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
	#define SC_DECLARE_ALIGNED(x) __declspec(align( x ))
	#define SC_ALIGN_ATTRIBUTE(x)
#else
	#define SC_DECLARE_ALIGNED(x)
	#define SC_ALIGN_ATTRIBUTE(x)
#endif

/* Helper Macros **************************************************************/

#define SC_CONCAT(a,b)		a##b
#define SC_MAKE_NAME(a,b)	SC_CONCAT(a,b)

/* OS Dependent Headers *******************************************************/

#if defined(SC_PLATFORM_WINDOWS)
	#include <windows.h>
	#include <CL/cl.h>
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <sys/time.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdarg.h>

	#if defined(SC_PLATFORM_MACOSX)
		#include <OpenCL/opencl.h>
	#else
		#include <CL/cl.h>
	#endif
#endif

/* Standard Headers ***********************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <ctype.h>

#if !defined(SC_RELEASE_BUILD)
#define scAssert(expr) \
    ((expr) ? (void)0 : \
        scCritical(SC_DEFAULT_SESSION, "Assertion \"%s\" failed in %s, line %d!\n", \
               #expr, __FILE__, __LINE__))
#else
#define scAssert(expr) do{ } while(0)
#endif

/******************************************************************************/

SC_EXTERN_C_END

#endif	/* __SC_PLATFORM_H__ */

