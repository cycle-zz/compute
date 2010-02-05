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

#ifndef __CE_PLATFORM_H__
#define __CE_PLATFORM_H__

/* Platform  **************************************************************************************/

#if defined(__APPLE__) && !defined(CE_PLATFORM_MACOSX)
	#define CE_PLATFORM_MACOSX	(1)
#elif (defined (_WIN32) || defined(__WIN32__)) && defined(_MSC_VER) && !defined(CE_PLATFORM_WINDOWS)
	#define CE_PLATFORM_WINDOWS	(1)
#elif (defined(linux) || defined(__linux) || defined (__linux__)) && !defined(CE_PLATFORM_LINUX)
	#define CE_PLATFORM_LINUX	(1)
#endif

#if !(defined(__i386__) || defined(__amd64__))
	#define CE_ARCH_PPC		(1)
#endif

#if (defined(__i386__) || defined(__amd64__))
	#define CE_ARCH_X86		(1)
	#if defined(__amd64__)
		#define CE_64BIT	(1)
	#else
		#define CE_32BIT	(1)
	#endif
#endif

/* C Declaration **********************************************************************************/

#if !defined(CE_EXTERN_C_BEGIN)
	#if defined(__cplusplus)
		#define CE_EXTERN_C_BEGIN extern CE_API_EXPORT "C" {
		#define CE_EXTERN_C_END   }
	#else
		#define CE_EXTERN_C_BEGIN
		#define CE_EXTERN_C_END
	#endif
#endif

/* Restrict Declaration ***************************************************************************/

#if !defined(__cplusplus) && defined(__GNUC__)
	#define CE_RESTRICT			restrict
#else
	#define CE_RESTRICT
#endif

/**************************************************************************************************/

CE_EXTERN_C_BEGIN

/* Export Declaration *****************************************************************************/

#if defined(CE_PLATFORM_WINDOWS)
	#if defined(CE_SHARED_LIB_TARGET) && defined(CE_SHARED_LIB_EXPORT)
		#define CE_API_EXPORT 				__declspec(dllexport) 
	#elif defined(CE_SHARED_LIB_TARGET)
		#define CE_API_EXPORT 				__declspec(dllimport) 
	#endif
#else
	#define CE_API_EXPORT
#endif

/* Inline Declaration *****************************************************************************/

#if !defined(CE_INLINE)
	#if defined(__GNUC__) && (__GNUC__ == 4) && !defined(DEBUG)
		#define CE_INLINE 			static __inline__ __attribute__((always_inline))
	#elif defined(__GNUC__)
		#define CE_INLINE 			static __inline__
	#elif defined(__cplusplus)
		#define CE_INLINE 			static inline
	#elif defined(_MSC_VER)
		#define CE_INLINE 			static __inline
	#elif defined(__WIN32__)
		#define CE_INLINE			static __inline__
	#endif
#endif

/* alignment detection *********************************************************/

#if defined(__GNUC__)
	#define CE_DECLARE_ALIGNED(x) 
	#define CE_ALIGN_ATTRIBUTE(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
	#define CE_DECLARE_ALIGNED(x) __declspec(align( x )) 
	#define CE_ALIGN_ATTRIBUTE(x) 
#else
	#define CE_DECLARE_ALIGNED(x)
	#define CE_ALIGN_ATTRIBUTE(x)
#endif

/* Helper Macros **************************************************************/

#define CE_CONCAT(a,b)		a##b
#define CE_MAKE_NAME(a,b)	CE_CONCAT(a,b)

/* OS Dependent Headers *******************************************************/

#if defined(CE_PLATFORM_WINDOWS)
	#include <windows.h>
	#include <CL/cl.h>
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <sys/time.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdarg.h>

	#if defined(CE_PLATFORM_MACOSX)
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

#if defined(CE_DEBUG_BUILD)
#define ceAssert(expr) \
    ((expr) ? (void)0 : \
        ceCritical(CE_DEFAULT_SESSION, "Assertion \"%s\" failed in %s, line %d!\n", \
               #expr, __FILE__, __LINE__))
#else
#define ceAssert(expr) do{ } while(0)
#endif

/******************************************************************************/

CE_EXTERN_C_END

#endif	/* __CE_PLATFORM_H__ */

